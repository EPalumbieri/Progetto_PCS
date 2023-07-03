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

  // OPZIONE 1 TOLLERANZA SUL NUMERO DI TRIANGOLI
  vector<pair<unsigned int, double>> areeOrdinate = SortLibrary::HeapSort(idArea);
  double teta = 0.9;
  unsigned int num_triangoli=(int)(teta*mesh.NumberCell2D);
  for (unsigned int i = 0; i < num_triangoli; i++)
  {
    if(mesh.DeleteCell2D[areeOrdinate[0].first]==false)
    {
        mesh.Bisezione(areeOrdinate[0].first);
    }
    areeOrdinate.erase(areeOrdinate.begin());
  }

  // OPZIONE 2 TOLLERANZA AREA
  while (mesh.idAreeDaBisezionare.size() > 0)
  {
      if(!mesh.DeleteCell2D[mesh.idAreeDaBisezionare.back().first])
      {
          unsigned int idT = mesh.idAreeDaBisezionare.back().first;
          mesh.idAreeDaBisezionare.pop_back();
          mesh.Bisezione(idT);
      }
      else mesh.idAreeDaBisezionare.pop_back();
  }
  mesh.ExportMesh("/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/NewCell0Ds.csv","/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/NewCell1Ds.csv","/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/NewCell2Ds.csv");
}

// (X2-X1)*iHat+(Y2-Y1)*jHat



