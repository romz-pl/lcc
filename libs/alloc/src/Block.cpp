#include <lcc/alloc/Block.h>

#include <cassert>

namespace lcc {

std::byte* Block::get(std::size_t n)
{
    assert(n > 0);

    if(availableSpace < n)
    {
        return nullptr;
    }

    begin += n;
    availableSpace -= n;
    return begin;
}

void Block::free()
{
    availableSpace = blockSize;
    begin = buffer.data();
}

std::size_t Block::getUsedSpace() const
{
    return buffer.size() - availableSpace;
}

}
