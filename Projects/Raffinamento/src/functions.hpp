#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#endif // FUNCTIONS_HPP


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


///\brief dato l'id del triangolo vogliamo estrarre l'area del triangolo
///\param mesh: a TriangularMesh struct s
///\param idT: è l'id del triangolo, nella struct sono degli unsigned int quindi gli passo una variabile di sto tipo
///\return the result of the reading, true if is success, false otherwise
double Area(TriangularMesh& mesh,unsigned int& idT);

}
