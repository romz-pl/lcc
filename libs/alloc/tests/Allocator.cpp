#include <gtest/gtest.h>
#include <llc/alloc/Allocator.h>

namespace llc::test {

TEST(Allocator, allocate)
{
    constexpr unsigned long n{123};
    constexpr unsigned int a{0};
    void* p = Allocator::allocate(n, a);
    ASSERT_TRUE(p != nullptr);
}

}
