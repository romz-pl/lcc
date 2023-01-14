# Strings

Cited from the book Dave Hanson and Christopher W. Fraser 
"A Retargetable C Compiler: Design and Implementation" 
(Addison-Wesley, 1995, ISBN 0-8053-1670-1)

Strings are created for identifiers, constants, registers, and so on. Strings
are compared often; for example, when a symbol table is searched for
an identifier.

The most common uses of strings are provided by the functions 
exported by **string** library:
```c++
char *string (char *str);
char *stringn (char *str, int len);
char *stringd (int n);
```

Each of these functions returns a pointer to a permanently allocated
string. The function **string** makes a copy of the null-terminated string **str**, 
the function **stringn**
makes a copy of the **len** bytes in **str**, and the function **stringd** converts **n** to its
decimal representation and returns that string.


These functions save exactly one copy of each distinct string, so two
strings returned by these functions can be compared for equality by 
comparing their addresses. These semantics simplify comparisons and save
space, and **stringn** can handle strings with embedded null characters.


# String interning

From Wikipedia: https://en.wikipedia.org/wiki/String_interning

In computer science, **string interning** is a method of storing only one copy of each distinct string value, 
which must be immutable. Interning strings makes some string processing tasks more time-efficient 
or space-efficient at the cost of requiring more time when the string is created or interned. 
The distinct values are stored in a **string intern pool**. 

The single copy of each string is called its **intern** and is typically looked up by a method of the string class, 
for example String.intern() in Java. All compile-time constant strings in Java are automatically 
interned using this method.

## Motivation

**String interning** speeds up string comparisons, which are sometimes a performance bottleneck 
in applications (such as compilers and dynamic programming language runtimes) that rely heavily 
on associative arrays with string keys to look up the attributes and methods of an object. 
Without interning, comparing two distinct strings may involve examining every character of both.
This is slow for several reasons: it is inherently *O(n)* in the length of the strings; it typically 
requires reads from several regions of memory, which take time; and the reads fill up 
the processor cache, meaning there is less cache available for other needs. With interned strings, a 
simple object identity test suffices after the original intern operation; this is typically implemented 
as a pointer equality test, normally just a single machine instruction with no memory reference at all.

**String interning** also reduces memory usage if there are many instances of the same string value; 
for instance, it is read from a network or from storage. Such strings may include magic 
numbers or network protocol information. For example, XML parsers may intern names of tags and 
attributes to save memory. Network transfer of objects over Java RMI serialization object streams 
can transfer strings that are interned more efficiently, as the String object's handle is used in 
place of duplicate objects upon serialization.


## Boost Flyweight Library
From https://www.boost.org/doc/libs/1_81_0/libs/flyweight/doc/index.html


Flyweights are small-sized handle classes granting constant access to shared common data, thus 
allowing for the management of large amounts of entities within reasonable memory limits. 
**Boost.Flyweight** makes it easy to use this common programming idiom by providing the class 
template `flyweight<T>`, which acts as a drop-in replacement for `const T`.


### Rationale

Consider an application that has to manage large quantities of objects of moderate size, potentially 
requiring more memory than reasonably available. When these objects are immutable, i.e. they do not modify 
its internal state except maybe for reattaching to a new set of state data, and some additional conditions 
are met, a very convenient optimization technique known as the flyweight pattern can be introduced. 

Let us say there are `N` different objects living at a given time inside the application, globally 
taking `M` different values. If `N` is much greater than `M`, that is, there are many equivalent objects, 
we can eliminate the implicit redundancy by replacing the original objects with handle classes which refer 
to a common repository of shared value objects, as depicted in the figure. The handle objects or flyweights, 
which act as proxies for the actual values, typically occupy the size of a mere pointer. The larger the value 
classes, and the greater the `N/M` ratio, the more significant the memory savings achieved by this tecnhique. 
The classical example of application of the flyweight idiom is that of a word processor: each letter in the 
document carries a large wealth of information, such as its Unicode identifier, font, size, typesetting effects, 
etc., but given that the degree of letter repetition in a document is extremely high, implementing those letters as 
flyweight classes allows us to easily handle documents ranging in the hundreds of thousands of characters. 


## StringPool.h from LLVM project

From file https://opensource.apple.com/source/llvmCore/llvmCore-3418.0.80/include/llvm/Support/StringPool.h.auto.html

This file declares an interned string pool, which helps reduce the cost of
strings by using the same storage for identical strings.

To intern a string:
```c++
StringPool Pool;
PooledStringPtr Str = Pool.intern("wakka wakka");
```
To use the value of an interned string, use operator bool and operator*:
```c++
if (Str)
     cerr << "the string is" << *Str << "\n";
```
Pooled strings are immutable, but you can change a `PooledStringPtr` to point
to another instance. So that interned strings can eventually be freed,
strings in the string pool are reference-counted (automatically).

```c++
#ifndef LLVM_SUPPORT_STRINGPOOL_H
#define LLVM_SUPPORT_STRINGPOOL_H

#include "llvm/ADT/StringMap.h"
#include <new>
#include <cassert>

namespace llvm {

  class PooledStringPtr;

  /// StringPool - An interned string pool. Use the intern method to add a
  /// string. Strings are removed automatically as PooledStringPtrs are
  /// destroyed.
  class StringPool {
    /// PooledString - This is the value of an entry in the pool's interning
    /// table.
    struct PooledString {
      StringPool *Pool;  ///< So the string can remove itself.
      unsigned Refcount; ///< Number of referencing PooledStringPtrs.

    public:
      PooledString() : Pool(0), Refcount(0) { }
    };

    friend class PooledStringPtr;

    typedef StringMap<PooledString> table_t;
    typedef StringMapEntry<PooledString> entry_t;
    table_t InternTable;

  public:
    StringPool();
    ~StringPool();

    /// intern - Adds a string to the pool and returns a reference-counted
    /// pointer to it. No additional memory is allocated if the string already
    /// exists in the pool.
    PooledStringPtr intern(StringRef Str);

    /// empty - Checks whether the pool is empty. Returns true if so.
    ///
    inline bool empty() const { return InternTable.empty(); }
  };

  /// PooledStringPtr - A pointer to an interned string. Use operator bool to
  /// test whether the pointer is valid, and operator * to get the string if so.
  /// This is a lightweight value class with storage requirements equivalent to
  /// a single pointer, but it does have reference-counting overhead when
  /// copied.
  class PooledStringPtr {
    typedef StringPool::entry_t entry_t;
    entry_t *S;

  public:
    PooledStringPtr() : S(0) {}

    explicit PooledStringPtr(entry_t *E) : S(E) {
      if (S) ++S->getValue().Refcount;
    }

    PooledStringPtr(const PooledStringPtr &That) : S(That.S) {
      if (S) ++S->getValue().Refcount;
    }

    PooledStringPtr &operator=(const PooledStringPtr &That) {
      if (S != That.S) {
        clear();
        S = That.S;
        if (S) ++S->getValue().Refcount;
      }
      return *this;
    }

    void clear() {
      if (!S)
        return;
      if (--S->getValue().Refcount == 0) {
        S->getValue().Pool->InternTable.remove(S);
        S->Destroy();
      }
      S = 0;
    }

    ~PooledStringPtr() { clear(); }

    inline const char *begin() const {
      assert(*this && "Attempt to dereference empty PooledStringPtr!");
      return S->getKeyData();
    }

    inline const char *end() const {
      assert(*this && "Attempt to dereference empty PooledStringPtr!");
      return S->getKeyData() + S->getKeyLength();
    }

    inline unsigned size() const {
      assert(*this && "Attempt to dereference empty PooledStringPtr!");
      return S->getKeyLength();
    }

    inline const char *operator*() const { return begin(); }
    inline operator bool() const { return S != 0; }

    inline bool operator==(const PooledStringPtr &That) { return S == That.S; }
    inline bool operator!=(const PooledStringPtr &That) { return S != That.S; }
  };

} // End llvm namespace

#endif
```


## Boost.Pool
From https://www.boost.org/doc/libs/1_81_0/libs/pool/doc/html/index.html

### What is Pool?

Pool allocation is a memory allocation scheme that is very fast, but limited in its usage. For more 
information on pool allocation (also called simple segregated storage, see concepts concepts and Simple Segregated Storage).

### Why should I use Pool?

Using Pools gives you more control over how memory is used in your program. For example, you could have a 
situation where you want to allocate a bunch of small objects at one point, and then reach a point in your 
program where none of them are needed any more. Using pool interfaces, you can choose to run their destructors 
or just drop them off into oblivion; the pool interface will guarantee that there are no system memory leaks.

### When should I use Pool?

Pools are generally used when there is a lot of allocation and deallocation of small objects. Another 
common usage is the situation above, where many objects may be dropped out of memory.

In general, use Pools when you need a more efficient way to do unusual memory control.

### Which pool allocator should I use?

pool_allocator is a more general-purpose solution, geared towards efficiently servicing requests 
for any number of contiguous chunks.

fast_pool_allocator is also a general-purpose solution but is geared towards efficiently servicing requests 
for one chunk at a time; it will work for contiguous chunks, but not as well as pool_allocator.

If you are seriously concerned about performance, use fast_pool_allocator when dealing with containers 
such as `std::list`, and use pool_allocator when dealing with containers such as `std::vector`. 


## Library https://github.com/chriso/intern

Fast, immutable string interning for C.

### What is this?

- A way of assigning a unique integer ID to each unique string, without collisions
- Two-way lookup: ID => string, string => ID
- Each string is stored only once in memory
- Optional inlining of unsigned integer strings
- Very low fragmentation via a custom block allocator
- Minimal overhead per string: currently ~40 bytes, which could be lower at the cost of additional fragmentation
- Fast: intern many millions of strings per second
- String repository optimization based on frequency analysis (improve locality)
- Support for snapshots (restore to a previous state)


## libstringintern
From https://github.com/RipcordSoftware/libstringintern

A thread safe lock free C++ library for interning strings to save heap space. Smaller heaps generally
mean faster applications due to cache locality and less swapping to disk. The other upside 
is you can store more stuff in RAM.

### Use case

You have an in-memory database with 100 million customer records with associated address details. 
You therefore have 100 million state and country strings associated with those records. 
That will use a lot of memory.

When strings are interned you only ever have one instance of the string plus a reference instance per 
record which is 4 bytes in size.

### How it works

- Interned strings are stored in pages, grouped by string size, a bit like tcmalloc.
- Pages are 2MB in size to take advantage of THP.
- Each string is hashed using XX64 with the low order bytes used as an index into the page.
- If the index refers to a location in the page that is free the string and hash are committed to the page and the caller gets a reference to the string.
- If the index is already taken then we compare the hashes and if they match we return a reference to the entry.
- If the hashes did not match then allocate a new page and add the string and hash returning the string reference.
- The host application turns the reference into a real pointer to use the string, a bit like `std::weak_ptr`.
- References are 32bit values made up from `PAGE(16):INDEX(16)`. On 64bit systems these use half the storage space of a pointer.
- Interned strings are immutable.


## StringPool
From https://github.com/danielkrupinski/StringPool

A performant and memory efficient storage for immutable strings with C++17. Supports all standard 
char types: `char`, `wchar_t`, `char16_t`, `char32_t` and C++20's `char8_t`.

### Motivation

Standard C++ string classes - `std::string`, `std::wstring `etc. - aren't very efficient when it comes to memory 
usage and allocations. Due to small string optimization a lot of space can be wasted when storing huge amounts 
of long strings, that don't fit the capacity of the small string buffer. A common allocation strategy that
`std::basic_string` uses (doubling the capacity when extending the storage) can lead to almost `50%` waste of memory 
unless `std::basic_string::shrink_to_fit()` is called.

**StringPool** was created to provide a way of storing strings that don't change throughout program 
execution without excessive memory usage. Furthermore, it combats memory fragmentation by storing strings together, in blocks.

### StringPool doesn't do string interning

**StringPool** doesn't perform any string comparisons, neither it differentiates between two strings - each call 
to `StringPool<>::add()` gives you a brand new view of the string.

