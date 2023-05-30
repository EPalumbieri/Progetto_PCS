#ifndef __STRUCT_H
#define __STRUCT_H

#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
using namespace std;
using namespace Eigen;


// qui definisco le classi e gli oggetti
namespace ProjectLibrary
{

    struct TriangularMesh
    {
        unsigned int NumberCell0D = 0; ///< number of Cell0D
        std::vector<Punto> Cell0D = {}; ///< Cell0D id, size 1 x NumberCell0D
        bool ImportCell0Ds();

        unsigned int NumberCell1D = 0; ///< number of Cell1D
        std::vector<Lato> Cell1D = {}; ///< Cell1D id, size 1 x NumberCell1D
        std::vector<bool> DeleteCell1D  ={}; // true esiste, false eliminato

        unsigned int NumberCell2D = 0; ///< number of Cell2D
        std::vector<Triangolo> Cell2D = {}; ///< Cell2D id, size 1 x NumberCell2D
        std::map<unsigned int, list<unsigned int>> Adjacency = {}; // true esiste, false eliminato
        std::vector<bool> DeleteCell2D  ={}; //True esiste False eliminato
    };

    struct Punto
    {
        unsigned int id;
        Vector2d coordinate;
    };

    struct Lato
    {
       unsigned int id;
       Vector2i idV;
    };

    struct Triangolo
    {
        unsigned int id;
        Vector3i idV;
        Vector3i idL;
        double Area;
    };

   //--------------------------------------------------------*

   //struttura per ordinare le aree creiamo una strutture in  prima posizione contiene id e in seconda posizione contiene l'area
   //lo utilizzo nel main per ordinare le aree e non utilizzare le mappe  che costanotano di più,
   // definisco questa struct e ordino il vettore attraverso questa struct perche creo un vettore si ArrIdArea quindo fatto questo
   //il vettore mi tornera odinato attraverso la funzione sort

   //attraverso confronto aree riusciremo a ordinare gli id grazie alle aree pechè diciamo alla macchina di guar

//   struct ArrIdArea
//   {
//    unsigned int idTr;
//    double areaTr;

//   };

//   inline bool operator>(const ArrIdArea &obj1, const ArrIdArea &obj2)
//   {
//    return obj1.areaTr> obj2.areaTr;
//   }


   ///--------------------------------------------------------------*
   ///\brief Import the Cell0D properties from Cell0Ds.csv file
   ///\param mesh: a TriangularMesh struct
   ///\return the result of the reading, true if is success, false otherwise
   // bool ImportCell0Ds(TriangularMesh& mesh);

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
   ///\return area triangolo
   double Area(TriangularMesh& mesh,unsigned int& idT);

   ///\brief dato l'id del lato ci restituisce la lunghezza del lato
   ///\param mesh: a TriangularMesh struct
   ///\param idL: è l'id del lato
   ///\return lunghezza lato
   double LunghezzaLato(TriangularMesh& mesh,unsigned int idL);

   ///\brief dato l'id del triangolo ci restituisce il lato lungo
   ///\param mesh: a TriangularMesh struct
   ///\param idT: è l'id del triangolo
   ///\return id lato maggiore dei 3
   unsigned int LatoLungo(TriangularMesh& mesh,unsigned int idT);

   ///\brief dato l'id del triangolo, l'id del lato lungo ci restituisce il vertice opposto
   ///\param mesh: a TriangularMesh struct
   ///\param idL: è l'id del lato
   ///\param idT: è l'id del triangolo
   ///\return id vertice opposto
   unsigned int VerticeOpposto(TriangularMesh& mesh,unsigned int idL,unsigned int idT);

   ///\brief calcola punto medio e aggiorna
   ///\param mesh: a TriangularMesh struct
   ///\param idL: è l'id del lato
   ///\return id vertice punto medio
   unsigned int PuntoMedio(TriangularMesh& mesh,unsigned int& idL);

   ///\brief dato l'id del triangolo divede e aggiorna
   ///\param mesh: a TriangularMesh struct
   ///\param idT: è l'id del triangolo
   ///\return taglia
   bool Bisezione(TriangularMesh& mesh,unsigned int IdT);
}

#endif // __EMPTY_H
