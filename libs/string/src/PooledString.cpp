#include <lcc/string/PooledString.h>

namespace lcc {

PooledString &PooledString::operator=(const PooledString &Other)
{
  if (S) {
    assert(S->second && "Releasing PooledString with zero ref count");
    --S->second;
  }
  S = Other.S;
  if (S)
    ++S->second;
  return *this;
}

PooledString &PooledString::operator=(PooledString &&Other) {
  if (S) {
    assert(S->second && "Releasing PooledString with zero ref count");
    --S->second;
  }
  S = nullptr;
  std::swap(S, Other.S);
  return *this;
}

}
