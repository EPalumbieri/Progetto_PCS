#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>

#include "empty_class.hpp"

using namespace testing;

TEST(TestEmpty, TestEmpty)
{
  ProjectLibrary::Empty empty;
  ASSERT_NO_THROW(empty.Show());
}

#endif // __TEST_EMPTY_H
