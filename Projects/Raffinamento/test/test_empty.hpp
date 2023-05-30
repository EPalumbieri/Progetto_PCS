#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>
#include "struct.hpp"
#include "Eigen/Eigen"

using namespace testing;
using namespace Eigen;
namespace ProjectLibrary
{
//TEST(TestEmpty, TestEmpty)
//{
//  Empty empty;
//  ASSERT_NO_THROW(empty.Show());
//}

TEST(testAria, testValoreCorretto)
{
  Vector2d v1;
  v1 << 0,0;

  Vector2d v2;
  v2 << 0,1;

  Vector2d v3;
  v3 << 1,0;

  Punto P1= Punto(0,v1);
  Punto P2= Punto(1,v2);
  Punto P3= Punto(2,v3);

  double area = TriangularMesh::Area(P1,P2,P3);

  EXPECT_EQ(area, 0.5);
}
}
#endif // __TEST_EMPTY_H
