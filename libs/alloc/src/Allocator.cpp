#include <lcc/alloc/Allocator.h>

#include <lcc/alloc/Align.h>
#include <lcc/alloc/Header.h>
#include <cassert>
#include <cstdlib>

#undef roundup
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

namespace lcc {
void *Allocator::allocate(unsigned long n, unsigned int a)
{
    Block *ap;

    assert(a < arena.size());
    assert(n > 0);
    ap = arena[a];
    n = roundup(n, sizeof(Align));
    while (n > static_cast<unsigned long>(ap->limit - ap->avail)) {
        if ((ap->next = freeblocks) != nullptr) {
            freeblocks = freeblocks->next;
            ap = ap->next;
        } else
            {
                unsigned long m = sizeof (Header) + n + roundup(10*1024, sizeof(Align));
                ap->next = (Block*)std::malloc(m);
                ap = ap->next;
                if (ap == nullptr) {
                    // error("insufficient memory\n");
                    exit(1);
                }
                ap->limit = (char *)ap + m;
            }
        ap->avail = (char *)((Header *)ap + 1);
        ap->next = nullptr;
        arena[a] = ap;

    }
    ap->avail += n;
    return ap->avail - n;
}

void Allocator::deallocate(unsigned int a)
{
    assert(a < arena.size());
    arena[a]->next = freeblocks;
    freeblocks = first[a].next;
    first[a].next = nullptr;
    arena[a] = &first[a];
}


void *Allocator::newarray(unsigned long m, unsigned long n, unsigned int a)
{
    return allocate(m * n, a);
}

}
