#include <memory>

#include "testlist.hpp"

TYPED_TEST(ListTest, ParametrisedConstructorCreatesEqual) {
    typedef typename TestFixture::List_ List_;

    List_ list_a(0);
    List_ list_b(0);
    ASSERT_TRUE(list_a == list_b);
}

TYPED_TEST(ListTest, ChainsWithEqualParametersAreEqual) {
    typedef typename TestFixture::List_ List_;
    typedef std::shared_ptr<const List_> ListPtr_;

    ListPtr_ list_aa = ListPtr_(new List_(1));
    List_ list_ab(2, list_aa);

    ListPtr_ list_ba = ListPtr_(new List_(1));
    List_ list_bb(2, list_ba);

    ASSERT_TRUE(list_ab == list_bb);
}
