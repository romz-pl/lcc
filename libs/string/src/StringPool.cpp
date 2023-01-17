#include <lcc/string/StringPool.h>

#include <lcc/string/PooledStringPtr.h>

namespace lcc {

StringPool::~StringPool() {
#ifndef NDEBUG
  clearDeadEntries();
  assert(Pool.empty() && "Dangling references at pool destruction time");
#endif // NDEBUG
}

PooledStringPtr StringPool::intern(std::string S)
{
  std::lock_guard<std::mutex> Lock(PoolMutex);
  PoolMap::iterator I;
  bool Added;
  std::tie(I, Added) = Pool.try_emplace(std::move(S), 0);
  return PooledStringPtr(&*I);
}

void StringPool::clearDeadEntries()
{
  std::lock_guard<std::mutex> Lock(PoolMutex);
  for (auto I = Pool.begin(), E = Pool.end(); I != E;) {
    auto Tmp = I++;
    if (Tmp->second == 0)
      Pool.erase(Tmp);
  }
}

bool StringPool::empty() const
{
  std::lock_guard<std::mutex> Lock(PoolMutex);
  return Pool.empty();
}

}
