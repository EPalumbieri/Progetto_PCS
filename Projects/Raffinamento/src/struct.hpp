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

    struct Punto
    {
        unsigned int idP;
        Vector2d coordinate;
        Punto(unsigned int id, Vector2d coordinate):idP(id),coordinate(coordinate){}
    };

    struct Lato
    {
       unsigned int idL;
       array<unsigned int, 2> idV;
       //double lunghezzaLato;
       Lato(unsigned int idL, array<unsigned int, 2> idV):idL(idL), idV(idV){}
    };

    struct Triangolo
    {
        unsigned int idT;
        array<unsigned int, 3> idV;
        array<unsigned int, 3> idL;
        double Area;
        Triangolo(unsigned int idT, array<unsigned int, 3> idV, array<unsigned int, 3> idL,double Area):idT(idT), idV(idV), idL(idL), Area(Area){}
    };

    struct TriangularMesh
    {
        unsigned int NumberCell0D = 0; ///< number of Cell0D
        std::vector<Punto> Cell0D = {}; ///< Cell0D id, size 1 x NumberCell0D
        bool ImportCell0Ds();

        unsigned int NumberCell1D = 0; ///< number of Cell1D
        std::vector<Lato> Cell1D = {}; ///< Cell1D id, size 1 x NumberCell1D
        std::vector<bool> DeleteCell1D  ={}; // true esiste, false eliminato
        bool ImportCell1Ds();
        double LunghezzaLato(const unsigned int& idL);
        unsigned int PuntoMedio(const unsigned int& idL);

        unsigned int NumberCell2D = 0; ///< number of Cell2D
        std::vector<Triangolo> Cell2D = {}; ///< Cell2D id, size 1 x NumberCell2D
        std::map<unsigned int, list<unsigned int>> Adjacency = {}; // true esiste, false eliminato
        std::vector<bool> DeleteCell2D  ={}; //True esiste False eliminato
        bool ImportCell2Ds();
        unsigned int LatoLungo(const unsigned int& idT);
        unsigned int VerticeOpposto(const unsigned int& idL,const unsigned int& idT);
        bool Bisezione(const unsigned int& IdT);
        static double Area(const Punto &P1, const Punto &P2, const Punto &P3);
    };

}

#endif // __EMPTY_H
