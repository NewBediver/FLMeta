#include "helpers.hpp"

#include <cstdint>
#include <type_traits>

namespace fl_meta {

////////////////////////////////////////////////////////////////////////////////

template <typename ...TArgs>
struct TTypeList {
    using head = TNull;
    using tail = TNull;
};

template <typename TFirst, typename ...TLast>
struct TTypeList<TFirst, TLast...> {
    using head = TFirst;
    using tail = TTypeList<TLast...>;
};

using TEmptyTypeList = TTypeList<>;

////////////////////////////////////////////////////////////////////////////////
// TODO: Test IsEmpty

template <typename TList>
struct TIsEmpty
    : public std::true_type {};

template <>
struct TIsEmpty<TEmptyTypeList>
    : public std::true_type {};

template <typename ...TArgs>
struct TIsEmpty<TTypeList<TArgs...>>
    : public std::integral_constant<bool,
        std::is_same<typename TTypeList<TArgs...>::head, TNull>::value
            && TIsEmpty<typename TTypeList<TArgs...>::tail>::value> {};

////////////////////////////////////////////////////////////////////////////////
// TODO: Test Contains

template <typename TFirst, typename TList>
struct TContains
    : public std::true_type {};

template <typename ...TArgs>
struct TContains<TNull, TTypeList<TArgs...>>
    : public std::true_type {};

template <typename THead, typename... TArgs>
struct TContains<THead, TTypeList<TArgs...>>
    : public std::integral_constant<bool,
        std::is_same<typename TTypeList<TArgs...>::head, THead>::value
            || TContains<THead, typename TTypeList<TArgs...>::tail>::value> {};

////////////////////////////////////////////////////////////////////////////////
// TODO: Test Length

template <typename TList>
struct TLength
    : public std::integral_constant<uint64_t, 0> {};

template <typename ...TArgs>
struct TLength<TTypeList<TArgs...>>
    : std::integral_constant<uint64_t,
        TIsEmpty<TTypeList<TArgs...>>::value
            ? 0
            : 1 + TLength<typename TTypeList<TArgs...>::tail>::value> {};

////////////////////////////////////////////////////////////////////////////////
//// TODO: Test TypeAt

template <uint64_t Index, typename TList>
struct TTypeAt {
    using type = TNull;
};

template <typename ...TArgs>
struct TTypeAt<0, TTypeList<TArgs...>> {
    using type = typename TTypeList<TArgs...>::head;
};

template <uint64_t N, typename ...TArgs>
struct TTypeAt<N, TTypeList<TArgs...>> {
    static_assert(N < TLength<TTypeList<TArgs...>>::value, "Index is too big");
    using type =
        typename TTypeAt<N - 1, typename TTypeList<TArgs...>::tail>::type;
};

}   // namespace fl_meta

// TODO: Searching
// TODO: Appending
// TODO: Erasing
// TODO: Erasing dublicates
// TODO: Replacing
// TODO: Partial ordering
// TODO: Slicing
// TODO: Printing
