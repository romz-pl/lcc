#include <gtest/gtest.h>
#include <lcc/alloc/Allocator.h>

namespace lcc::test {

TEST(Allocator, get)
{
    Allocator allocator;
    constexpr std::size_t n{123};
    constexpr std::size_t a{0};
    std::byte* p = allocator.get(n, a);
    ASSERT_TRUE(p != nullptr);
}

TEST(Allocator, free)
{
    Allocator allocator;
    constexpr std::size_t n{123};
    constexpr std::size_t a{0};
    std::size_t total{0};

    while(total < 5 * 10 * 1024)
    {
        std::byte* p = allocator.get(n, a);
        ASSERT_TRUE(p != nullptr);
        total += n;
    }

    allocator.free(a);
}

}
