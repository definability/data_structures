#include "testlist.hpp"

TYPED_TEST(ListTest, DefaultConstructorAndDestructorWorks) {
    typename TestFixture::List_ list;
}

TYPED_TEST(ListTest, CopyConstructorWorks) {
    typename TestFixture::List_ list_a;
    typename TestFixture::List_ list_b(list_a);
}

TYPED_TEST(ListTest, CopyConstructorCreatesEqual) {
    typename TestFixture::List_ list_a;
    typename TestFixture::List_ list_b(list_a);
    ASSERT_TRUE(list_a == list_b);
}

TYPED_TEST(ListTest, ParametrisedConstructorCreatesEqual) {
    typename TestFixture::List_ list_a;
    typename TestFixture::List_ list_b(0);
    ASSERT_TRUE(list_a == list_b);
}
