#include "struct.hpp"

#include "Eigen/Eigen"
#include <fstream>
#include "map"

using namespace std;
using namespace Eigen;
using namespace ProjectLibrary;

int main()
{
  TriangularMesh mesh;

    ImportCell0Ds(mesh);
    ImportCell1Ds(mesh);

  if(!ImportCell2Ds(mesh))
  {
    return 1;
  }
  else
  {
    cout << "Mappa Adiacenze: " << endl;
    for(auto it = mesh.Adjacency.begin(); it != mesh.Adjacency.end(); it++) // per tutti i lati
    {
      cout << "key:\t" << it -> first << "\t values:";
      for(const unsigned int id : it -> second)
        cout << "\t" << id;

      cout << endl;
    }
  }


  for (unsigned int i=0; i< mesh.NumberCell2D; i++)
  {double area= Area(mesh,mesh.Cell2DId[i]);
    cout <<mesh.Cell2DId[i]<<':'<< area << endl;
  };

};

