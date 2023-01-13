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
    spdlog::info("Arena::get, {}", n);
    while(currentBloc != block.end())
    {
        std::byte* p = currentBloc->get(n);
        if(p != nullptr)
        {
            return p;
        }
        currentBloc++;
    }

    spdlog::info("block.insert, {}", n);
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
