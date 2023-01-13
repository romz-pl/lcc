#include <gtest/gtest.h>
#include <lcc/alloc/Allocator.h>

namespace lcc::test {

TEST(Allocator, get)
{
    Allocator allocator;
    constexpr unsigned long n{123};
    constexpr unsigned int a{0};
    std::byte* p = allocator.get(n, a);
    ASSERT_TRUE(p != nullptr);
}

}
