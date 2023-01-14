#include <gtest/gtest.h>
#include <lcc/alloc/Arena.h>

namespace lcc::test {

TEST(Arena, get)
{
    Arena arena;
    constexpr std::size_t n{Block::blockSize / 8};
    std::byte* p = arena.get(n);
    EXPECT_TRUE(p != nullptr);
}

TEST(Arena, get_too_large)
{
    Arena arena;
    constexpr std::size_t n{2U * Block::blockSize};
    EXPECT_ANY_THROW(arena.get(n));
}

TEST(Arena, free)
{
    Arena arena;
    constexpr std::size_t n{Block::blockSize / 10U};
    std::size_t total{0};

    while(total < 5U * Block::blockSize)
    {
        std::byte* p = arena.get(n);
        EXPECT_NE(p, nullptr);
        total += n;
    }

    arena.free();
}

TEST(Arena, getUsedSpace)
{
    Arena arena;
    constexpr std::size_t n{Block::blockSize / 4};
    std::size_t total{0};

    while(total < 5U * Block::blockSize)
    {
        std::byte* p = arena.get(n);
        EXPECT_NE(p, nullptr);
        total += n;
        EXPECT_EQ(arena.getUsedSpace(), total);
    }

    arena.free();
    EXPECT_EQ(arena.getUsedSpace(), 0U);
}

}
