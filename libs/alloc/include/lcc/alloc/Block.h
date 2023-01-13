#pragma once

namespace lcc {

struct Block {
    Block *next{nullptr};
    char *limit{nullptr};
    char *avail{nullptr};
};

}
