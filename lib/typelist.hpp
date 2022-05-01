#include "helpers.hpp"

#include <cstdint>
#include <iostream>
#include <type_traits>
#include <utility>

namespace fl_meta {

////////////////////////////////////////////////////////////////////////////////

template <typename ...TArgs>
struct TTypeList {
    using THead = TNull;
    using TTail = TNull;
};

template <typename TFirst, typename ...TLast>
struct TTypeList<TFirst, TLast...> {
    using THead = TFirst;
    using TTail = TTypeList<TLast...>;
};

using EmptyTypeList = TTypeList<>;

////////////////////////////////////////////////////////////////////////////////

template <typename TList>
struct TIsEmpty
    : public std::true_type {};

template <>
struct TIsEmpty<EmptyTypeList>
    : public std::true_type {};

template <typename ...TArgs>
struct TIsEmpty<TTypeList<TArgs...>>
    : public std::integral_constant<bool,
        std::is_same_v<typename TTypeList<TArgs...>::THead, TNull>
            && TIsEmpty<typename TTypeList<TArgs...>::TTail>::value> {};

template <typename T>
inline constexpr bool kIsEmpty = TIsEmpty<T>::value;

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TList>
struct TContains
    : public std::false_type {};

template <typename ...TArgs>
struct TContains<TNull, TTypeList<TArgs...>>
    : public std::false_type {};

template <typename T, typename... TArgs>
struct TContains<T, TTypeList<TArgs...>>
    : public std::integral_constant<bool,
        std::is_same_v<typename TTypeList<TArgs...>::THead, T>
            || TContains<T, typename TTypeList<TArgs...>::TTail>::value> {};

template <typename T, typename TList>
inline constexpr bool kContains = TContains<T, TList>::value;

////////////////////////////////////////////////////////////////////////////////

template <typename TList>
struct TLength
    : public std::integral_constant<uint64_t, 0> {};

template <typename ...TArgs>
struct TLength<TTypeList<TArgs...>>
    : std::integral_constant<uint64_t,
        kIsEmpty<TTypeList<TArgs...>>
            ? 0
            : 1 + TLength<typename TTypeList<TArgs...>::TTail>::value> {};

template <typename TList>
inline constexpr uint64_t kLength = TLength<TList>::value;

////////////////////////////////////////////////////////////////////////////////

template <uint64_t Index, typename TList>
struct TTypeAt {
    using TType = TNull;
};

template <typename ...TArgs>
struct TTypeAt<0, TTypeList<TArgs...>> {
    using TType = typename TTypeList<TArgs...>::THead;
};

template <uint64_t N, typename ...TArgs>
struct TTypeAt<N, TTypeList<TArgs...>> {
    static_assert(N < kLength<TTypeList<TArgs...>>, "Index is too big");
    using TType =
        typename TTypeAt<N - 1, typename TTypeList<TArgs...>::TTail>::TType;
};

template <uint64_t Index, typename TList>
using TypeAt = typename TTypeAt<Index, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TList>
struct TIndexOf
    : public std::integral_constant<
        uint64_t,
        std::numeric_limits<uint64_t>::max()> {};

template <typename T>
struct TIndexOf<T, EmptyTypeList>
    : public std::integral_constant<
        uint64_t,
        std::is_same_v<T, EmptyTypeList::THead>
            ? 0
            : std::numeric_limits<uint64_t>::max()> {
    static_assert(
        std::is_same_v<T, typename EmptyTypeList::THead>,
        "Current type list doesn't contain specified type");
};

template <typename T, typename ...TArgs>
struct TIndexOf<T, TTypeList<T, TArgs...>>
    : public std::integral_constant<uint64_t, 0> {};

template <typename T, typename ...TArgs>
struct TIndexOf<T, TTypeList<TArgs...>>
    : std::integral_constant<
        uint64_t,
        1 + TIndexOf<T, typename TTypeList<TArgs...>::TTail>::value> {};

template <typename T, typename TList>
inline constexpr int kIndexOf = TIndexOf<T, TList>::value;

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TList>
struct TAdd {};

template <typename T, typename ...TArgs>
struct TAdd<T, TTypeList<TArgs...>> {
    using TType = TTypeList<TArgs..., T>;
};

template <typename T, typename TList>
using Add = typename TAdd<T, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TList>
struct TAppend {};

template <typename T, typename ...TArgs>
struct TAppend<T, TTypeList<TArgs...>> {
    using TType = TTypeList<TArgs..., T>;
};

template <typename ...TArgs1, typename ...TArgs2>
struct TAppend<TTypeList<TArgs1...>, TTypeList<TArgs2...>> {
    using TType = TTypeList<TArgs2..., TArgs1...>;
};

template <typename T, typename TList>
using Append = typename TAppend<T, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TList>
struct TErase {};

template <typename T>
struct TErase<T, EmptyTypeList> {
    using TType = EmptyTypeList;
};

template <typename T, typename ...TArgs>
struct TErase<T, TTypeList<TArgs...>> {
    using TType = typename std::conditional_t<
        std::is_same_v<typename TTypeList<TArgs...>::THead, T>,
        typename TTypeList<TArgs...>::TTail,
        Append<
            typename TErase<T, typename TTypeList<TArgs...>::TTail>::TType,
            TTypeList<typename TTypeList<TArgs...>::THead>>>;
};

template <typename T, typename TList>
using Erase = typename TErase<T, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TList>
struct TEraseAll {};

template <typename T>
struct TEraseAll<T, EmptyTypeList> {
    using TType = EmptyTypeList;
};

template <typename T, typename ...TArgs>
struct TEraseAll<T, TTypeList<TArgs...>> {
    using TType = typename std::conditional_t<
        std::is_same_v<typename TTypeList<TArgs...>::THead, T>,
        typename TEraseAll<T, typename TTypeList<TArgs...>::TTail>::TType,
        Append<
            typename TEraseAll<T, typename TTypeList<TArgs...>::TTail>::TType,
            TTypeList<typename TTypeList<TArgs...>::THead>>>;
};
;
template <typename T, typename TList>
using EraseAll = typename TEraseAll<T, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <typename TList>
struct TEraseDuplicates {};

template <>
struct TEraseDuplicates<EmptyTypeList> {
    using TType = EmptyTypeList;
};

template <typename ...TArgs>
struct TEraseDuplicates<TTypeList<TArgs...>> {
public:
    using TType = Append<
        Erase<
            typename TTypeList<TArgs...>::THead,
            typename TEraseDuplicates<
                typename TTypeList<TArgs...>::TTail>::TType>,
        TTypeList<typename TTypeList<TArgs...>::THead>>;
};

template <typename TList>
using EraseDuplicates = typename TEraseDuplicates<TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <typename TTo, typename TFrom, typename TList>
struct TReplace {};

template <typename TTo, typename TFrom>
struct TReplace<TTo, TFrom, EmptyTypeList> {
    using TType = EmptyTypeList;
};

template <typename TTo, typename TFrom, typename ...TArgs>
struct TReplace<TTo, TFrom, TTypeList<TArgs...>> {
    using TType = typename std::conditional_t<
        std::is_same_v<typename TTypeList<TArgs...>::THead, TFrom>,
        Append<
            typename TTypeList<TArgs...>::TTail,
            TTypeList<TTo>>,
        Append<
            typename TReplace<
                TTo,
                TFrom,
                typename TTypeList<TArgs...>::TTail>::TType,
            TTypeList<typename TTypeList<TArgs...>::THead>>>;
};

template <typename TTo, typename TFrom, typename TList>
using Replace = typename TReplace<TTo, TFrom, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <typename TTo, typename TFrom, typename TList>
struct TReplaceAll {};

template <typename TTo, typename TFrom>
struct TReplaceAll<TTo, TFrom, EmptyTypeList> {
    using TType = EmptyTypeList;
};

template <typename TTo, typename TFrom, typename ...TArgs>
struct TReplaceAll<TTo, TFrom, TTypeList<TArgs...>> {
    using TType = typename std::conditional_t<
        std::is_same_v<typename TTypeList<TArgs...>::THead, TFrom>,
        Append<
            typename TReplaceAll<
                TTo,
                TFrom,
                typename TTypeList<TArgs...>::TTail>::TType,
            TTypeList<TTo>>,
        Append<
            typename TReplaceAll<
                TTo,
                TFrom,
                typename TTypeList<TArgs...>::TTail>::TType,
            TTypeList<typename TTypeList<TArgs...>::THead>>>;
};

template <typename TTo, typename TFrom, typename TList>
using ReplaceAll = typename TReplaceAll<TTo, TFrom, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <typename TBase, typename TList>
struct TMostDerived {};

template <typename TBase>
struct TMostDerived<TBase, EmptyTypeList> {
    using TType = TBase;
};

template <typename TBase, typename ...TArgs>
struct TMostDerived<TBase, TTypeList<TArgs...>> {
private:
    using TCandidate = typename TMostDerived<
        TBase,
        typename TTypeList<TArgs...>::TTail>::TType;

public:
    using TType = std::conditional_t<
        std::is_base_of_v<TCandidate, typename TTypeList<TArgs...>::THead>,
        typename TTypeList<TArgs...>::THead,
        TCandidate>;
};

template <typename TBase, typename TList>
using MostDerived = typename TMostDerived<TBase, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <typename TList>
struct TDerivedToFront {};

template <>
struct TDerivedToFront<EmptyTypeList> {
    using TType = EmptyTypeList;
};

template <typename ...TArgs>
struct TDerivedToFront<TTypeList<TArgs...>> {
private:
    using TCandidate = MostDerived<
        typename TTypeList<TArgs...>::THead,
        typename TTypeList<TArgs...>::TTail>;
    using TTemp = Replace<
        typename TTypeList<TArgs...>::THead,
        TCandidate,
        typename TTypeList<TArgs...>::TTail>;
    using TList = typename TDerivedToFront<TTemp>::TType;

public:
    using TType = Append<
        typename TDerivedToFront<TList>::TType,
        TTypeList<TCandidate>>;
};

template <typename TList>
using DerivedToFront = typename TDerivedToFront<TList>::TType;

////////////////////////////////////////////////////////////////////////////////

namespace internal {

template <uint64_t IndexBegin, uint64_t IndexEnd, typename TList>
struct TSliceHelper {};

template <uint64_t IndexBegin, uint64_t IndexEnd>
struct TSliceHelper<IndexBegin, IndexEnd, EmptyTypeList> {
    using TType = EmptyTypeList;
};

template <uint64_t IndexBegin, typename ...TArgs>
struct TSliceHelper<IndexBegin, IndexBegin + 1, TTypeList<TArgs...>> {
    using TType = TTypeList<TypeAt<IndexBegin, TTypeList<TArgs...>>>;
};

template <uint64_t IndexBegin, uint64_t IndexEnd, typename ...TArgs>
struct TSliceHelper<IndexBegin, IndexEnd, TTypeList<TArgs...>> {
private:
    static_assert(IndexEnd > IndexBegin, "Invalid range");

public:
    using TType = Add<
        TypeAt<IndexEnd - 1, TTypeList<TArgs...>>,
        typename TSliceHelper<
            IndexBegin,
            IndexEnd - 1,
            TTypeList<TArgs...>>::TType>;
};

template <uint64_t IndexBegin, uint64_t IndexEnd, typename TList>
using SliceHelper = typename TSliceHelper<IndexBegin, IndexEnd, TList>::TType;

}   // namespace internal

template <uint64_t IndexBegin, uint64_t IndexAfterEnd, typename TList>
struct TSlice {};

template <uint64_t IndexBegin, uint64_t IndexEnd, typename ...TArgs>
struct TSlice<IndexBegin, IndexEnd, TTypeList<TArgs...>> {
    using TType = internal::SliceHelper<
        IndexBegin,
        IndexEnd,
        TTypeList<TArgs...>>;
};

template <uint64_t IndexBegin, uint64_t IndexEnd, typename TList>
using Slice = typename TSlice<IndexBegin, IndexEnd, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <uint64_t Index, typename TList>
struct TCutTo {};

template <uint64_t Index, typename ...TArgs>
struct TCutTo<Index, TTypeList<TArgs...>> {
    using TType = typename TSlice<0, Index, TTypeList<TArgs...>>::TType;
};

template <uint64_t Index, typename TList>
using CutTo = typename TCutTo<Index, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

template <uint64_t Index, typename TList>
struct TCutFrom {};

template <uint64_t Index, typename ...TArgs>
struct TCutFrom<Index, TTypeList<TArgs...>> {
private:
    using TList = TTypeList<TArgs...>;

public:
    using TType = typename TSlice<Index, kLength<TList>, TList>::TType;
};

template <uint64_t Index, typename TList>
using CutFrom = typename TCutFrom<Index, TList>::TType;

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& ostr, EmptyTypeList) {
    ostr << "{}";
    return ostr;
}

template <typename TList>
void PrintTypeListHelper(TList, std::ostream&) {};

template <typename T>
void PrintTypeListHead(T, std::ostream& ostr) {
    ostr << typeid(T).name();
}

template <typename ...TArgs>
void PrintTypeListHead(TTypeList<TArgs...> tl, std::ostream& ostr) {
    ostr << tl;
}

template <typename THead, typename ...TArgs>
void PrintTypeListHelper(TTypeList<THead, TArgs...>, std::ostream& ostr) {
    PrintTypeListHead(THead{}, ostr);
    if (!kIsEmpty<TTypeList<TArgs...>>) {
        ostr << ' ';
        PrintTypeListHelper<TArgs...>(TTypeList<TArgs...>{}, ostr);
    }
}

template <typename ...TArgs>
std::ostream& operator<<(std::ostream& ostr, TTypeList<TArgs...> tl) {
    ostr << '{';
    PrintTypeListHelper(tl, ostr);
    ostr << '}';
    return ostr;
}

}   // namespace fl_meta
