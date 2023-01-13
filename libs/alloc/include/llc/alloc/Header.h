#pragma once

#include <llc/alloc/Block.h>
#include <llc/alloc/Align.h>

namespace llc {

union Header {
    Block b;
    Align a;
};

}
