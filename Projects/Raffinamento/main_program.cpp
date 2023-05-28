#include "struct.hpp"
#include "functions.hpp"
// #include "refiningfunc.hpp"
#include "Eigen/Eigen"
#include <fstream>
#include "map"
#include "vector"

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
// <<<<<<< Updated upstream
  }

  vector<ArrIdArea> vettoreAree;

  for (unsigned int i=0; i< mesh.NumberCell2D; i++)
  { ArrIdArea pippo;
    pippo.idTr=mesh.Cell2DId[i] ;
    pippo.areaTr =Area(mesh,mesh.Cell2DId[i]);
    vettoreAree.push_back(pippo);
  };


  // Ordinamento del vettore in base al membro "area"
 sort(vettoreAree.begin(), vettoreAree.end(),greater<ArrIdArea>()); //qui dobbiamo fare il npstro

  cout << "Vettore dopo ordinamento:" << endl;
  for (const ArrIdArea& elemento : vettoreAree) {
  cout << "ID: " << elemento.idTr << ", Area: " << elemento.areaTr << endl;
  }


  // vedo se gira la bisezione per qualche triangolo a caso
  for (unsigned int i=0; 1<10; i++)
  {
    if (Bisezione(mesh,i))
        cout<<"ce l'ho fatta"<<endl;
  }

// =======
  }



