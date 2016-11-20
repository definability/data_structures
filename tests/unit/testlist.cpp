#include "testlist.hpp"

TYPED_TEST(ListTest, ParametrisedConstructorCreatesEqual) {
    typename TestFixture::List_ list_a(0);
    typename TestFixture::List_ list_b(0);
    ASSERT_TRUE(list_a == list_b);
}
