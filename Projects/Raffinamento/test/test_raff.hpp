#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>
#include "struct.hpp"
#include "sorting.hpp"
#include "Eigen/Eigen"

using namespace testing;
using namespace Eigen;
using namespace SortLibrary;
namespace ProjectLibrary
{

//TEST METODI STATICI CIOE CHE NON UTILIZZANO ATTRIBUTI DELLA MESH ( GUARDARE FUNZIONIZIONI ) TriangularMesh::METODO(input)
//diamo noi input
TEST(testAreaFunzione, testAreaCalcolata)
{
  Vector2d v1;
  v1 << 0,0;

  Vector2d v2;
  v2 << 0,1;

  Vector2d v3;
  v3 << 1,0;

  Punto P1= Punto(0,v1);
  Punto P2= Punto(1,v2);
  Punto P3= Punto(2,v3);

  double area = TriangularMesh::Area(P1,P2,P3);

  EXPECT_EQ(area, 0.5);
}

TEST(TestSorting, TestHeapSortIntBuiltIn)
{
  vector<int> v = {9, 13, 5, 10, 2, 7, 9, 4, 6, 12};
  vector<int> ssV = HeapSort<int>(v);
  vector<int> sortedV = {2, 4, 5, 6, 7, 9, 9, 10, 12 ,13};
  EXPECT_EQ(ssV, sortedV);
}

TEST(TestSorting, TestHeapSortVectorPair)
{
  vector<pair<unsigned int,double>> v;
  for(int i=0;i<10;i++){
      v.push_back(pair<unsigned int,double> (i+1,i));
  }
  vector<pair<unsigned int,double>> ssV = SortLibrary::HeapSort<pair<unsigned int,double>>(v);

  vector<pair<unsigned int,double>> sortedV;
  for(int i=9;i>=0;i--){
      sortedV.push_back(pair<unsigned int,double> (i+1,i));
  }
  EXPECT_EQ(ssV, sortedV);
}

TEST(testLenght, testLunghezzaVera)
{
    Vector2d v1;
    v1 << 0,0;

    Vector2d v2;
    v2 << 0,1;

    Punto P1= Punto(0,v1);

    Punto P2= Punto(1,v2);

    double lunghezza = TriangularMesh::LunghezzaLato(P1, P2);

    EXPECT_EQ(lunghezza, 1.0);
}


//su metodi non statici --------- si usano attrbuti della mesh nella funzione quindi si scrive nomeOggetto.metodo(input)

TEST(testLatoLungo, testLatoLungoVerificato)
{   TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT = 74;
    unsigned int idll = meshTest.LatoLungo(idT);
    EXPECT_EQ(idll, 142);
}

TEST(testVerOpp, testVerOppVerificato)
{
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT = 74;
    unsigned int idL = 142;

    unsigned int VO = meshTest.VerticeOpposto(idT,idL);

    EXPECT_EQ(VO,47);
}

TEST(testPuntoMedioFunc, testPM)
{
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();


    unsigned int idLL =142;
    unsigned int idPMgiusto = meshTest.PuntoMedio(idLL);
    unsigned int idPmCell0D = meshTest.Cell0D[idPMgiusto].idP;
    //SERVE A VERIFICARE SE METTE E SLAVA BENE L'OGGETTO PUNTO IN CELL0D DELLA MESH

    EXPECT_EQ(idPmCell0D,idPMgiusto);
}


TEST(testLatoAccantoFunc,testLatoAccanto)
{
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT= 74;
    unsigned int idL12= 142;
    unsigned int idP1= 50;

    //SERVE A VERIFICARE SE METTE E SALVA BENE L'OGGETTO PUNTO IN CELL0D DELLA MESH
    unsigned int L10Giusto=139;

    unsigned int IdL10= meshTest.LatoAccanto(idT,idL12,idP1);;

    EXPECT_EQ(IdL10,L10Giusto);
}
//*************************************************************** TEST BISEZIONE
//TEST T1
TEST(testBisezioneFunc,testBisezioneT1vert)
{
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT= 74;
    meshTest.Bisezione(idT);
    // IN ORDINE GLI INSERIAMO P1,P0,PM
    array<unsigned int, 3> idVT1= meshTest.Cell2D[meshTest.NumberCell2D-4].idV;
    array<unsigned int, 3> idVT1Giusto={50,47,89};
    EXPECT_EQ(idVT1,idVT1Giusto);
 }



 TEST(testBisezioneFunc,testBisezioneT1lati)
 {
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT= 74;
    meshTest.Bisezione(idT);
    // IN ORDINE GLI INSERIAMO L1O, LM0, L1M
    array<unsigned int, 3> idlT1= meshTest.Cell2D[meshTest.NumberCell2D-4].idL;
    array<unsigned int, 3> idlT1Giusto={139,232,233};
    EXPECT_EQ(idlT1,idlT1Giusto);
 }

// TEST TRIANGOLO T2
 TEST(testBisezioneFunc,testBisezioneT2vert)
 {
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT= 74;
    meshTest.Bisezione(idT);
    // IN ORDINE GLI INSERIAMO P2,P0,PM
    array<unsigned int, 3> idVT2= meshTest.Cell2D[meshTest.NumberCell2D-3].idV;
    array<unsigned int, 3> idVT2Giusto={55,47,89};
    EXPECT_EQ(idVT2,idVT2Giusto);
 }



 TEST(testBisezioneFunc,testBisezioneT2lati)
 {
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT= 74;
    meshTest.Bisezione(idT);
    // IN ORDINE GLI INSERIAMO L2O, LM0, L2M
    array<unsigned int, 3> idlT2= meshTest.Cell2D[meshTest.NumberCell2D-3].idL;
    array<unsigned int, 3> idlT2Giusto={9,232,234};
    EXPECT_EQ(idlT2,idlT2Giusto);
 }


 //TEST T4
 TEST(testBisezioneFunc,testBisezioneT4vert)
 {
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT= 74;
    meshTest.Bisezione(idT);
    // IN ORDINE GLI INSERIAMO P1,P3,PM
    array<unsigned int, 3> idVT4= meshTest.Cell2D[meshTest.NumberCell2D-2].idV;
    array<unsigned int, 3> idVT4Giusto={50,59,89};
    EXPECT_EQ(idVT4,idVT4Giusto);
 }



 TEST(testBisezioneFunc,testBisezioneT4lati)
 {
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT= 74;
    meshTest.Bisezione(idT);
    // IN ORDINE GLI INSERIAMO L13, LM3, L1M
    array<unsigned int, 3> idlT4= meshTest.Cell2D[meshTest.NumberCell2D-2].idL;
    array<unsigned int, 3> idlT4Giusto={147,235,233};
    EXPECT_EQ(idlT4,idlT4Giusto);
 }

 // TEST TRIANGOLO T5
 TEST(testBisezioneFunc,testBisezioneT5vert)
 {
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT= 74;
    meshTest.Bisezione(idT);
    // IN ORDINE GLI INSERIAMO P2,P3,PM
    array<unsigned int, 3> idVT5= meshTest.Cell2D[meshTest.NumberCell2D-1].idV;
    array<unsigned int, 3> idVT5Giusto={55,59,89};
    EXPECT_EQ(idVT5,idVT5Giusto);
 }



 TEST(testBisezioneFunc,testBisezioneT5lati)
 {
    TriangularMesh meshTest;
    meshTest.ImportCell0Ds();
    meshTest.ImportCell1Ds();
    meshTest.ImportCell2Ds();

    unsigned int idT= 74;
    meshTest.Bisezione(idT);
    // IN ORDINE GLI INSERIAMO L23, LM3, L2M
    array<unsigned int, 3> idlT5= meshTest.Cell2D[meshTest.NumberCell2D-1].idL;
    array<unsigned int, 3> idlT5Giusto={148,235,234};
    EXPECT_EQ(idlT5,idlT5Giusto);
 }


}
#endif // __TEST_EMPTY_H













