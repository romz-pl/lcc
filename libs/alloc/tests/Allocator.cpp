#include <gtest/gtest.h>
#include <lcc/alloc/Allocator.h>

namespace lcc::test {

TEST(Allocator, get)
{
    Allocator allocator;
    constexpr std::size_t n{123U};
    constexpr std::size_t a{0};
    std::byte* p = allocator.get(n, a);
    EXPECT_TRUE(p != nullptr);
}

TEST(Allocator, get_too_large)
{
    Allocator allocator;
    constexpr std::size_t n{2U * Block::blockSize};
    constexpr std::size_t a{0};
    EXPECT_ANY_THROW(allocator.get(n, a));
}

TEST(Allocator, area_out_of_bound)
{
    Allocator allocator;
    constexpr std::size_t n{321U};
    constexpr std::size_t a{Allocator::arenaNo + 1U};
    EXPECT_ANY_THROW(allocator.get(n, a));
}

TEST(Allocator, free)
{
    Allocator allocator;
    constexpr std::size_t n{123U};
    constexpr std::size_t a{0};
    std::size_t total{0};

    while(total < 5U * 10U * 1024U)
    {
        std::byte* p = allocator.get(n, a);
        ASSERT_TRUE(p != nullptr);
        total += n;
    }

    allocator.free(a);
}

}
