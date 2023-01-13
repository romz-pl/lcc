#include <lcc/alloc/Block.h>

#include <spdlog/spdlog.h>

namespace lcc {

std::byte* Block::get(std::size_t n)
{
    spdlog::info("Block::get, {}", n);

    if(availableSpace < n)
        return nullptr;

    begin += n;
    availableSpace -= n;
    return begin;
}

void Block::free()
{
    availableSpace = blockSize;
    begin = buffer.data();
}

}
