#pragma once

#include <lcc/alloc/Block.h>
#include <array>
#include <list>

namespace lcc {

class Arena
{
public:
    Arena();
    std::byte* get(std::size_t n);

private:
    std::list<Block> block;

    std::list<Block>::iterator currentBloc;

};
}
