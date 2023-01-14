#include <lcc/alloc/Allocator.h>

#include <spdlog/spdlog.h>

namespace lcc {

std::byte* Allocator::get(std::size_t n, std::size_t a)
{
    if(a >= arena.size())
    {
        const std::string txt = fmt::format("Allocator::get: condition [a < arena.size()] does not hold with a={}.", a);
        spdlog::critical(txt);
        throw std::runtime_error{txt};
    }
    return arena[a].get(n);
}

void Allocator::free(std::size_t a)
{
    if(a >= arena.size())
    {
        const std::string txt = fmt::format("Allocator::free: condition [a < arena.size()] does not hold with a={}.", a);
        spdlog::critical(txt);
        throw std::runtime_error{txt};
    }
    arena[a].free();
}

}
