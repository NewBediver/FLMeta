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
    static_assert(std::is_same_v<
        TErase<int, EmptyTypeList>::TType,
        Erase<int, EmptyTypeList>>);
    static_assert(std::is_same_v<Erase<int, EmptyTypeList>, EmptyTypeList>);

    static_assert(std::is_same_v<
        TErase<int, TTypeList<int>>::TType,
        Erase<int, TTypeList<int>>>);
    static_assert(std::is_same_v<Erase<int, TTypeList<int>>, EmptyTypeList>);

    static_assert(std::is_same_v<
        TErase<
            int,
            TTypeList<int, TTypeList<bool>, std::string>>::TType,
        Erase<
            int,
            TTypeList<int, TTypeList<bool>, std::string>>>);
    static_assert(std::is_same_v<
        Erase<int, TTypeList<int, TTypeList<bool>, std::string>>,
        TTypeList<TTypeList<bool>, std::string>>);

    static_assert(std::is_same_v<
        TErase<
            TTypeList<bool>,
            TTypeList<int, TTypeList<bool>, std::string>>::TType,
        Erase<
            TTypeList<bool>,
            TTypeList<int, TTypeList<bool>, std::string>>>);
    static_assert(std::is_same_v<
        Erase<TTypeList<bool>, TTypeList<int, TTypeList<bool>, std::string>>,
        TTypeList<int, std::string>>);

    static_assert(std::is_same_v<
        TErase<
            std::string,
            TTypeList<int, TTypeList<bool>, std::string>>::TType,
        Erase<
            std::string,
            TTypeList<int, TTypeList<bool>, std::string>>>);
    static_assert(std::is_same_v<
        Erase<std::string, TTypeList<int, TTypeList<bool>, std::string>>,
        TTypeList<int, TTypeList<bool>>>);

    static_assert(std::is_same_v<
        TErase<
            char,
            TTypeList<int, TTypeList<bool>, std::string>>::TType,
        Erase<
            char,
            TTypeList<int, TTypeList<bool>, std::string>>>);
    static_assert(std::is_same_v<
        Erase<char, TTypeList<int, TTypeList<bool>, std::string>>,
        TTypeList<int, TTypeList<bool>, std::string>>);

    static_assert(std::is_same_v<
        TErase<
            int,
            TTypeList<int, TTypeList<bool>, int, std::string>>::TType,
        Erase<
            int,
            TTypeList<int, TTypeList<bool>, int, std::string>>>);
    static_assert(std::is_same_v<
        Erase<int, TTypeList<int, TTypeList<bool>, int, std::string>>,
        TTypeList<TTypeList<bool>, int, std::string>>);

    static_assert(std::is_same_v<
        TErase<
            int,
            TTypeList<char, int, TTypeList<bool>, int, std::string>>::TType,
        Erase<
            int,
            TTypeList<char, int, TTypeList<bool>, int, std::string>>>);
    static_assert(std::is_same_v<
        Erase<int, TTypeList<char, int, TTypeList<bool>, int, std::string>>,
        TTypeList<char, TTypeList<bool>, int, std::string>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TEraseAll) {
    static_assert(std::is_same_v<
        TEraseAll<int, EmptyTypeList>::TType,
        EraseAll<int, EmptyTypeList>>);
    static_assert(std::is_same_v<EraseAll<int, EmptyTypeList>, EmptyTypeList>);

    static_assert(std::is_same_v<
        TEraseAll<int, TTypeList<int>>::TType,
        EraseAll<int, TTypeList<int>>>);
    static_assert(std::is_same_v<EraseAll<int, TTypeList<int>>, EmptyTypeList>);

    static_assert(std::is_same_v<
        TEraseAll<
            int,
            TTypeList<int, TTypeList<bool>, std::string>>::TType,
        EraseAll<
            int,
            TTypeList<int, TTypeList<bool>, std::string>>>);
    static_assert(std::is_same_v<
        EraseAll<int, TTypeList<int, TTypeList<bool>, std::string>>,
        TTypeList<TTypeList<bool>, std::string>>);

    static_assert(std::is_same_v<
        TEraseAll<
            TTypeList<bool>,
            TTypeList<int, TTypeList<bool>, std::string>>::TType,
        EraseAll<
            TTypeList<bool>,
            TTypeList<int, TTypeList<bool>, std::string>>>);
    static_assert(std::is_same_v<
        EraseAll<TTypeList<bool>, TTypeList<int, TTypeList<bool>, std::string>>,
        TTypeList<int, std::string>>);

    static_assert(std::is_same_v<
        TEraseAll<
            std::string,
            TTypeList<int, TTypeList<bool>, std::string>>::TType,
        EraseAll<
            std::string,
            TTypeList<int, TTypeList<bool>, std::string>>>);
    static_assert(std::is_same_v<
        EraseAll<std::string, TTypeList<int, TTypeList<bool>, std::string>>,
        TTypeList<int, TTypeList<bool>>>);

    static_assert(std::is_same_v<
        TEraseAll<
            char,
            TTypeList<int, TTypeList<bool>, std::string>>::TType,
        EraseAll<
            char,
            TTypeList<int, TTypeList<bool>, std::string>>>);
    static_assert(std::is_same_v<
        EraseAll<char, TTypeList<int, TTypeList<bool>, std::string>>,
        TTypeList<int, TTypeList<bool>, std::string>>);

    static_assert(std::is_same_v<
        TEraseAll<
            int,
            TTypeList<int, TTypeList<bool>, int, std::string>>::TType,
        EraseAll<
            int,
            TTypeList<int, TTypeList<bool>, int, std::string>>>);
    static_assert(std::is_same_v<
        EraseAll<int, TTypeList<int, TTypeList<bool>, int, std::string>>,
        TTypeList<TTypeList<bool>, std::string>>);

    static_assert(std::is_same_v<
        TEraseAll<
            int,
            TTypeList<char, int, TTypeList<bool>, int, std::string>>::TType,
        EraseAll<
            int,
            TTypeList<char, int, TTypeList<bool>, int, std::string>>>);
    static_assert(std::is_same_v<
        EraseAll<int, TTypeList<char, int, TTypeList<bool>, int, std::string>>,
        TTypeList<char, TTypeList<bool>, std::string>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TEraseDuplicates) {
    static_assert(std::is_same_v<
        TEraseDuplicates<EmptyTypeList>::TType,
        EraseDuplicates<EmptyTypeList>>);
    static_assert(std::is_same_v<
        EraseDuplicates<EmptyTypeList>,
        EmptyTypeList>);

    static_assert(std::is_same_v<
        typename TEraseDuplicates<TTypeList<int>>::TType,
        EraseDuplicates<TTypeList<int>>>);
    static_assert(std::is_same_v<
        EraseDuplicates<TTypeList<int>>,
        TTypeList<int>>);

    static_assert(std::is_same_v<
        typename TEraseDuplicates<TTypeList<int, int, int>>::TType,
        EraseDuplicates<TTypeList<int, int, int>>>);
    static_assert(std::is_same_v<
        EraseDuplicates<TTypeList<int, int, int>>,
        TTypeList<int>>);

    static_assert(std::is_same_v<
        TEraseDuplicates<TTypeList<int, TTypeList<bool>, std::string>>::TType,
        EraseDuplicates<TTypeList<int, TTypeList<bool>, std::string>>>);
    static_assert(std::is_same_v<
        EraseDuplicates<TTypeList<int, TTypeList<bool>, std::string>>,
        TTypeList<int, TTypeList<bool>, std::string>>);

    static_assert(std::is_same_v<
        TEraseDuplicates<TTypeList<
            int,
            int,
            TTypeList<bool>,
            std::string,
            TTypeList<bool>,
            TTypeList<bool, char>>>::TType,
        EraseDuplicates<TTypeList<
            int,
            int,
            TTypeList<bool>,
            std::string,
            TTypeList<bool>,
            TTypeList<bool, char>>>>);
    static_assert(std::is_same_v<
        EraseDuplicates<TTypeList<
            int,
            int,
            TTypeList<bool>,
            std::string,
            TTypeList<bool>,
            TTypeList<bool, char>>>,
        TTypeList<int, TTypeList<bool>, std::string, TTypeList<bool, char>>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TReplace) {
    static_assert(std::is_same_v<
        typename TReplace<int, double, EmptyTypeList>::TType,
        Replace<int, double, EmptyTypeList>>);
    static_assert(std::is_same_v<
        Replace<int, double, EmptyTypeList>,
        EmptyTypeList>);

    static_assert(std::is_same_v<
        typename TReplace<int, char, TTypeList<int>>::TType,
        Replace<int, char, TTypeList<int>>>);
    static_assert(std::is_same_v<
        Replace<int, char, TTypeList<int>>,
        TTypeList<int>>);

    static_assert(std::is_same_v<
        typename TReplace<int, char, TTypeList<std::string>>::TType,
        Replace<int, char, TTypeList<std::string>>>);
    static_assert(std::is_same_v<
        Replace<int, char, TTypeList<std::string>>,
        TTypeList<std::string>>);

    static_assert(std::is_same_v<
        typename TReplace<int, char, TTypeList<char>>::TType,
        Replace<int, char, TTypeList<char>>>);
    static_assert(std::is_same_v<
        Replace<int, char, TTypeList<char>>,
        TTypeList<int>>);

    static_assert(std::is_same_v<
        typename TReplace<
            TTypeList<bool, char>,
            TTypeList<int>,
            TTypeList<std::string, TTypeList<int>, int, TTypeList<int>>>::TType,
        Replace<
            TTypeList<bool, char>,
            TTypeList<int>,
            TTypeList<std::string, TTypeList<int>, int, TTypeList<int>>>>);
    static_assert(std::is_same_v<
        Replace<
            TTypeList<bool, char>,
            TTypeList<int>,
            TTypeList<std::string, TTypeList<int>, int, TTypeList<int>>>,
        TTypeList<
            std::string,
            TTypeList<bool, char>,
            int,
            TTypeList<int>>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TReplaceAll) {
    static_assert(std::is_same_v<
        typename TReplaceAll<int, double, EmptyTypeList>::TType,
        ReplaceAll<int, double, EmptyTypeList>>);
    static_assert(std::is_same_v<
        ReplaceAll<int, double, EmptyTypeList>,
        EmptyTypeList>);

    static_assert(std::is_same_v<
        typename TReplaceAll<int, char, TTypeList<int>>::TType,
        ReplaceAll<int, char, TTypeList<int>>>);
    static_assert(std::is_same_v<
        ReplaceAll<int, char, TTypeList<int>>,
        TTypeList<int>>);

    static_assert(std::is_same_v<
        typename TReplaceAll<int, char, TTypeList<std::string>>::TType,
        ReplaceAll<int, char, TTypeList<std::string>>>);
    static_assert(std::is_same_v<
        ReplaceAll<int, char, TTypeList<std::string>>,
        TTypeList<std::string>>);

    static_assert(std::is_same_v<
        typename TReplaceAll<int, char, TTypeList<char>>::TType,
        ReplaceAll<int, char, TTypeList<char>>>);
    static_assert(std::is_same_v<
        ReplaceAll<int, char, TTypeList<char>>,
        TTypeList<int>>);

    static_assert(std::is_same_v<
        typename TReplaceAll<
            TTypeList<bool, char>,
            TTypeList<int>,
            TTypeList<std::string, TTypeList<int>, int, TTypeList<int>>>::TType,
        ReplaceAll<
            TTypeList<bool, char>,
            TTypeList<int>,
            TTypeList<std::string, TTypeList<int>, int, TTypeList<int>>>>);
    static_assert(std::is_same_v<
        ReplaceAll<
            TTypeList<bool, char>,
            TTypeList<int>,
            TTypeList<std::string, TTypeList<int>, int, TTypeList<int>>>,
        TTypeList<
            std::string,
            TTypeList<bool, char>,
            int,
            TTypeList<bool, char>>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TMostDerived) {
    class TA {};
    class TB: public TA {};
    class TC: public TB {};
    class TD: public TA {};
    class TE: public TD {};
    class TF: public TE {};

    static_assert(std::is_same_v<
        typename TMostDerived<TA, EmptyTypeList>::TType,
        MostDerived<TA, EmptyTypeList>>);
    static_assert(std::is_same_v<
        MostDerived<TA, EmptyTypeList>,
        TA>);

    static_assert(std::is_same_v<
        typename TMostDerived<TA, TTypeList<TA, TD, TB, TC, TF, TE>>::TType,
        MostDerived<TA, TTypeList<TA, TD, TB, TC, TF, TE>>>);
    static_assert(std::is_same_v<
        MostDerived<TA, TTypeList<TA, TD, TB, TC, TF, TE>>,
        TF>);

    static_assert(std::is_same_v<
        typename TMostDerived<TB, TTypeList<TA, TD, TB, TC, TF, TE>>::TType,
        MostDerived<TB, TTypeList<TA, TD, TB, TC, TF, TE>>>);
    static_assert(std::is_same_v<
        MostDerived<TB, TTypeList<TA, TD, TB, TC, TF, TE>>,
        TC>);

    static_assert(std::is_same_v<
        typename TMostDerived<TC, TTypeList<TA, TD, TB, TC, TF, TE>>::TType,
        MostDerived<TC, TTypeList<TA, TD, TB, TC, TF, TE>>>);
    static_assert(std::is_same_v<
        MostDerived<TC, TTypeList<TA, TD, TB, TC, TF, TE>>,
        TC>);

    static_assert(std::is_same_v<
        typename TMostDerived<TD, TTypeList<TA, TD, TB, TC, TF, TE>>::TType,
        MostDerived<TD, TTypeList<TA, TD, TB, TC, TF, TE>>>);
    static_assert(std::is_same_v<
        MostDerived<TD, TTypeList<TA, TD, TB, TC, TF, TE>>,
        TF>);

    static_assert(std::is_same_v<
        typename TMostDerived<TE, TTypeList<TA, TD, TB, TC, TF, TE>>::TType,
        MostDerived<TE, TTypeList<TA, TD, TB, TC, TF, TE>>>);
    static_assert(std::is_same_v<
        MostDerived<TE, TTypeList<TA, TD, TB, TC, TF, TE>>,
        TF>);

    static_assert(std::is_same_v<
        typename TMostDerived<TF, TTypeList<TA, TD, TB, TC, TF, TE>>::TType,
        MostDerived<TF, TTypeList<TA, TD, TB, TC, TF, TE>>>);
    static_assert(std::is_same_v<
        MostDerived<TF, TTypeList<TA, TD, TB, TC, TF, TE>>,
        TF>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TDerivedToFront) {
    class TA {};
    class TB: public TA {};
    class TC: public TB {};
    class TD: public TA {};
    class TE: public TD {};
    class TF: public TE {};

    static_assert(std::is_same_v<
        typename TDerivedToFront<EmptyTypeList>::TType,
        DerivedToFront<EmptyTypeList>>);
    static_assert(std::is_same_v<
        DerivedToFront<EmptyTypeList>,
        EmptyTypeList>);

    static_assert(std::is_same_v<
        typename TDerivedToFront<TTypeList<TA, TD, TB, TC, TF, TE>>::TType,
        DerivedToFront<TTypeList<TA, TD, TB, TC, TF, TE>>>);
    static_assert(std::is_same_v<
        DerivedToFront<TTypeList<TA, TD, TB, TC, TF, TE>>,
        TTypeList<TF, TE, TC, TB, TD, TA>>);

    static_assert(std::is_same_v<
        typename TDerivedToFront<
            TTypeList<TA, TD, TF, TD, TB, TC, TE, TB, TF, TC>>::TType,
        DerivedToFront<TTypeList<TA, TD, TF, TD, TB, TC, TE, TB, TF, TC>>>);
    static_assert(std::is_same_v<
        DerivedToFront<TTypeList<TC, TD, TF, TD, TB, TC, TE, TB, TF, TA>>,
        TTypeList<TC, TF, TF, TE, TC, TB, TD, TB, TD, TA>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TCutTo) {
    static_assert(std::is_same_v<
        typename TCutTo<5, EmptyTypeList>::TType,
        CutTo<5, EmptyTypeList>>);
    static_assert(std::is_same_v<
        CutTo<5, EmptyTypeList>,
        EmptyTypeList>);
   
    static_assert(std::is_same_v<
        typename TCutTo<3, TTypeList<int, std::string, char>>::TType,
        CutTo<3, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        CutTo<3, TTypeList<int, std::string, char>>,
        TTypeList<int, std::string, char>>);

    static_assert(std::is_same_v<
        typename TCutTo<2, TTypeList<int, std::string, char>>::TType,
        CutTo<2, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        CutTo<2, TTypeList<int, std::string, char>>,
        TTypeList<int, std::string>>);

    static_assert(std::is_same_v<
        typename TCutTo<1, TTypeList<int, std::string, char>>::TType,
        CutTo<1, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        CutTo<1, TTypeList<int, std::string, char>>,
        TTypeList<int>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TCutFrom) {
    static_assert(std::is_same_v<
        typename TCutFrom<5, EmptyTypeList>::TType,
        CutFrom<5, EmptyTypeList>>);
    static_assert(std::is_same_v<
        CutFrom<5, EmptyTypeList>,
        EmptyTypeList>);
   
    static_assert(std::is_same_v<
        typename TCutFrom<0, TTypeList<int, std::string, char>>::TType,
        CutFrom<0, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        CutFrom<0, TTypeList<int, std::string, char>>,
        TTypeList<int, std::string, char>>);

    static_assert(std::is_same_v<
        typename TCutFrom<1, TTypeList<int, std::string, char>>::TType,
        CutFrom<1, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        CutFrom<1, TTypeList<int, std::string, char>>,
        TTypeList<std::string, char>>);

    static_assert(std::is_same_v<
        typename TCutFrom<2, TTypeList<int, std::string, char>>::TType,
        CutFrom<2, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        CutFrom<2, TTypeList<int, std::string, char>>,
        TTypeList<char>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, TSlice) {
    static_assert(std::is_same_v<
        typename TSlice<2, 5, EmptyTypeList>::TType,
        Slice<2, 5, EmptyTypeList>>);
    static_assert(std::is_same_v<
        Slice<2, 5, EmptyTypeList>,
        EmptyTypeList>);
   
    static_assert(std::is_same_v<
        typename TSlice<0, 3, TTypeList<int, std::string, char>>::TType,
        Slice<0, 3, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        Slice<0, 3, TTypeList<int, std::string, char>>,
        TTypeList<int, std::string, char>>);

    static_assert(std::is_same_v<
        typename TSlice<0, 2, TTypeList<int, std::string, char>>::TType,
        Slice<0, 2, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        Slice<0, 2, TTypeList<int, std::string, char>>,
        TTypeList<int, std::string>>);

    static_assert(std::is_same_v<
        typename TSlice<0, 1, TTypeList<int, std::string, char>>::TType,
        Slice<0, 1, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        Slice<0, 1, TTypeList<int, std::string, char>>,
        TTypeList<int>>);

    static_assert(std::is_same_v<
        typename TSlice<1, 3, TTypeList<int, std::string, char>>::TType,
        Slice<1, 3, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        Slice<1, 3, TTypeList<int, std::string, char>>,
        TTypeList<std::string, char>>);

    static_assert(std::is_same_v<
        typename TSlice<1, 2, TTypeList<int, std::string, char>>::TType,
        Slice<1, 2, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        Slice<1, 2, TTypeList<int, std::string, char>>,
        TTypeList<std::string>>);

    static_assert(std::is_same_v<
        typename TSlice<2, 3, TTypeList<int, std::string, char>>::TType,
        Slice<2, 3, TTypeList<int, std::string, char>>>);
    static_assert(std::is_same_v<
        Slice<2, 3, TTypeList<int, std::string, char>>,
        TTypeList<char>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TTypeList, Printer) {
    auto printer = [&] (const auto& type_list) {
        std::stringstream ss;
        ss << type_list;
        return ss.str();
    };

    EXPECT_EQ("{}", printer(EmptyTypeList{}));
    EXPECT_EQ(
        std::string("{") + typeid(int).name() + " " + typeid(char).name() + " "
            + typeid(std::string).name() + "}",
        printer(TTypeList<int, char, std::string>{}));
    EXPECT_EQ(
        std::string("{") + typeid(int).name() + " {" + typeid(char).name()
            + " {" + typeid(bool).name() + "} " + typeid(std::string).name()
            + " {" + typeid(char).name() + "}} " + typeid(bool).name() + " "
            + typeid(int).name() + "}",
        printer(TTypeList<
            int,
            TTypeList<
                char,
                TTypeList<bool>,
                std::string,
                TTypeList<char>>,
            bool,
            int>{}));
}

}   // namespace fl_meta::test
