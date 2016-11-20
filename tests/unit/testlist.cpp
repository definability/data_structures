#include "testlist.hpp"

TYPED_TEST(ListTest, ParametrisedConstructorCreatesEqual) {
    typedef typename TestFixture::List_ List_;

    List_ list_a(0);
    List_ list_b(0);
    ASSERT_TRUE(list_a == list_b);
}
