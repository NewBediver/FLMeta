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
// TODO: Test IsEmpty

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
// TODO: Test Contains

template <typename T, typename TList>
struct TContains
    : public std::true_type {};

template <typename ...TArgs>
struct TContains<TNull, TTypeList<TArgs...>>
    : public std::true_type {};

template <typename T, typename... TArgs>
struct TContains<T, TTypeList<TArgs...>>
    : public std::integral_constant<bool,
        std::is_same_v<typename TTypeList<TArgs...>::THead, T>
            || TContains<typename TTypeList<TArgs...>::TTail, T>::value> {};

template <typename T, typename TList>
inline constexpr bool kContains = TContains<T, TList>::value;

////////////////////////////////////////////////////////////////////////////////
// TODO: Test Length

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
//// TODO: Test TypeAt

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
// TODO: Test IndexOf

template <typename T, typename TList>
struct TIndexOf
    : public std::integral_constant<uint64_t, 0> {};

template <typename T, typename ...TArgs>
struct TIndexOf<T, TTypeList<TArgs...>>
    : std::integral_constant<uint64_t,
        std::is_same_v<typename TTypeList<TArgs...>::THead, T>
            ? 0
            : 1 + TIndexOf<T, typename TTypeList<TArgs...>::TTail>::value> {
    static_assert(
        std::is_same_v<typename TTypeList<TArgs...>::TTail, TNull>,
        "Current type list doesn't contain specified type");
};

template <typename T, typename TList>
inline constexpr int kIndexOf = TIndexOf<T, TList>::value;

////////////////////////////////////////////////////////////////////////////////
// TODO: Test Add

template <typename T, typename TList>
struct TAdd {};

template <typename T, typename ...TArgs>
struct TAdd<T, TTypeList<TArgs...>> {
    using TType = TTypeList<TArgs..., T>;
};

template <typename T, typename TList>
using Add = typename TAdd<T, TList>::TType;

////////////////////////////////////////////////////////////////////////////////
// TODO: Test Append

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
// TODO: Test Erase

template <typename T, typename TList>
struct TErase {};

template <typename T, typename ...TArgs>
struct TErase<T, TTypeList<TArgs...>> {
    using TType = typename std::conditional_t<
        std::is_same_v<typename TTypeList<TArgs...>::THead, T>,
        typename TTypeList<TArgs...>::TTail,
        Append<T, typename TTypeList<TArgs...>::TTail>>;
};

template <typename T, typename TList>
using Erase = typename TErase<T, TList>::TType;

////////////////////////////////////////////////////////////////////////////////
// TODO: Test EraseAll

template <typename T, typename TList>
struct TEraseAll {};

template <typename T, typename ...TArgs>
struct TEraseAll<T, TTypeList<TArgs...>> {
private:
    using TErased = typename TEraseAll<
        T,
        typename TTypeList<TArgs...>::TTail>::TType;
    using THead = typename TTypeList<TArgs...>::THead;

public:
    using TType = typename std::conditional_t<
        std::is_same_v<THead, T>,
        TErased,
        Append<TErased, TTypeList<THead>>>;
};

template <typename T, typename TList>
using EraseAll = typename TEraseAll<T, TList>::TType;

////////////////////////////////////////////////////////////////////////////////
// TODO: Test EraseDuplicates

template <typename TList>
struct TEraseDuplicates {};

template <>
struct TEraseDuplicates<EmptyTypeList> {
    using TType = EmptyTypeList;
};

template <typename ...TArgs>
struct TEraseDuplicates<TTypeList<TArgs...>> {
private:
    using TTail = typename TEraseDuplicates<
        typename TTypeList<TArgs...>::Tail>::TType;
    using TList = Erase<typename TTypeList<TArgs...>::THead, TTail>;

public:
    using TType = Append<
        TList,
        TEraseDuplicates<typename TTypeList<TArgs...>::TTail>>;
};

template <typename TList>
using EraseDuplicates = typename TEraseDuplicates<TList>::TType;

////////////////////////////////////////////////////////////////////////////////
// TODO: Test Replace

template <typename TTo, typename TFrom, typename TList>
struct TReplace {};

template <typename TTo, typename TFrom, typename ...TArgs>
struct TReplace<TTo, TFrom, TTypeList<TArgs...>> {
    using TType = std::is_same_v<TFrom, typename TTypeList<TArgs...>::THead>
        ? Append<typename TTypeList<TArgs...>::TTail, TTo>
        : Append<typename TReplace<
            typename TTypeList<TArgs...>::TTail,
            typename TTypeList<TArgs...>::THead>::TType;
};

template <typename TTo, typename TFrom, typename TList>
using Replace = typename TReplace<TTo, TFrom, TList>::TType;

////////////////////////////////////////////////////////////////////////////////
// TODO: Test ReplaceAll

template <typename TTo, typename TFrom, typename TList>
struct TReplaceAll {};

template <typename TTo, typename TFrom, typename ...TArgs>
struct TReplaceAll<TTo, TFrom, TTypeList<TArgs...>> {
    using TType = std::is_same_v<TFrom, typename TTypeList<TArgs...>::THead>
        ? Append<Replace<TTo, TFrom, typename TTypeList<TArgs...>::TTail, TTo>
        : Append<
            Replace<TTo, TFrom, typename TTypeList<TArgs...>::TTail>,
            typename TTypeList<TArgs...>::THead>;
};

template <typename TTo, typename TFrom, typename TList>
using ReplaceAll = typename TReplaceAll<TTo, TFrom, TList>::TType;

////////////////////////////////////////////////////////////////////////////////
// TODO: Test MostDerived



////////////////////////////////////////////////////////////////////////////////
// TODO: Test DerivedToFront



////////////////////////////////////////////////////////////////////////////////
// TODO: Test Slice



////////////////////////////////////////////////////////////////////////////////
// TODO: Test operator<<

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
    PrintTypeListHead(std::declval<THead>(), ostr);
    if (!kIsEmpty<TTypeList<TArgs...>>) {
        ostr << ' ';
        PrintTypeListHelper<TArgs...>(std::declval<TTypeList<TArgs...>>(), ostr);
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

// TODO: Partial ordering
// TODO: Slicing
