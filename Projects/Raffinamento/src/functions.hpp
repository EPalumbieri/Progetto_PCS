#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

using namespace std;
using namespace Eigen;
namespace ProjectLibrary
{

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
#endif // FUNCTIONS_HPP
