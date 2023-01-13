#pragma once

#include <lcc/alloc/Block.h>
#include <lcc/alloc/Align.h>

namespace lcc {

union Header {
    Block b;
    Align a;
};

}
