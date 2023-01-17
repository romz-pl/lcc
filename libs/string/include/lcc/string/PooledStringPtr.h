#pragma once

#include <lcc/string/StringPool.h>

#include <cassert>

namespace lcc {

/// Pointer to a pooled string.
class PooledStringPtr {
  friend class StringPool;
  friend struct std::hash<PooledStringPtr>;

public:
  PooledStringPtr() = default;
  PooledStringPtr(std::nullptr_t) {}
  PooledStringPtr(const PooledStringPtr &Other) : S(Other.S) {
    if (S)
      ++S->second;
  }

  PooledStringPtr &operator=(const PooledStringPtr &Other);

  PooledStringPtr(PooledStringPtr &&Other) : S(nullptr) {
    std::swap(S, Other.S);
  }

  PooledStringPtr &operator=(PooledStringPtr &&Other);

  ~PooledStringPtr() {
    if (S) {
      assert(S->second && "Releasing PooledStringPtr with zero ref count");
      --S->second;
    }
  }

  explicit operator bool() const { return S; }

  const std::string &operator*() const { return S->first; }

  friend bool operator==(const PooledStringPtr &LHS,
                         const PooledStringPtr &RHS) {
    return LHS.S == RHS.S;
  }

  friend bool operator!=(const PooledStringPtr &LHS,
                         const PooledStringPtr &RHS) {
    return !(LHS == RHS);
  }

  friend bool operator<(const PooledStringPtr &LHS,
                        const PooledStringPtr &RHS) {
    return LHS.S < RHS.S;
  }

private:
  using PoolEntry = StringPool::PoolMapEntry;
  using PoolEntryPtr = PoolEntry *;

  PooledStringPtr(StringPool::PoolMapEntry *S) : S(S) {
    if (S)
      ++S->second;
  }

  PoolEntryPtr S = nullptr;
};

}


namespace std {

// Make PooledStringPtrs hashable.
template <> struct hash<lcc::PooledStringPtr> {
  size_t operator()(const lcc::PooledStringPtr &A) const {
    return hash<lcc::PooledStringPtr::PoolEntryPtr>()(A.S);
  }
};

} // namespace std
