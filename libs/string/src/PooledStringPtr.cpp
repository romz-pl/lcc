#include <lcc/string/PooledStringPtr.h>

namespace lcc {

PooledStringPtr &PooledStringPtr::operator=(const PooledStringPtr &Other)
{
  if (S) {
    assert(S->second && "Releasing PooledStringPtr with zero ref count");
    --S->second;
  }
  S = Other.S;
  if (S)
    ++S->second;
  return *this;
}

PooledStringPtr &PooledStringPtr::operator=(PooledStringPtr &&Other) {
  if (S) {
    assert(S->second && "Releasing PooledStringPtr with zero ref count");
    --S->second;
  }
  S = nullptr;
  std::swap(S, Other.S);
  return *this;
}

}
