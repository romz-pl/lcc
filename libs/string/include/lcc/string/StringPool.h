#pragma once

//===------- string_pool.h - Thread-safe pool for strings -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Contains a thread-safe string pool. Strings are ref-counted, but not
// automatically deallocated. Unused entries can be cleared by calling
// StringPool::clearDeadEntries.
//
//===----------------------------------------------------------------------===//

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

namespace lcc {

class PooledString;

/// String pool for strings names used by the ORC runtime.
class StringPool {
  friend class PooledString;

public:
  /// Destroy a StringPool.
  ~StringPool();

  /// Create a string pointer from the given string.
  PooledString intern(std::string S);

  /// Remove from the pool any entries that are no longer referenced.
  void clearDeadEntries();

  /// Returns true if the pool is empty.
  bool empty() const;

private:
  using RefCountType = std::atomic<size_t>;
  using PoolMap = std::unordered_map<std::string, RefCountType>;
  using PoolMapEntry = PoolMap::value_type;
  mutable std::mutex PoolMutex;
  PoolMap Pool;
};

} // end namespace lcc



