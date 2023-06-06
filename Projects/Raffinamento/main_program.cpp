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

  // ordino il vettore in base alle aree
  vector<pair<unsigned int, double>> areeOrdinate = SortLibrary::HeapSort(idArea);

  // applico la bisezione fino a una certa tolleranza (da sistemare in relativa)
  double teta = 0.6;
  unsigned int num_triangoli=(int)(teta*mesh.NumberCell2D);
  cout<<num_triangoli<<endl;
  for (unsigned int i = 0; i < num_triangoli; i++)
  {
    if(mesh.DeleteCell2D[areeOrdinate[0].first]==false){
    mesh.Bisezione(areeOrdinate[0].first);
    areeOrdinate.erase(areeOrdinate.begin());
    }

  //for (unsigned int i = 1; i < static_cast<unsigned int>(num_triangoli); i++)
  for (unsigned int i = 0; i <20; i++)
  {
      if(mesh.DeleteCell2D[areeOrdinate[0].first]==false)
      { mesh.Bisezione(areeOrdinate[0].first);
        areeOrdinate.erase(areeOrdinate.begin());}
      areeOrdinate.erase(areeOrdinate.begin());
  }


}
  mesh.ExportMesh("C:/Users/filip/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/NewCell0Ds.csv","C:/Users/filip/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/NewCell1Ds.csv","C:/Users/filip/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/NewCell2Ds.csv");
}



