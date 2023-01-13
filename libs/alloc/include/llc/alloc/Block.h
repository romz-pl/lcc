#pragma once

namespace llc {

struct Block {
    Block *next{nullptr};
    char *limit{nullptr};
    char *avail{nullptr};
};

}
