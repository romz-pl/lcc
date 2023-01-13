#pragma once

namespace lcc {

union Align {
    long l;
    char *p;
    double d;
    int (*f)(void);
};

}
