#pragma once

namespace llc {

union Align {
    long l;
    char *p;
    double d;
    int (*f)(void);
};

}
