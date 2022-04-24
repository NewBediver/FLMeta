#include <gtest/gtest.h>

#include "lib/typelist.hpp"

#include <type_traits>

namespace fl_meta::test {

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TBasic) {
    static_assert(std::is_same_v<EmptyTypeList, EmptyTypeList>);
    static_assert(std::is_same_v<EmptyTypeList::THead, TNull>);
    static_assert(std::is_same_v<EmptyTypeList::TTail, TNull>);

    static_assert(std::is_same_v<
        TTypeList<int, char, std::string>,
        TTypeList<int, char, std::string>>);
    static_assert(!std::is_same_v<
        TTypeList<int, char, std::string>,
        TTypeList<char, int, std::string>>);
    static_assert(!std::is_same_v<
        TTypeList<int, char, std::string>,
        TTypeList<int, char>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TIsEmpty) {
    static_assert(TIsEmpty<EmptyTypeList>::value == kIsEmpty<EmptyTypeList>);
    static_assert(kIsEmpty<EmptyTypeList>);

    static_assert(TIsEmpty<TTypeList<int>>::value == kIsEmpty<TTypeList<char>>);
    static_assert(!kIsEmpty<TTypeList<int>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TContains) {
    static_assert(TContains<TNull, EmptyTypeList>::value
        == kContains<TNull, EmptyTypeList>);
    static_assert(!kContains<TNull, EmptyTypeList>);

    static_assert(TContains<int, EmptyTypeList>::value
        == kContains<int, EmptyTypeList>);
    static_assert(!kContains<int, EmptyTypeList>);

    static_assert(TContains<int, TTypeList<char, int, std::string>>::value
        == kContains<int, TTypeList<char, int, std::string>>);
    static_assert(kContains<int, TTypeList<char, int, std::string>>);

    static_assert(TContains<bool, TTypeList<char, int, std::string>>::value
        == kContains<bool, TTypeList<char, int, std::string>>);
    static_assert(!kContains<bool, TTypeList<char, int, std::string>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TLength) {
    static_assert(TLength<EmptyTypeList>::value == kLength<EmptyTypeList>);
    static_assert(kLength<EmptyTypeList> == 0);

    static_assert(TLength<TTypeList<int>>::value == kLength<TTypeList<int>>);
    static_assert(kLength<TTypeList<int>> == 1);

    static_assert(TLength<TTypeList<char, int, std::string>>::value
        == kLength<TTypeList<int, TTypeList<char, int, std::string>, bool>>);
    static_assert(
        kLength<TTypeList<int, TTypeList<char, int, std::string>, bool>> == 3);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TTypeAt) {
    static_assert(std::is_same_v<
        TTypeAt<0, EmptyTypeList>::TType,
        TypeAt<0, EmptyTypeList>>);
    static_assert(std::is_same_v<TypeAt<0, EmptyTypeList>, TNull>);

    using TList = TTypeList<char, TTypeList<int, bool>, std::string>;

    static_assert(std::is_same_v<
        TTypeAt<0, TList>::TType,
        TypeAt<0, TList>>);
    static_assert(std::is_same_v<TypeAt<0, TList>, char>);

    static_assert(std::is_same_v<
        TTypeAt<1, TList>::TType,
        TypeAt<1, TList>>);
    static_assert(std::is_same_v<TypeAt<1, TList>, TTypeList<int, bool>>);

    static_assert(std::is_same_v<
        TTypeAt<2, TList>::TType,
        TypeAt<2, TList>>);
    static_assert(std::is_same_v<TypeAt<2, TList>, std::string>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TIndexOf) {
    static_assert(TIndexOf<TNull, EmptyTypeList>::value
        == kIndexOf<TNull, EmptyTypeList>);
    static_assert(kIndexOf<TNull, EmptyTypeList> == 0);

    using TList = TTypeList<char, TTypeList<int, bool>, std::string>;

    static_assert(TIndexOf<char, TList>::value == kIndexOf<char, TList>);
    static_assert(kIndexOf<char, TList> == 0);

    static_assert(TIndexOf<TTypeList<int, bool>, TList>::value
        == kIndexOf<TTypeList<int, bool>, TList>);
    static_assert(kIndexOf<TTypeList<int, bool>, TList> == 1);

    static_assert(TIndexOf<std::string, TList>::value
        == kIndexOf<std::string, TList>);
    static_assert(kIndexOf<std::string, TList> == 2);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TAdd) {
    static_assert(std::is_same_v<
        typename TAdd<int, EmptyTypeList>::TType,
        Add<int, EmptyTypeList>>);
    static_assert(std::is_same_v<
        Add<int, EmptyTypeList>,
        TTypeList<int>>);

    static_assert(std::is_same_v<
        typename TAdd<int, TTypeList<char>>::TType,
        Add<int, TTypeList<char>>>);
    static_assert(std::is_same_v<
        Add<int, TTypeList<char>>,
        TTypeList<char, int>>);

    static_assert(std::is_same_v<
        typename TAdd<
            TTypeList<bool, char>,
            TTypeList<std::string, int>>::TType,
        Add<TTypeList<bool, char>, TTypeList<std::string, int>>>);
    static_assert(std::is_same_v<
        Add<TTypeList<bool, char>, TTypeList<std::string, int>>,
        TTypeList<std::string, int, TTypeList<bool, char>>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TAppend) {
    static_assert(std::is_same_v<
        typename TAppend<int, EmptyTypeList>::TType,
        Append<int, EmptyTypeList>>);
    static_assert(std::is_same_v<
        Append<int, EmptyTypeList>,
        TTypeList<int>>);

    static_assert(std::is_same_v<
        typename TAppend<int, TTypeList<char>>::TType,
        Append<int, TTypeList<char>>>);
    static_assert(std::is_same_v<
        Append<int, TTypeList<char>>,
        TTypeList<char, int>>);

    static_assert(std::is_same_v<
        typename TAppend<
            TTypeList<bool, char>,
            TTypeList<std::string, int>>::TType,
        Append<TTypeList<bool, char>, TTypeList<std::string, int>>>);
    static_assert(std::is_same_v<
        Append<TTypeList<bool, char>, TTypeList<std::string, int>>,
        TTypeList<std::string, int, bool, char>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TErase) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TEraseAll) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TEraseDuplicates) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TReplace) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TReplaceAll) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TMostDerived) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TDerivedToFront) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TCutTo) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TCutFrom) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TSlice) {

}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, Printer) {

}

}   // namespace fl_meta::test
