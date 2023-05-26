#ifndef __EMPTY_H
#define __EMPTY_H

#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include "map"

using namespace std;
using namespace Eigen;

// qui definisco le classi e gli oggetti
namespace ProjectLibrary
{

struct TriangularMesh
{
    unsigned int NumberCell0D = 0; ///< number of Cell0D
    std::vector<unsigned int> Cell0DId = {}; ///< Cell0D id, size 1 x NumberCell0D
    std::vector<Vector2d> Cell0DCoordinates = {}; ///< Cell0D coordinates, size 2 x NumberCell0D (x,y)

    unsigned int NumberCell1D = 0; ///< number of Cell1D
    std::vector<unsigned int> Cell1DId = {}; ///< Cell1D id, size 1 x NumberCell1D
    std::vector<Vector2i> Cell1DVertices = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)

    unsigned int NumberCell2D = 0; ///< number of Cell2D
    //std::vector<bool> eliminato  =
    std::vector<unsigned int> Cell2DId = {}; ///< Cell2D id, size 1 x NumberCell2D
    std::vector<array<unsigned int, 3>> Cell2DVertices = {}; ///< Cell2D Vertices indices, size 1 x NumberCell2DVertices[NumberCell2D]
    std::vector<array<unsigned int, 3>> Cell2DEdges = {}; ///< Cell2D Cell1D indices, size 1 x NumberCell2DEdges[NumberCell2D]
    std::map<unsigned int, list<unsigned int>> Adjacency = {}; // id del lato collegato a id dei due triangoli ad esso adiacenti
   };

   //--------------------------------------------------------*

   //struttura per ordinare le aree creiamo una strutture in  prima posizione contiene id e in seconda posizione contiene l'area
   //lo utilizzo nel main per ordinare le aree e non utilizzare le mappe  che costanotano di più,
   // definisco questa struct e ordino il vettore attraverso questa struct perche creo un vettore si ArrIdArea quindo fatto questo
   //il vettore mi tornera odinato attraverso la funzione sort

   //attraverso confronto aree riusciremo a ordinare gli id grazie alle aree pechè diciamo alla macchina di guar

   struct ArrIdArea
   {
    int idTr;
    double areaTr;

   };

   inline bool operator>(const ArrIdArea &obj1, const ArrIdArea &obj2)
   {
    return obj1.areaTr> obj2.areaTr;
   }
  //--------------------------------------------------------------*


   ///\brief Import the Cell0D properties from Cell0Ds.csv file
   ///\param mesh: a TriangularMesh struct
   ///\return the result of the reading, true if is success, false otherwise
   bool ImportCell0Ds(TriangularMesh& mesh);

   ///\brief Import the Cell1D properties from Cell1Ds.csv file
   ///\param mesh: a TriangularMesh struct
   ///\return the result of the reading, true if is success, false otherwise
   bool ImportCell1Ds(TriangularMesh& mesh);

   ///\brief Import the Cell2D properties from Cell2Ds.csv file
   ///\param mesh: a TriangularMesh struct
   ///\return the result of the reading, true if is success, false otherwise
   bool ImportCell2Ds(TriangularMesh& mesh);

  //_________________________________________________________________*
   ///\brief dato l'id del triangolo vogliamo estrarre l'area del triangolo
   ///\param mesh: a TriangularMesh struct s
   ///\param idT: è l'id del triangolo, nella struct sono degli unsigned int quindi gli passo una variabile di sto tipo
   ///\return the result of the reading, true if is success, false otherwise
   double Area(TriangularMesh& mesh,unsigned int& idT);

//----* lunghezza lato funzione input id lato  prende le coordinate e calcolo la lunghezza del lato, input: id tra
//----* vertice adiacente: input: id triangolo, id lato lungo ----> output

}
#endif // __EMPTY_H
