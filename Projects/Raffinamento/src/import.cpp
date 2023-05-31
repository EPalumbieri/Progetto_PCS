#include "struct.hpp"
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{
bool TriangularMesh::ImportCell0Ds()
{
    ifstream file;
    file.open("C:/Users/elisa/OneDrive/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv");
    if (file.fail())
    {
    cerr<<"file open failed 0"<< endl;
    return false;
    }

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  this->NumberCell0D = listLines.size();

  if (this->NumberCell0D == 0)
  {
    cerr << "There is no cell 0D" << endl;
    return false;
  }

  this->Cell0D.reserve(this->NumberCell0D);
  // mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

  for (const string& line : listLines)
  {
    istringstream converter(line);

    unsigned int id;
    unsigned int marker;
    Vector2d coord;

    converter >>  id >> marker >> coord(0) >> coord(1);

    this->Cell0D.push_back(Punto(id,coord));

  }
  file.close();
  return true;
}
// ***************************************************************************
bool TriangularMesh::ImportCell1Ds()
{
    ifstream file;
    file.open("C:/Users/elisa/OneDrive/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv");

    if (file.fail())
    {
    cerr<<"file open failed 1"<< endl;
    return false;
    }

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  this->NumberCell1D = listLines.size();

  if (this->NumberCell1D == 0)
  {
    cerr << "There is no cell 1D" << endl;
    return false;
  }

  this->Cell1D.reserve(this->NumberCell1D);
  //mesh.Cell1DVertices.reserve(mesh.NumberCell1D);
  this->DeleteCell1D.resize(this->NumberCell1D); //li inizializza a false

  for (const string& line : listLines)
  {
    istringstream converter(line);

    unsigned int id;
    unsigned int marker;
    array<unsigned int, 2> vertices;

    converter >>  id >> marker >> vertices[0] >> vertices[1];

    this->Cell1D.push_back(Lato(id,vertices));
  }

  file.close();
  return true;
}
// ***************************************************************************
bool TriangularMesh::ImportCell2Ds()
{
  ifstream file;
  file.open("C:/Users/elisa/OneDrive/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv");
  if (file.fail())
  {
  cerr<<"file open failed 2"<< endl;
  return false;
  }

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  this->NumberCell2D = listLines.size();

  if (this->NumberCell2D == 0)
  {
    cerr << "There is no cell 2D" << endl;
    return false;
  }

  this->Cell2D.reserve(this->NumberCell2D);
  this->DeleteCell2D.resize(this->NumberCell2D);

  for (const string& line : listLines)
  {
    istringstream converter(line);

    // inizializzo i dati
    unsigned int id;
    array<unsigned int, 3> vertices;
    array<unsigned int, 3> edges;

    converter >>  id;
    for(unsigned int i = 0; i < 3; i++)
      converter >> vertices[i];
    for(unsigned int i = 0; i < 3; i++)
    {
      converter >> edges[i];
         if (this->Adjacency.find(edges[i]) == this->Adjacency.end()) //se non c'è nella mappa
              this->Adjacency.insert({edges[i],{id}}); //aggiungo l'id del lato e lo associo all'id del triangolo
          else
              this->Adjacency[edges[i]].push_back(id); // aggiungo l'id del triangolo
    }
    double area=Area(this->Cell0D[vertices[0]],this->Cell0D[vertices[1]],this->Cell0D[vertices[2]]);
    this->Cell2D.push_back(Triangolo(id, vertices, edges, area));
//    mesh.DeleteCell1D.push_back(true);
  }
  file.close();
  return true;
}
}
