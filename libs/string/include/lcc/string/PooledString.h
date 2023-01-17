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

private:
  PooledString(StringPool::PoolMapEntry *S) : S(S) {
    if (S)
      ++S->second;
  }

  StringPool::PoolMapEntry* S = nullptr;
};

}

