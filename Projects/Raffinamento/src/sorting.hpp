#ifndef __SORTING_H
#define __SORTING_H

#include "iostream"
#include "list"
#include "Eigen/Eigen"
#include "map"
#include <utility>

using namespace std;
using namespace Eigen;
namespace SortLibrary {


  struct DecreasingInt
  {
    int value;
    DecreasingInt() = default;
    DecreasingInt(const int& value): value(value){}

  };

  // bool comparePairs(const std::pair<int, int>& a, const std::pair<int, int>& b) {
  // return a.first < b.first;
  inline ostream &operator<<(ostream &os, const DecreasingInt &obj)
  {
    os << obj.value;
    return os;
  }

  inline bool operator==(const std::pair<unsigned int, double>& obj1, const std::pair<unsigned int, double>& obj2)
  {
    return obj1.second == obj2.second;
  }

  inline bool operator!=(const std::pair<unsigned int, double>& obj1, const std::pair<unsigned int, double>& obj2)
  {
    return !(obj1.second == obj2.second);
  }

  inline bool operator< (const std::pair<unsigned int, double>& obj1, const std::pair<unsigned int, double>& obj2)
  {
    return obj1.second > obj2.second;
  }

  inline bool operator>(const std::pair<unsigned int, double>& obj1, const std::pair<unsigned int, double>& obj2)
  {
    return obj1.second < obj2.second;
  }



 template<typename T>
 void Heapify(vector<T>& sortedV, int n,int i)
 {
    // Initialize largest as root
    int largest = i;

    // left = 2*i + 1
    int l = 2 * i + 1;

    // right = 2*i + 2
    int r = 2 * i + 2;

    // If left child is larger than root
    if (l < n && sortedV[l] > sortedV[largest])
        largest = l;

    // If right child is larger than largest
    // so far
    if (r < n && sortedV[r] > sortedV[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(sortedV[i], sortedV[largest]);

        // Recursively heapify the affected
        // sub-tree
        Heapify(sortedV,n, largest);
    };
}

 template<typename T>
 vector<T> HeapSort(const vector<T>& v)
 {
    unsigned int n = v.size(); //dimensione vettore
    vector<T> sortedV = v;

    // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--)
            Heapify(sortedV, n, i);

    // One by one extract an element
    // from heap
    for (int i = n - 1; i > 0; i--) {

        // Move current root to end
        swap(sortedV[0], sortedV[i]);

        // call max heapify on the reduced heap
        Heapify(sortedV, i, 0);
        }
    return sortedV;
  }

}

#endif // __SORTING_H

