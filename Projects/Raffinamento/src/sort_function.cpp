#include "struct.hpp"
#include "Eigen/Eigen"
#include <iostream>
#include <fstream>
#include "vector"

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{
  TriangularMesh mesh;
// Main function to do heap sort

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int N = mesh.NumberCell2D;
vector<double> arr;

for (unsigned int i = 0; i < mesh.NumberCell2D; i++)
  arr.push_back(mesh.Cell2D[i].Area);

// Main function to do heap sort
void heapSort(vector<int> &arr, int N)
{
    // Build heap (rearrange array)
    for (int i = N / 2 - 1; i >= 0; i--)
        heapify(arr, N, i);

    // One by one extract an element
    // from heap
    for (int i = N - 1; i > 0; i--) {

        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
void heapify(vector<int> &arr, int N, int i)
{
    // Find largest among root,
    // left child and right child

    // Initialize largest as root
    int largest = i;

    // left = 2*i + 1
    int left = 2 * i + 1;

    // right = 2*i + 2
    int right = 2 * i + 2;

    // If left child is larger than root
    if (left < N && arr[left] > arr[largest])

        largest = left;

    // If right child is larger than largest
    // so far
    if (right < N && arr[right] > arr[largest])

        largest = right;

    // Swap and continue heapifying
    // if root is not largest
    // If largest is not root
    if (largest != i) {

        swap(&arr[i], &arr[largest]);

        // Recursively heapify the affected
        // sub-tree
        heapify(arr, N, largest);
    }
}

}
