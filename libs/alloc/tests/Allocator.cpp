#include <gtest/gtest.h>
#include <lcc/alloc/Allocator.h>

namespace lcc::test {

TEST(Allocator, get)
{
    Allocator allocator;
    constexpr std::size_t n{Block::blockSize / 8};
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
    constexpr std::size_t n{Block::blockSize / 3U};
    constexpr std::size_t a{Allocator::arenaNo + 1U};
    EXPECT_ANY_THROW(allocator.get(n, a));
}

TEST(Allocator, free)
{
    Allocator allocator;
    constexpr std::size_t n{Block::blockSize / 10U};
    constexpr std::size_t a{0};
    std::size_t total{0};

    while(total < 5U * Block::blockSize)
    {
        std::byte* p = allocator.get(n, a);
        EXPECT_NE(p, nullptr);
        total += n;
    }

    allocator.free(a);
}

TEST(Allocator, getUsedSpace)
{
    Allocator allocator;
    constexpr std::size_t n{Block::blockSize / 4};
    constexpr std::size_t a{2};
    std::size_t total{0};

    while(total < 5U * Block::blockSize)
    {
        std::byte* p = allocator.get(n, a);
        EXPECT_NE(p, nullptr);
        total += n;
        EXPECT_EQ(allocator.getUsedSpace(), total);
    }

    allocator.free(a);
    EXPECT_EQ(allocator.getUsedSpace(), 0U);
}

}
