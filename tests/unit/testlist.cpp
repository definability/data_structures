#include <memory>

#include "testlist.hpp"

TYPED_TEST(ListTest, ParametrisedConstructorCreatesEqual) {
    using List_ = typename TestFixture::List_;

    List_ list_a(0);
    List_ list_b(0);
    ASSERT_EQ(list_a, list_b);
}

TYPED_TEST(ListTest, ParametrisedConstructorCreatesNotEqual) {
    using List_ = typename TestFixture::List_;

    List_ list_a(0);
    List_ list_b(1);
    ASSERT_NE(list_a, list_b);
}

TYPED_TEST(ListTest, ChainsWithEqualParametersAreEqual) {
    using List_ = typename TestFixture::List_;

    List_ list_aa(1);
    List_ list_ab(2, list_aa);

    List_ list_ba(1);
    List_ list_bb(2, list_ba);

    ASSERT_EQ(list_ab, list_bb);
}

TYPED_TEST(ListTest, ConstructsListCorrectlyFromInitializerList) {
    using List_ = typename TestFixture::List_;

    List_ list_aa(2);
    List_ list_ab(1, list_aa);
    List_ list_bb({1, 2});

    ASSERT_EQ(list_ab, list_bb);
}

TYPED_TEST(ListTest, ChainsWithNotEqualParametersAreNotEqual) {
    using List_ = typename TestFixture::List_;

    List_ list_aa(1);
    List_ list_ab(2, list_aa);

    List_ list_ba(2);
    List_ list_bb(2, list_ba);

    List_ list_cc{1, 2};

    ASSERT_NE(list_ab, list_bb);
    ASSERT_NE(list_ab, list_aa);
    ASSERT_NE(list_cc, list_bb);
    ASSERT_NE(list_cc, list_ab);
}

TYPED_TEST(ListTest, InsertsFirstNodeProperlyPtr) {
    using List_ = typename TestFixture::List_;

    List_ list(1);
    List_ listConstructed(2, list);
    List_ listInserted = list.insert(2);

    List_ listProper{2, 1};

    ASSERT_EQ(listInserted, listConstructed);
    ASSERT_EQ(listInserted, listProper);
}

TYPED_TEST(ListTest, InsertsFirstNodeProperly) {
    using List_ = typename TestFixture::List_;

    List_ list(1);
    List_ listConstructed(2, list);

    List_ listProper{2, 1};

    ASSERT_EQ(list.insert(2), listConstructed);
    ASSERT_EQ(list.insert(2), listProper);
    ASSERT_EQ(list.insert(2), list.insert(2));
}

TYPED_TEST(ListTest, InsertsLastNodeProperlyPtr) {
    using List_ = typename TestFixture::List_;

    List_ list(2);
    List_ listConstructed(1, list);

    List_ listBeforeInsert(1);
    List_ listInserted = listBeforeInsert.insert(2, 1);

    List_ listProper{1, 2};

    ASSERT_EQ(listInserted, listConstructed);
    ASSERT_EQ(listInserted, listProper);
}

TYPED_TEST(ListTest, InsertsLastNodeProperly) {
    using List_ = typename TestFixture::List_;

    List_ list_(2);
    List_ listConstructed(1, list_);

    List_ list(1);

    List_ listProper{1, 2};

    ASSERT_EQ(list.insert(2, 1), listConstructed);
    ASSERT_EQ(list.insert(2, 1), listProper);
    ASSERT_EQ(list.insert(2, 1), list.insert(2, 1));
}

TYPED_TEST(ListTest, PopulatesListToThreeElementsProperly) {
    using List_ = typename TestFixture::List_;

    List_ list(1);

    List_ listProper{1, 2, 3};

    ASSERT_EQ(list.insert(3, 1).insert(2, 1), listProper);
    ASSERT_EQ(list.insert(3, 1).insert(2, 1),
              list.insert(2, 1).insert(3, 2));
}

TYPED_TEST(ListTest, TailShouldReturnAllExceptFirstElement) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2, 3, 4, 5};
    List_ listTail{2, 3, 4, 5};
    ASSERT_EQ(list.tail(), listTail);
    ASSERT_NE(list.tail(), list);
}

TYPED_TEST(ListTest, RemoveShouldRemoveFirstElementByDefault) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2, 3, 4, 5};
    List_ listTail{2, 3, 4, 5};
    ASSERT_EQ(list.tail(), list.remove());
    ASSERT_EQ(list.remove(), list.remove(0));
}

TYPED_TEST(ListTest, RemoveShouldRemoveMiddleElementProperly) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2, 3, 4, 5};
    List_ listAfter{1, 2, 4, 5};
    ASSERT_EQ(list.remove(2), listAfter);
}

TYPED_TEST(ListTest, RemoveShouldRemoveLastElementProperly) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2, 3, 4, 5};
    List_ listAfter{1, 2, 3, 4};
    ASSERT_EQ(list.remove(4), listAfter);
}

TYPED_TEST(ListTest, ReverseShouldWorkForSingleElement) {
    using List_ = typename TestFixture::List_;

    List_ list{1};
    ASSERT_EQ(list.reverse(), list);
}

TYPED_TEST(ListTest, ReverseShouldWorkForTwoElementsList) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2};
    List_ listReversed{2, 1};
    ASSERT_EQ(list.reverse(), listReversed);
}

TYPED_TEST(ListTest, ReverseShouldWorkForMultipleElementsList) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2, 3, 4, 5};
    List_ listReversed{5, 4, 3, 2, 1};
    ASSERT_EQ(list.reverse(), listReversed);
}

TYPED_TEST(ListTest, SliceShouldRemoveFirstElementsCorrectly) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2, 3, 4, 5};
    List_ listSliced{3, 4, 5};

    ASSERT_EQ(list.slice(2), listSliced);
}

TYPED_TEST(ListTest, SliceShouldRemoveLastElementsCorrectly) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2, 3, 4, 5};
    List_ listSliced{1, 2, 3};

    ASSERT_EQ(list.slice(0, 2), listSliced);
}

TYPED_TEST(ListTest, SliceShouldRemoveBorderElementsCorrectly) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2, 3, 4, 5};
    List_ listSliced{2, 3};

    ASSERT_EQ(list.slice(1, 2), listSliced);
}

TYPED_TEST(ListTest, FillSizeCorrect) {
    using List_ = typename TestFixture::List_;

    ASSERT_EQ(List_::fill(10, 0).size(), 10);
}

TYPED_TEST(ListTest, FillLargeSuccess) {
    using List_ = typename TestFixture::List_;

    ASSERT_EQ(List_::fill(1E5, 0).size(), 1E5);
}
