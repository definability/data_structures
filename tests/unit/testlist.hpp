#include "gtest/gtest.h"
#include "list.hpp"

template<typename T> class ListTest : public ::testing::Test {
    public:
        using List_ = const List<const T>;
        static T shared_;
        T value_;
    protected:
        ListTest() {};
        virtual ~ListTest() {};
        virtual void SetUp() {};
        virtual void TearDown() {};
};

typedef ::testing::Types<
    char, short, int, long, long long,
    unsigned char, unsigned short, unsigned int,
    unsigned long, unsigned long long
> MyTypes;

TYPED_TEST_CASE(ListTest, MyTypes);
