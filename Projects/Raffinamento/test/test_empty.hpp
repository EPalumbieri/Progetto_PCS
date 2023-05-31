#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>
#include "struct.hpp"
#include "sorting.hpp"
#include "Eigen/Eigen"

using namespace testing;
using namespace Eigen;
using namespace SortLibrary;
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
TEST(TestSorting, TestHeapSortIntBuiltIn)
{
  vector<int> v = {9, 13, 5, 10, 2, 7, 9, 4, 6, 12};
  vector<int> ssV = HeapSort<int>(v);
  vector<int> sortedV = {2, 4, 5, 6, 7, 9, 9, 10, 12 ,13};
  EXPECT_EQ(ssV, sortedV);
}

TEST(TestSorting, TestHeapSortVectorPair)
{
  vector<pair<unsigned int,double>> v;
  for(int i=0;i<10;i++){
      v.push_back(pair<unsigned int,double> (i+1,i));
  }
  vector<pair<unsigned int,double>> ssV = SortLibrary::HeapSort<pair<unsigned int,double>>(v);

  vector<pair<unsigned int,double>> sortedV;
  for(int i=9;i>=0;i--){
      sortedV.push_back(pair<unsigned int,double> (i+1,i));
  }
  EXPECT_EQ(ssV, sortedV);
}

//TEST(TestConti,TestLunghezzaLato)
//{

//    lunghezza=LunghezzaLato();

//    EXPECT_EQ(lunghezza, lunghezzaV);

//? devo dare in input un id--> come lo lego alle coordinate?
//}
}
#endif // __TEST_EMPTY_H













