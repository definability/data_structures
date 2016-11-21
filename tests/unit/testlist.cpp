#include <memory>

#include "testlist.hpp"

TYPED_TEST(ListTest, ParametrisedConstructorCreatesEqual) {
    using List_ = typename TestFixture::List_;

    List_ list_a(0);
    List_ list_b(0);
    ASSERT_TRUE(list_a == list_b);
}

TYPED_TEST(ListTest, ParametrisedConstructorCreatesNotEqual) {
    using List_ = typename TestFixture::List_;

    List_ list_a(0);
    List_ list_b(1);
    ASSERT_TRUE(list_a != list_b);
}

TYPED_TEST(ListTest, ChainsWithEqualParametersAreEqual) {
    using List_ = typename TestFixture::List_;
    using ListPtr_ = std::shared_ptr<const List_>;

    ListPtr_ list_aa = ListPtr_(new List_(1));
    List_ list_ab(2, list_aa);

    ListPtr_ list_ba = ListPtr_(new List_(1));
    List_ list_bb(2, list_ba);

    ASSERT_TRUE(list_ab == list_bb);
}

TYPED_TEST(ListTest, ConstructsListCorrectlyFromInitializerList) {
    using List_ = typename TestFixture::List_;
    using ListPtr_ = std::shared_ptr<const List_>;

    ListPtr_ list_aa = ListPtr_(new List_(2));
    List_ list_ab(1, list_aa);
    List_ list_bb({1, 2});

    ASSERT_TRUE(list_ab == list_bb);
}

TYPED_TEST(ListTest, ChainsWithNotEqualParametersAreNotEqual) {
    using List_ = typename TestFixture::List_;
    using ListPtr_ = std::shared_ptr<const List_>;

    ListPtr_ list_aa = ListPtr_(new List_(1));
    List_ list_ab(2, list_aa);

    ListPtr_ list_ba = ListPtr_(new List_(2));
    List_ list_bb(2, list_ba);

    List_ list_cc{1, 2};

    ASSERT_TRUE(list_ab != list_bb);
    ASSERT_TRUE(list_ab != *list_aa);
    ASSERT_TRUE(list_cc != list_bb);
    ASSERT_TRUE(list_cc != list_ab);
}

TYPED_TEST(ListTest, InsertsFirstNodeProperlyPtr) {
    using List_ = typename TestFixture::List_;
    using ListPtr_ = std::shared_ptr<const List_>;

    ListPtr_ list = ListPtr_(new List_(1));
    ListPtr_ listConstructed = ListPtr_(new List_(2, list));
    ListPtr_ listInserted = list->insert(2);

    List_ listProper{2, 1};

    ASSERT_TRUE(*listInserted == *listConstructed);
    ASSERT_TRUE(*listInserted == listProper);
}

TYPED_TEST(ListTest, InsertsFirstNodeProperly) {
    using List_ = typename TestFixture::List_;
    using ListPtr_ = std::shared_ptr<const List_>;

    ListPtr_ list = ListPtr_(new List_(1));
    List_ listConstructed(2, list);

    List_ listProper{2, 1};

    ASSERT_TRUE(*list->insert(2) == listConstructed);
    ASSERT_TRUE(*list->insert(2) == listProper);
    ASSERT_TRUE(*list->insert(2) == *list->insert(2));
}

TYPED_TEST(ListTest, InsertsLastNodeProperlyPtr) {
    using List_ = typename TestFixture::List_;
    using ListPtr_ = std::shared_ptr<const List_>;

    ListPtr_ list = ListPtr_(new List_(2));
    ListPtr_ listConstructed = ListPtr_(new List_(1, list));

    ListPtr_ listBeforeInsert = ListPtr_(new List_(1));
    ListPtr_ listInserted = listBeforeInsert->insert(2, 1);

    List_ listProper{1, 2};

    ASSERT_TRUE(*listInserted == *listConstructed);
    ASSERT_TRUE(*listInserted == listProper);
}

TYPED_TEST(ListTest, InsertsLastNodeProperly) {
    using List_ = typename TestFixture::List_;
    using ListPtr_ = std::shared_ptr<const List_>;

    ListPtr_ list_ = ListPtr_(new List_(2));
    List_ listConstructed(1, list_);

    ListPtr_ list = ListPtr_(new List_(1));

    List_ listProper{1, 2};

    ASSERT_TRUE(*list->insert(2, 1) == listConstructed);
    ASSERT_TRUE(*list->insert(2, 1) == listProper);
    ASSERT_TRUE(*list->insert(2, 1) == *list->insert(2, 1));
}

TYPED_TEST(ListTest, PopulatesListToThreeElementsProperly) {
    using List_ = typename TestFixture::List_;
    using ListPtr_ = std::shared_ptr<const List_>;

    ListPtr_ list = ListPtr_(new List_(1));

    List_ listProper{1, 2, 3};

    ASSERT_TRUE(*list->insert(3, 1)->insert(2, 1) == listProper);
    ASSERT_TRUE(*list->insert(3, 1)->insert(2, 1) ==
                *list->insert(2, 1)->insert(3, 2));
}

TYPED_TEST(ListTest, TailShouldReturnAllExceptFirstElement) {
    using List_ = typename TestFixture::List_;

    List_ list{1, 2, 3, 4, 5};
    List_ listTail{2, 3, 4, 5};
    ASSERT_TRUE(*list.tail() == listTail);
    ASSERT_FALSE(*list.tail() == list);
}
