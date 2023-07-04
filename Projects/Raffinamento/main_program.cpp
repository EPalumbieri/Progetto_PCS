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
  mesh.ImportCell2Ds();

//  // -----------------------------------------------------------------------------------------------------
//  // OPZIONE 1 TOLLERANZA SUL NUMERO DI TRIANGOLI
//  vector<pair<unsigned int, double>> idArea;
//  for (unsigned int i = 0; i < mesh.NumberCell2D; i++)
//  {
//      idArea.push_back(std::pair<unsigned int, double>(i,mesh.Cell2D[i].Area));
//  }
//  vector<pair<unsigned int, double>> areeOrdinate = SortLibrary::HeapSort(idArea);
//  double teta = 0.9;
//  unsigned int num_triangoli=(int)(teta*mesh.NumberCell2D);
//  for (unsigned int i = 0; i < num_triangoli; i++)
//  {
//    if(!mesh.DeleteCell2D[areeOrdinate[0].first])
//    {
//        mesh.Bisezione(areeOrdinate[0].first);
//    }
//    areeOrdinate.erase(areeOrdinate.begin());
//  }

//  // -----------------------------------------------------------------------------------------------------
//  // OPZIONE 2 TOLLERANZA AREA VETTORE QUALUNQUE
//  while (mesh.idAreeDaBisezionare.size() > 0)
//  {
//      if(!mesh.DeleteCell2D[mesh.idAreeDaBisezionare.back().first])
//      {
//          unsigned int idT = mesh.idAreeDaBisezionare.back().first;
//          mesh.idAreeDaBisezionare.pop_back();
//          mesh.Bisezione(idT);
//      }
//      else mesh.idAreeDaBisezionare.pop_back();
//  }

  // -----------------------------------------------------------------------------------------------------
  // OPZIONE 3 TOLLERANZA AREA VETTORE ORDINATO
  vector<pair<unsigned int, double>> areeOrdinate3 = SortLibrary::HeapSort(mesh.idAreeDaBisezionare);
  while (areeOrdinate3.size() > 0)
  {
      if(!mesh.DeleteCell2D[areeOrdinate3[0].first] && areeOrdinate3[0].second > mesh.tolleranza)
      {
         unsigned int a = mesh.NumberCell2D;
         unsigned int idT = areeOrdinate3[0].first;
         areeOrdinate3.erase(areeOrdinate3.begin());
         mesh.Bisezione(idT);
         unsigned int b = mesh.NumberCell2D;
         for (unsigned int i = a; i < b+1; i++)
         {
             areeOrdinate3.push_back(std::pair<unsigned int, double>(i,mesh.Cell2D[i].Area));
         }
      }
      else
      {
         areeOrdinate3.erase(areeOrdinate3.begin());
      }
  }
  mesh.ExportMesh("/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/NewCell0Ds.csv","/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/NewCell1Ds.csv","/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/NewCell2Ds.csv");
}
// (X2-X1)*iHat+(Y2-Y1)*jHat
