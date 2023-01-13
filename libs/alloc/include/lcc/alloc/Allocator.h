#pragma once

#include <lcc/alloc/Block.h>
#include <array>

namespace lcc {

class Allocator
{
public:
    static void *allocate(unsigned long n, unsigned int a);
    static void deallocate(unsigned int a);
    static void *newarray(unsigned long m, unsigned long n, unsigned int a);

private:
    constexpr static unsigned int blockNo{3U};

    static inline std::array<Block, blockNo> first;
    static inline std::array<Block*, blockNo> arena{&first[0], &first[1], &first[2]};
    static inline Block *freeblocks{nullptr};
};
}
