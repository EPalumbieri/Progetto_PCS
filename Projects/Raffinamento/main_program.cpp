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
  }

  // opzione 1: ordino il vettore in base alle aree
  vector<pair<unsigned int, double>> areeOrdinate = SortLibrary::HeapSort(idArea);

  // o commenti questo
//  // applico la bisezione fino a una certa tolleranza
//  double teta = 0.9;
//  unsigned int num_triangoli=(int)(teta*mesh.NumberCell2D);
//  // cout<<num_triangoli<<endl;
//  for (unsigned int i = 0; i < num_triangoli; i++)
//  {
//    if(mesh.DeleteCell2D[areeOrdinate[0].first]==false)
//    {
//        mesh.Bisezione(areeOrdinate[0].first);
//    }
//    areeOrdinate.erase(areeOrdinate.begin());
//  }

  // o commenti questo.
  // opzione 2
  int dim = areeOrdinate.size();
  double perc = 0.7;
  int pos = floor(dim*perc);
  double areaMax = areeOrdinate[pos].second;
  double gamma = (1-perc) * areaMax;
  while (idArea.size() > 0)
  {
      if (idArea[0].second > gamma)
      {
          unsigned int a = mesh.NumberCell2D;
          mesh.Bisezione(idArea[0].first);
          unsigned int b = mesh.NumberCell2D;
          for (unsigned int i = a; i < b+1; i++)
          {
              idArea.push_back(std::pair<unsigned int, double>(i,mesh.Cell2D[i].Area));
          }
      }
      idArea.erase(idArea.begin());
  }
  mesh.ExportMesh("/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/NewCell0Ds.csv","/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/NewCell1Ds.csv","/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/NewCell2Ds.csv");
}



