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
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; ///< Cell0D markers, size 1 x NumberCell0D (marker)

    unsigned int NumberCell1D = 0; ///< number of Cell1D
    std::vector<unsigned int> Cell1DId = {}; ///< Cell1D id, size 1 x NumberCell1D
    std::vector<Vector2i> Cell1DVertices = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; ///< Cell1D propertoes, size 1 x NumberCell1D (marker)

    unsigned int NumberCell2D = 0; ///< number of Cell2D
    std::vector<unsigned int> Cell2DId = {}; ///< Cell2D id, size 1 x NumberCell2D
    std::vector<array<unsigned int, 3>> Cell2DVertices = {}; ///< Cell2D Vertices indices, size 1 x NumberCell2DVertices[NumberCell2D]
    std::vector<array<unsigned int, 3>> Cell2DEdges = {}; ///< Cell2D Cell1D indices, size 1 x NumberCell2DEdges[NumberCell2D]
    std::map<unsigned int, list<unsigned int>> Adjacency = {}; // id del lato collegato a id dei due triangoli ad esso adiacenti
};

//    ///\brief Import the triangular mesh and test if the mesh is correct
//    ///\param mesh: a TriangularMesh struct
//    ///\return the result of the reading, true if is success, false otherwise
//    bool ImportMesh(TriangularMesh& mesh);

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


}
#endif // __EMPTY_H
