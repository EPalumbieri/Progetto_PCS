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

  inline bool operator==(const pair<unsigned int, double>& obj1, const pair<unsigned int, double>& obj2)
  {
    return obj1.second == obj2.second;
  }

  inline bool operator!=(const pair<unsigned int, double>& obj1, const pair<unsigned int, double>& obj2)
  {
    return !(obj1.second == obj2.second);
  }

  inline bool operator< (const pair<unsigned int, double>& obj1, const pair<unsigned int, double>& obj2)
  {
    return obj1.second > obj2.second;
  }

  inline bool operator>(const pair<unsigned int, double>& obj1, const pair<unsigned int, double>& obj2)
  {
    return obj1.second < obj2.second;
  }

 template<typename T>
 void Heapify(vector<T>& sortedV, int n,int i)
 {
    // Inizializzo il maggiore come la radice
    int largest = i;

    // left = 2*i + 1
    int l = 2 * i + 1;

    // right = 2*i + 2
    int r = 2 * i + 2;

    // Se left è maggiore della radice
    if (l < n && sortedV[l] > sortedV[largest])
        largest = l;

    // Se right è maggiore della radice
    if (r < n && sortedV[r] > sortedV[largest])
        largest = r;

    // Se il maggiore non è la radice
    if (largest != i) {
        swap(sortedV[i], sortedV[largest]);

        // Applica Heapify ricorsivamente al sottoalbero
        Heapify(sortedV,n, largest);
    };
}

 template<typename T>
 void CreaHeap(vector<T>& sortedV, int n)
 {
     // Costruisco l'heap
     for (int i = n / 2 - 1; i >= 0; i--)
         Heapify(sortedV, n, i);
 }

 template<typename T>
 vector<T> HeapSort(const vector<T>& v)
 {
    unsigned int n = v.size();
    vector<T> sortedV = v;

    CreaHeap(sortedV, n);

    // Uno alla volta estraggo gli elementi dall'heap
    for (int i = n - 1; i > 0; i--) {

        // Sposto la radice corrente in fondo
        swap(sortedV[0], sortedV[i]);

        // Chiamo Heapify all'heap ridotto
        Heapify(sortedV, i, 0);
        }
    return sortedV;
  }

}

#endif // __SORTING_H

