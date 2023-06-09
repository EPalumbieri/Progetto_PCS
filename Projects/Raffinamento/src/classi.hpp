#ifndef __STRUCT_H
#define __STRUCT_H

#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
using namespace std;
using namespace Eigen;
namespace ProjectLibrary
{
    class Punto
    {
    public:
        unsigned int idP;
        Vector2d coordinate;
        Punto(unsigned int id, Vector2d coordinate):idP(id),coordinate(coordinate){}
    };

    class Lato
    {
    public:
       unsigned int idL;
       array<unsigned int, 2> idV;
       Lato(unsigned int idL, array<unsigned int, 2> idV):idL(idL), idV(idV){}
    };

    class Triangolo
    {
    public:
        unsigned int idT;
        array<unsigned int, 3> idV;
        array<unsigned int, 3> idL;
        double Area;
        Triangolo(unsigned int idT, array<unsigned int, 3> idV, array<unsigned int, 3> idL,double Area):idT(idT), idV(idV), idL(idL), Area(Area){}
    };

    class TriangularMesh
    {
    public:
        bool ExportMesh(string file0D, string file1D, string file2D);

        unsigned int NumberCell0D = 0; ///< number of Cell0D
        std::vector<Punto> Cell0D = {}; ///< Cell0D id, size 1 x NumberCell0D //oggetti di tipo punto
        bool ImportCell0Ds();
        bool ExportCell0Ds(string nomeFile);

        unsigned int NumberCell1D = 0; ///< number of Cell1D
        std::vector<Lato> Cell1D = {}; ///< Cell1D id, size 1 x NumberCell1D //elenco id Lati
        std::vector<bool> DeleteCell1D = {}; //True se elimino false se mantengo
        bool ImportCell1Ds();
        bool ExportCell1Ds(string nomeFile);

        static double LunghezzaLato(const Punto &P1, const Punto &P2);
        unsigned int PuntoMedio(const unsigned int &idL);

        unsigned int NumberCell2D = 0; ///< number of Cell2D
        std::vector<Triangolo> Cell2D = {}; ///< Cell2D id, size 1 x NumberCell2D
        std::map<unsigned int, list<unsigned int>> Adjacency = {};
        std::vector<bool> DeleteCell2D  ={};
        bool ImportCell2Ds();
        bool ExportCell2Ds(string nomeFile);

        unsigned int LatoLungo(const unsigned int& idT);
        unsigned int VerticeOpposto(const unsigned int& idL,const unsigned int& idT);
        bool Bisezione(const unsigned int& IdT);
        static double Area(const Punto &P1, const Punto &P2, const Punto &P3);
        unsigned int LatoAccanto(const unsigned int& idT,const unsigned int& idL, const unsigned int& idP);

        vector<pair<unsigned int,double>> idAreeDaBisezionare;
        double tolleranza= 0.002;
    };
}

#endif
