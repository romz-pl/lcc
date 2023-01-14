#include <lcc/alloc/Arena.h>

#include <spdlog/spdlog.h>

namespace lcc {

Arena::Arena()
    : block{ 1U }
    , currentBloc{ block.begin() }
{
}

std::byte* Arena::get(std::size_t n)
{
    if(n > Block::blockSize)
    {
        spdlog::critical("Arena::get: requested memory ({}) greater than block size ({})", n, Block::blockSize);
        throw std::bad_alloc{};
    }

    while(currentBloc != block.end())
    {
        std::byte* p = currentBloc->get(n);
        if(p != nullptr)
        {
            return p;
        }
        currentBloc++;
    }

    currentBloc = block.insert( block.end(), Block{} );

    return currentBloc->get(n);
}

void Arena::free()
{
    for(Block& b : block)
    {
        b.free();
    }
    currentBloc = block.begin();
}

}
