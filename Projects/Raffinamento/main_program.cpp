#include "struct.hpp"
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
  mesh.ImportCell0Ds();
  mesh.ImportCell1Ds();

  if(!mesh.ImportCell2Ds())
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
  // Ordinamento del vettore in base al membro "area"
    sort(mesh.Cell2D.begin(), mesh.Cell2D.end(), []( const Triangolo & lhs , const Triangolo & rhs) {return lhs.Area > rhs.Area;}); //qui dobbiamo fare il npstro

 for (unsigned int i = 0; i < mesh.NumberCell2D; i++)
 {
     cout << "id triangolo "<<mesh.Cell2D[i].idT <<"area "<< mesh.Cell2D[i].Area << endl;
 }

   // vedo se gira la bisezione per le prime 10 aree più grandi

//  for(unsigned int i=0; i<10; i++)
//  {  Bisezione(mesh,vettoreAree[i].idTr); };

// mesh.Bisezione(74);
 // ALGORITMO TRIANGOLO//

  cout <<endl<< "Mappa Adiacenze: " << endl;
  for(auto it = mesh.Adjacency.begin(); it != mesh.Adjacency.end(); it++) // per tutti i lati
  {
  cout << "id lato:\t" << it -> first << "\t id triangol* adiacent*:";
  for(const unsigned int id : it -> second)
      cout << "\t" << id;

  cout << endl;
  }

}



