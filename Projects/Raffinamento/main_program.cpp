#include "struct.hpp"
#include "functions.hpp"
<<<<<<< HEAD
#include "longest_edge"
=======

>>>>>>> 9326a2ee5cc08ebd0d45bf3bd5a3af452361f90e
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
//  std::pair<unsigned int, double>
//  sort(pippo.begin(),pippo.end())

  vector<ArrIdArea> vettoreAree;

  for (unsigned int i=0; i< mesh.NumberCell2D; i++)
  { ArrIdArea pippo;
    pippo.idTr=mesh.Cell2DId[i] ;
    pippo.areaTr =Area(mesh,mesh.Cell2DId[i]);
    vettoreAree.push_back(pippo);
  };


  cout << "Vettore prima dell'ordinamento:" << endl;
  for (const ArrIdArea& elemento : vettoreAree) {
  cout << "ID: " << elemento.idTr << ", Area: " << elemento.areaTr << endl;
  }
 cout << "nnnnn" << endl;
  // Ordinamento del vettore in base al membro "area"
 sort(vettoreAree.begin(), vettoreAree.end(),greater<ArrIdArea>());

  cout << "Vettore dopo ordinamento:" << endl;
  for (const ArrIdArea& elemento : vettoreAree) {
  cout << "ID: " << elemento.idTr << ", Area: " << elemento.areaTr << endl;
  }




};

