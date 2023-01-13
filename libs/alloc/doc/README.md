Cited from the book Dave Hanson and Christopher W. Fraser 
"A Retargetable C Compiler: Design and Implementation" 
(Addison-Wesley, 1995, ISBN 0-8053-1670-1)

Storage Management
==================

Complex programs allocate memory dynamically, and **lcc** is no 
exception. In C, **malloc** allocates memory and **free** releases it. **lcc** could use
**malloc** and **free**, but there is a superior alternative that is more efficient,
easier to program, and better suited for use in compilers, and it is easily
understood in isolation.

Calling **malloc** incurs the obligation of a subsequent call to **free**. The
cost of this explicit deallocation can be significant. More important, it's
easy to forget it or, worse, deallocate something that's still referenced.
In some applications, most deallocations occur at the same time. 
Window systems are an example. Space for scroll bars, buttons, etc., are
allocated when the window is created and deallocated when the window
is destroyed. A compiler, like **lcc**, is another example. **lcc** allocates
memory in response to declarations, statements, and expressions as they
occur within functions, but it deallocates memory only at the ends of
statements and functions.

Most implementations of **malloc** use memory-management algorithms
that are necessarily based on the sizes of objects. Algorithms based on
**object lifetimes** are more efficient — if all of the deallocations can be
done at once. Indeed, stacklike allocation would be most efficient, but
it can be used only if object lifetimes are nested, which is generally not
the case in compilers and many other applications.

This chapter describes **lcc**'s storage management scheme, which is
based on object lifetimes. In this scheme, allocation is more efficient than
**malloc**, and the cost of deallocation is negligible. But the real benefit
is that this scheme simplifies the code. Allocation is so cheap that it
encourages simple applicative algorithms in place of more space-efficient
but complex ones. And allocation incurs no deallocation obligation, so
deallocation can't be forgotten.


Memory Management Interface
---------------------------

Memory is allocated from **arenas**, and entire arenas are deallocated at
once. Objects with the same lifetimes are allocated from the same arena.
The arena is identified by an arena identifier — a nonnegative integer —
when space from it is allocated or when all of it is deallocated.

The space immediately following the arena structure up to the location
given by the **limit** field is the allocable portion of the block, **avail** points
to the first free location within the block; space below **availl** has been
allocated and space beginning at **availl** and up to **limit** is available. The
**next** field points to the next block in the list. The implementation keeps
an arena pointer, which points to the first block in the list with available
space. Blocks are added to the list dynamically during allocation, as
detailed below.

There are three arenas known by the integers 0-2; clients usually
equate symbolic names to these arena identifiers for use in calls to
**allocate**, **deallocate**, and **newarray**.


Further Reading
---------------

Storage management is a busy area of research; Section 2.5 in Knuth
(1973a) [1] is the definitive reference. There is a long list of techniques that
are designed both for general-purpose use and for specific application
areas, including the design described in this chapter (Hanson 1990) [2]. A
competitive alternative is "quick fit" (Weinstock and Wulf 1988) [3]. Quick-fit
allocators maintain N free lists for the N block sizes requested most 
frequently. Usually, these sizes are small and contiguous; e.g., 8-128 bytes
in multiples of eight bytes. Allocation is easy and fast: Take the first
block from the appropriate free list. A block is deallocated by adding it
to the head of its list. Requests for sizes other than one of the N favored
sizes are handled with other algorithms, such as first fit (Knuth 1973a) [1].

One of the advantages of **lcc**'s arena-based algorithm is that 
allocations don't have to be paired with individual deallocations; a single 
deallocation frees the memory acquired by many allocations, which simplifies
programming. Garbage collection takes this advantage one step further.
A garbage collector periodically finds all of the storage that is in use and
frees the rest. It does so by following all of the accessible pointers in
the program. Appel (1991) [4] and Wilson (1994) [5] survey garbage-collection
algorithms. Garbage collectors usually need help from the programming
language, its compiler, and its run-time system in order to locate the 
accessible memory, but there are algorithms that can cope without such
help. Boehm and Weiser (1988) [6] describe one such algorithm for C. It
takes a conservative approach: Anything that looks like a pointer is taken
to be one. As a result, the collector identifies some inaccessible memory
as accessible and thus busy, but that's better than making the opposite
decision.

Bibliography
------------

[1] Knuth, D. E. 1973a, The Art of Computer Programming: Volume 1, 
Fundamental Algorithms (second edition). Reading, MA: Addison Wesley.

[2] Hanson, D. R. 1990. Fast allocation and deallocation of memory based on object
lifetimes. Software—Practice and Experience 20(1), 5-12.

[3] Weinstock, C. B., and W. A. Wulf. 1988. Quick fit: An efficient algorithm
for heap storage management. SIGPLANNotices 23(10), 141-148.

[4] Appel, A. W. 1991. Garbage collection. In P. Lee, Ed., Topics in Advanced
Language Implementation Techniques, 89-100. Cambridge, MA: MIT Press.

[5] Wilson, P. R. 1994. Uniprocessor garbage collection techniques. International 
Workshop on Memory Management, IWMM 1992: Memory Management, 1–42

[6] Boehni, H.-J., andM. Weiser. 1988. Garbage collection in an uncooperative
environment. Software—Practice and Experience 18(9), 807-820.
