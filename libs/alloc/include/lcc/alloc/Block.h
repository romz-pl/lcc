#pragma once

#include <array>
#include <cstddef>

namespace lcc {

class Block
{
public:
    std::byte* get(std::size_t n);
    void free();

    constexpr static std::size_t blockSize{ 10 * 1024 };

private:
    std::array<std::byte, blockSize> buffer;

    std::size_t availableSpace{blockSize};

    std::byte *begin{ buffer.data() };
};

}
