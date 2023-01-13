#include <lcc/alloc/Allocator.h>

#include <cassert>

namespace lcc {

std::byte* Allocator::get(std::size_t n, std::size_t a)
{
    assert(a < arena.size());
    return arena[a].get(n);
}

}
