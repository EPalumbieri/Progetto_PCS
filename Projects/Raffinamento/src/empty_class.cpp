#include "empty_class.hpp"
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include "map"

using namespace std;
using namespace Eigen;
namespace ProjectLibrary
{

// ***************************************************************************
bool ImportCell0Ds(TriangularMesh& mesh)
{

  ifstream file;
  file.open("./Cell0Ds.csv");

  if(file.fail())
    return false;

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  file.close();

  listLines.pop_front();

  mesh.NumberCell0D = listLines.size();

  if (mesh.NumberCell0D == 0)
  {
    cerr << "There is no cell 0D" << endl;
    return false;
  }

  mesh.Cell0DId.reserve(mesh.NumberCell0D);
  mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

  for (const string& line : listLines)
  {
    istringstream converter(line);

    unsigned int id;
    unsigned int marker;
    Vector2d coord;

    converter >>  id >> marker >> coord(0) >> coord(1);

    mesh.Cell0DId.push_back(id);
    mesh.Cell0DCoordinates.push_back(coord);

    if( marker != 0)
    {
      if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
        mesh.Cell0DMarkers.insert({marker, {id}});
      else
        mesh.Cell0DMarkers[marker].push_back(id);
    }

  }
  file.close();
  return true;
}
//// ***************************************************************************
bool ImportCell1Ds(TriangularMesh& mesh)
{

  ifstream file;
  file.open("./Cell1Ds.csv");

  if(file.fail())
    return false;

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  mesh.NumberCell1D = listLines.size();

  if (mesh.NumberCell1D == 0)
  {
    cerr << "There is no cell 1D" << endl;
    return false;
  }

  mesh.Cell1DId.reserve(mesh.NumberCell1D);
  mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

  for (const string& line : listLines)
  {
    istringstream converter(line);

    unsigned int id;
    unsigned int marker;
    Vector2i vertices;

    converter >>  id >> marker >> vertices(0) >> vertices(1);

    mesh.Cell1DId.push_back(id);
    mesh.Cell1DVertices.push_back(vertices);

    if( marker != 0)
    {
      if (mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end())
        mesh.Cell1DMarkers.insert({marker, {id}});
      else
        mesh.Cell1DMarkers[marker].push_back(id);
    }
  }

  file.close();

  return true;
}


// ***************************************************************************
bool ImportCell2Ds(TriangularMesh& mesh)
{

  ifstream file;
  file.open("./Cell2Ds.csv");

  if(file.fail())
    return false;

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  mesh.NumberCell2D = listLines.size();

  if (mesh.NumberCell2D == 0)
  {
    cerr << "There is no cell 2D" << endl;
    return false;
  }

  mesh.Cell2DId.reserve(mesh.NumberCell2D);
  mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
  mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

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
      // aggiungo l'id del triangolo
      }

    mesh.Cell2DId.push_back(id);
    mesh.Cell2DVertices.push_back(vertices);
    mesh.Cell2DEdges.push_back(edges);

  }
  file.close();
  return true;
}
}
 //***************************************************************************
//bool ImportMesh(TriangularMesh& mesh)
//{

//}
