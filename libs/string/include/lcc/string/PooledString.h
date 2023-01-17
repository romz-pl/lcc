#pragma once

#include <lcc/string/StringPool.h>

#include <cassert>

namespace lcc {

/// Pointer to a pooled string.
class PooledString {
  friend class StringPool;
  friend struct std::hash<PooledString>;

public:
  PooledString() = default;
  PooledString(std::nullptr_t) {}
  PooledString(const PooledString &Other) : S(Other.S) {
    if (S)
      ++S->second;
  }

  PooledString &operator=(const PooledString &Other);

  PooledString(PooledString &&Other) : S(nullptr) {
    std::swap(S, Other.S);
  }

  PooledString &operator=(PooledString &&Other);

  ~PooledString() {
    if (S) {
      assert(S->second && "Releasing PooledString with zero ref count");
      --S->second;
    }
  }

  explicit operator bool() const { return S; }

  const std::string &operator*() const { return S->first; }

  friend bool operator==(const PooledString &LHS,
                         const PooledString &RHS) {
    return LHS.S == RHS.S;
  }

  friend bool operator!=(const PooledString &LHS,
                         const PooledString &RHS) {
    return !(LHS == RHS);
  }

  friend bool operator<(const PooledString &LHS,
                        const PooledString &RHS) {
    return LHS.S < RHS.S;
  }

private:
  using PoolEntry = StringPool::PoolMapEntry;
  using PoolEntryPtr = PoolEntry *;

  PooledString(StringPool::PoolMapEntry *S) : S(S) {
    if (S)
      ++S->second;
  }

  PoolEntryPtr S = nullptr;
};

}


namespace std {

// Make PooledStringPtrs hashable.
template <> struct hash<lcc::PooledString> {
  size_t operator()(const lcc::PooledString &A) const {
    return hash<lcc::PooledString::PoolEntryPtr>()(A.S);
  }
};

} // namespace std
