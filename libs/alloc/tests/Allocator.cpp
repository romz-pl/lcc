#include <gtest/gtest.h>
#include <lcc/alloc/Allocator.h>

namespace lcc::test {

TEST(Allocator, allocate)
{
    constexpr unsigned long n{123};
    constexpr unsigned int a{0};
    void* p = Allocator::allocate(n, a);
    ASSERT_TRUE(p != nullptr);
}

}
