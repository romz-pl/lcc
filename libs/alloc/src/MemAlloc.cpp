//===- MemAlloc.cpp - Memory allocation functions -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <lcc/alloc/MemAlloc.h>
#include <lcc/alloc/error_handler.h>
#include <new>
#include <cstdlib>

namespace lcc {

__attribute__((returns_nonnull, __malloc__))
void *safe_malloc(std::size_t Sz)
{
  void *Result = std::malloc(Sz);
  if (Result == nullptr) {
    // It is implementation-defined whether allocation occurs if the space
    // requested is zero (ISO/IEC 9899:2018 7.22.3). Retry, requesting
    // non-zero, if the space requested was zero.
    if (Sz == 0)
      return safe_malloc(1);
    report_bad_alloc_error("Allocation failed");
  }
  return Result;
}

__attribute__((returns_nonnull, __malloc__))
void *safe_calloc(std::size_t Count, std::size_t Sz)
{
  void *Result = std::calloc(Count, Sz);
  if (Result == nullptr) {
    // It is implementation-defined whether allocation occurs if the space
    // requested is zero (ISO/IEC 9899:2018 7.22.3). Retry, requesting
    // non-zero, if the space requested was zero.
    if (Count == 0 || Sz == 0)
      return safe_malloc(1);
    report_bad_alloc_error("Allocation failed");
  }
  return Result;
}

__attribute__((returns_nonnull))
void *safe_realloc(void *Ptr, std::size_t Sz)
{
  void *Result = std::realloc(Ptr, Sz);
  if (Result == nullptr) {
    // It is implementation-defined whether allocation occurs if the space
    // requested is zero (ISO/IEC 9899:2018 7.22.3). Retry, requesting
    // non-zero, if the space requested was zero.
    if (Sz == 0)
      return safe_malloc(1);
    report_bad_alloc_error("Allocation failed");
  }
  return Result;
}

__attribute__((returns_nonnull, __malloc__))
void *allocate_buffer(std::size_t Size, std::size_t Alignment)
{
  return ::operator new(Size, std::align_val_t(Alignment));
}

void deallocate_buffer(void *Ptr, std::size_t Size, std::size_t Alignment)
{
  ::operator delete(Ptr, Size, std::align_val_t(Alignment));
}

}
