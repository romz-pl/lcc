//===---------- string_pool_test.cpp - Unit tests for StringPool ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <lcc/string/PooledString.h>
#include <lcc/string/StringPool.h>
#include "gtest/gtest.h"

namespace {

TEST(StringPool, UniquingAndComparisons)
{
  lcc::StringPool SP;
  const std::string hello{"hello"};
  lcc::PooledString P1 = SP.intern(hello);
  lcc::PooledString P2 = SP.intern(hello);
  lcc::PooledString P3 = SP.intern("goodbye");

  EXPECT_EQ(P1, P2) << "Failed to unique entries";
  EXPECT_NE(P1, P3) << "Unequal pooled strings comparing equal";
}

TEST(StringPool, Dereference)
{
  const std::string Txt{"foo"};
  lcc::StringPool SP;
  lcc::PooledString Foo = SP.intern(Txt);
  EXPECT_EQ(*Foo, Txt) << "Equality on dereferenced string failed";
}

TEST(StringPool, ClearDeadEntries)
{
  lcc::StringPool SP;
  {
    lcc::PooledString P1 = SP.intern("s1");
    SP.clearDeadEntries();
    EXPECT_FALSE(SP.empty()) << "\"s1\" entry in pool should still be retained";
  }
  EXPECT_FALSE(SP.empty()) << "pool should not be empty";
  SP.clearDeadEntries();
  EXPECT_TRUE(SP.empty()) << "pool should be empty";
}

TEST(StringPool, NullPtr)
{
  // Make sure that we can default construct and then destroy a null
  // PooledString.
  lcc::PooledString Null;
}

} // end anonymous namespace
