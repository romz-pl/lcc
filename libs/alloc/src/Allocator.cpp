#include <lcc/alloc/Allocator.h>

#include <cassert>
#include <spdlog/spdlog.h>

namespace lcc {

std::byte* Allocator::get(std::size_t n, std::size_t a)
{
    // spdlog::info("Allocator::get, {} {}", n, a);
    assert(a < arena.size());
    return arena[a].get(n);
}

void Allocator::free(std::size_t a)
{
    assert(a < arena.size());
    arena[a].free();
}

}
