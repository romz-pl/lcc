#pragma once

#include <lcc/alloc/Arena.h>

namespace lcc {

class Allocator
{
public:
    std::byte* get(std::size_t n, std::size_t a);
    void free(std::size_t a);

    constexpr static std::size_t arenaNo{3U};

private:
    std::array<Arena, arenaNo> arena;
};

}
