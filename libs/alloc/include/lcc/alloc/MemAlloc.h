#pragma once

//===- MemAlloc.h - Memory allocation functions -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
///
/// This file defines counterparts of C library allocation functions defined in
/// the namespace 'std'. The new allocation functions crash on allocation
/// failure instead of returning null pointer.
///
//===----------------------------------------------------------------------===//

#include <cstddef>

namespace lcc {

__attribute__((returns_nonnull, __malloc__)) void *safe_malloc(std::size_t Sz);

__attribute__((returns_nonnull, __malloc__)) void *safe_calloc(std::size_t Count, std::size_t Sz);

__attribute__((returns_nonnull)) void *safe_realloc(void *Ptr, std::size_t Sz);

/// Allocate a buffer of memory with the given size and alignment.
///
/// When the compiler supports aligned operator new, this will use it to to
/// handle even over-aligned allocations.
///
/// However, this doesn't make any attempt to leverage the fancier techniques
/// like posix_memalign due to portability. It is mostly intended to allow
/// compatibility with platforms that, after aligned allocation was added, use
/// reduced default alignment.
__attribute__((returns_nonnull, __malloc__)) void *
allocate_buffer(std::size_t Size, std::size_t Alignment);

/// Deallocate a buffer of memory with the given size and alignment.
///
/// If supported, this will used the sized delete operator. Also if supported,
/// this will pass the alignment to the delete operator.
///
/// The pointer must have been allocated with the corresponding new operator,
/// most likely using the above helper.
void deallocate_buffer(void *Ptr, std::size_t Size, std::size_t Alignment);

} // namespace lcc

