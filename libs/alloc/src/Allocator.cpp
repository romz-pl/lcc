#include <lcc/alloc/Allocator.h>

#include <spdlog/spdlog.h>
#include <numeric>

namespace lcc {

std::byte* Allocator::get(std::size_t n, std::size_t a)
{
    if(n == 0)
    {
        const std::string txt = fmt::format("Allocator::get: condition [n > 0] does not hold with n={}.", n);
        spdlog::critical(txt);
        throw std::runtime_error{txt};
    }

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

std::size_t Allocator::getUsedSpace() const
{
    auto plus = [](std::size_t sum, const Arena& a)
    {
        return sum + a.getUsedSpace();
    };

    return std::accumulate(arena.begin(), arena.end(), 0U, plus);
}

}
