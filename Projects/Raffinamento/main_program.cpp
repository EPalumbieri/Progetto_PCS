#include "struct.hpp"
#include "sorting.hpp"
#include "Eigen/Eigen"
#include <fstream>
#include "map"
#include "vector"
#include <utility>

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

  // creo un vettore di pair
  vector<pair<unsigned int, double>> idArea;
  for (unsigned int i = 0; i < mesh.NumberCell2D; i++)
  {
      idArea.push_back(std::pair<unsigned int, double>(i,mesh.Cell2D[i].Area));
      cout <<"idT:   "<< idArea[i].first << "Area: "<< idArea[i].second <<endl;
  }

  // ordino il vettore in base alle aree
  vector<pair<unsigned int, double>> areeOrdinate = SortLibrary::HeapSort(idArea);
  for (unsigned int i = 0; i < mesh.NumberCell2D; i++)
  {
    cout<<"triangoli ordinati:\t" << areeOrdinate[i].first<<"\t Area triangolo ordi: "<<areeOrdinate[i].second<<endl;
  }

  // applico la bisezione fino a una certa tolleranza (da sistemare in relativa)
  int teta = 45;
  //teta = teta % 100 * mesh.NumberCell2D;
  double areaTol = areeOrdinate[teta].second; //tolleranza sull'area
  cout << "Area tol:"<<areaTol<<endl;
  areeOrdinate.erase(areeOrdinate.begin() + teta, areeOrdinate.end()); //elimino i pair da teta in poi
  for (unsigned int i = 0; i < areeOrdinate.size(); i++)
  {
    // cout<<"triangoli ordinati restanti:\t" << areeOrdinate[i].first<<"\t Area triangolo ordi: "<<areeOrdinate[i].second<<endl;
  }
//  for ()
//  {
//      //areeOrdinate[0].erase
//    mesh.Bisezione(areeOrdinate[0].first);
//  }



//  for (unsigned int i = 0; i < mesh.NumberCell2D; i++)
//  {
//      cout << "id triangolo "<<mesh.Cell2D[i].idT <<"area "<< mesh.Cell2D[i].Area << endl;
//  }

  cout << "Mappa Adiacenze: " << endl;
  for(auto it = mesh.Adjacency.begin(); it != mesh.Adjacency.end(); it++) // per tutti i lati
  {
  cout << "id lato:\t" << it -> first << "\t id triangol* adiacent*:";
  for(const unsigned int id : it -> second)
      cout << "\t" << id;

  cout << endl;
  }

}



