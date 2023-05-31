#include "struct.hpp"
#include "sorting.hpp"
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include <algorithm>

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{
 double TriangularMesh::Area(const Punto &P1, const Punto &P2, const Punto &P3)
{
    return abs((P1.coordinate(0)*(P2.coordinate(1)-P3.coordinate(1))+P2.coordinate(0)*(P3.coordinate(1)-P1.coordinate(1))+P3.coordinate(0)*(P1.coordinate(1)-P2.coordinate(1)))/2);
}
//----------------------------------------------------------------------------

 double TriangularMesh::LunghezzaLato(const Punto &P1, const Punto &P2)
 {
    double lunghezza = sqrt(abs((P1.coordinate(0)-P2.coordinate(0))*(P1.coordinate(0)-P2.coordinate(0))-(P1.coordinate(1)-P2.coordinate(1))*(P1.coordinate(1)-P2.coordinate(1))));
    return lunghezza;
 }

// double TriangularMesh::LunghezzaLato(const unsigned int& idL)
// {
//    //array<unsigned int, 2> vertici = this->Cell1D[idL].idV; // array<unsigned int, 2> vertici = mesh.Cell1DVertices[idL];
//    Vector2d coord1 = this->Cell0D[Cell1D[idL].idV[0]].coordinate; // array<double, 2> = mesh.Cell0DCoordinates[vertici[0]];
//    Vector2d coord2 = this->Cell0D[Cell1D[idL].idV[1]].coordinate; // stesso di su

//    // io lo calcolerei senza radice per il costo computazionale (tanto non fa differenza)
//    double lunghezza = sqrt((coord2[0] - coord1[0]) * (coord2[0] - coord1[0]) + (coord2[1] - coord1[1]) * (coord2[1] - coord1[1]));
//    return lunghezza;
//  }
//----------------------------------------------------------------------------
 unsigned int TriangularMesh::LatoLungo(const unsigned int& idT)
  {
    array<unsigned int, 3> idLati = this->Cell2D[idT].idL;
    // Calcola la lunghezza di ogni lato e determina l'id del lato più lungo
    double lunghezzaMax = 0.0;
    unsigned int idLatoLungo = 0; // qui 0 non va bene perché corrisponde ad un id >>>> unsigned int idLatoLungo;

    for (unsigned int i = 0; i < 3; i++)    {
//       unsigned int idL = idLati[i];
//       double lunghezza = LunghezzaLato(idL);

       unsigned int idL = idLati[i];
       double lunghezza = LunghezzaLato(this->Cell0D[Cell1D[idL].idV[0]],this->Cell0D[Cell1D[idL].idV[1]]);

       //calcolo il lato massimo
       if (lunghezza > lunghezzaMax)    {
          lunghezzaMax = lunghezza;
          idLatoLungo = idL;
       }
    }
    return idLatoLungo;
  }
//----------------------------------------------------------------------------
 unsigned int TriangularMesh::VerticeOpposto(const unsigned int& idT,const unsigned int& idL)
  {
   array<unsigned int, 2> idVertici = this->Cell1D[idL].idV; // array<unsigned int, 2> idVertici = mesh.Cell1DVertices[idL];
   unsigned int vertice1 = idVertici[0]; // estrae i due vertici separatamente del lato piu lungo
   unsigned int vertice2 = idVertici[1];

   // Ottieni gli id dei tre vertici del triangolo idT considerato
   array<unsigned int, 3> idVerticiTriangolo = this->Cell2D[idT].idV;

   // Cerca l'id del vertice opposto al lato
   unsigned int verticeOpposto= 0;

   for (unsigned int i = 0; i < 3; i++)
   {
    unsigned int idVertice = idVerticiTriangolo[i];
    if (idVertice != vertice1 && idVertice != vertice2)
    {
     verticeOpposto = idVertice;

    }
  }
  return verticeOpposto;
 }

//----------------------------------------------------------------------------
unsigned int TriangularMesh::PuntoMedio(const unsigned int& idL)
{
    Vector2d punto1 = this->Cell0D[Cell1D[idL].idV[0]].coordinate;
    Vector2d punto2 = this->Cell0D[Cell1D[idL].idV[1]].coordinate;

//    array<unsigned int, 2> vertici = this->Cell1D[idL].idV; // array<unsigned int, 2> vertici = mesh.Cell1DVertices[idL];
//    Vector2d punto1 = this->Cell0DCoordinates[vertici[0]]; // array<double, 2> = mesh.Cell0DCoordinates[vertici[0]];
//    Vector2d punto2 = mesh.Cell0DCoordinates[vertici[1]];

    Vector2d pm;
    pm(0) = (punto1[0] + punto2[0])/2;
    pm(1) = (punto1[1] + punto2[1])/2;

    this->Cell0D.push_back(Punto(this->NumberCell0D, pm)); //
    //mesh.Cell0DId.push_back(mesh.NumberCell0D); //perché
    this->NumberCell0D ++;

    return this->NumberCell0D; //-1 perché parto da zero
}
//----------------------------------------------------------------------------
unsigned int TriangularMesh::LatoAccanto(const unsigned int& idT,const unsigned int& idL, const unsigned int& idP)
{
    array<unsigned int, 3> lati = this->Cell2D[idT].idL;
    unsigned int LatoAccanto=0;
    for (unsigned int i = 0; i < 3; i++)
    {   // se il lato preso contiene v1 e non è le, lo prendo, altrimenti prendo l'altro
        if (lati[i] != idL)
        { //cioè vado avanti
            array<unsigned int, 2> vertici = this->Cell1D[lati[i]].idV;
            if ( vertici[0] == idP || vertici[1] == idP)
            {
                LatoAccanto = lati[i]; //legato a 1m
                break;
            }
        }
    }
    return LatoAccanto;
}
//----------------------------------------------------------------------------
bool TriangularMesh::Bisezione(const unsigned int& idT)
{
   unsigned int idLL = LatoLungo(idT);
   unsigned int idVM = PuntoMedio(idLL);
   unsigned int idVO = VerticeOpposto(idT, idLL);
   this->DeleteCell1D[idLL]=true;  // cancello ID lato lungo
   this->DeleteCell2D[idT]=true;   // cancello id triangolo

   // costruisco la mediana
   array<unsigned int, 2> LatoMO;
   LatoMO[0] = idVM;
   LatoMO[1] = idVO;
   unsigned int idLatoMO =this->NumberCell1D;

   this->Cell1D.push_back(Lato(idLatoMO, LatoMO)); // Lato(id,lato)
   this->DeleteCell1D.push_back(false); //aggiungo id lato medio
   this->NumberCell1D++;

   // creo il lato piccolo 1
   array<unsigned int, 2> idVertici = this->Cell1D[idLL].idV; // estraggo i vertici del lato più lungo

   unsigned int idV1 = idVertici[0];
   array<unsigned int, 2> Lato1Mvertici;
   Lato1Mvertici[0] = idV1;
   Lato1Mvertici[1] = idVM;
   unsigned int idLato1M = this->NumberCell1D  ;

   this->Cell1D.push_back(Lato(idLato1M, Lato1Mvertici));
   this->DeleteCell1D.push_back(false);
   this->NumberCell1D++;
    
   // creo il lato piccolo 2
   unsigned int idV2 = idVertici[1];
   array<unsigned int, 2> Lato2Mvertici;
   Lato2Mvertici[0] = idV2;
   Lato2Mvertici[1] = idVM;
   unsigned int idLato2M =this->NumberCell1D;

   this->Cell1D.push_back(Lato(idLato2M, Lato2Mvertici));
   this->DeleteCell1D.push_back(false);
   this->NumberCell1D++;

   unsigned int Lato1O = LatoAccanto(idT, idLL, idV1);
   unsigned int Lato2O = LatoAccanto(idT, idLL, idV2);

//----------------------------------------------------------------------------
   //ADESSO CREO IL TRIANGOLO IdT1
   array<unsigned int, 3> vertici1 = {idV1, idVO, idVM};
   array<unsigned int, 3> lati1 = {Lato1O, idLatoMO, idLato1M};

   //salvo il triagolo
   unsigned int idT1 = this->NumberCell2D;
   double area1=Area(this->Cell0D[vertici1[0]],this->Cell0D[vertici1[1]],this->Cell0D[vertici1[2]]);
   this->Cell2D.push_back(Triangolo(idT1,vertici1,lati1,area1));
   this->DeleteCell2D.push_back(false);
   this->NumberCell2D++;

    //aggiorno adiacenza per lato opposto Lato1O
    auto it1 = this->Adjacency.find(Lato1O);
    if (it1 != this->Adjacency.end())
    {
        auto& valueList = it1->second;
        auto itElement = std::find(valueList.begin(), valueList.end(), idT);
        if (itElement != valueList.end()) {
            valueList.erase(itElement); // Rimuovi vecchioId
            valueList.push_back(idT1); // Inserisci il nuovo IdT2
         }
    };

    // AGGIUNGO IL NUOVO LATO CON LE ADIACENZE idLato1M
    this->Adjacency.insert({idLato1M,{idT1}});

 //_____________________________________________________________________________

    //CREO NUOVO TRIANGOLO IdT2
    array<unsigned int, 3> vertici2 = {idV2, idVO, idVM};
    array<unsigned int, 3> lati2 = {Lato2O,idLatoMO, idLato2M};

    unsigned int IdT2 = this->NumberCell2D;
    double area2=Area(this->Cell0D[vertici2[0]],this->Cell0D[vertici2[1]],this->Cell0D[vertici2[2]]);
    this->Cell2D.push_back(Triangolo(IdT2,vertici2,lati2,area2)); //creo un IdT2
    this->NumberCell2D++;
    this->DeleteCell2D.push_back(false);

    //adiacenze
    auto it2 = this->Adjacency.find(Lato2O);
    if (it2 != this->Adjacency.end())
    {
        auto& valueList = it2->second;
        auto itElement = std::find(valueList.begin(), valueList.end(), idT);
        if (itElement != valueList.end()) {
            valueList.erase(itElement); // Rimuovi vecchioId
            valueList.push_back(IdT2); // Inserisci il nuovo IdT2
         }
    };

    // AGGIUNGO IL NUOVO LATO CON LE ADIACENZE idLato2M
    this->Adjacency.insert({idLato2M,{IdT2}});
//-------------------------------------------*

    //Aggiorno adiacenze per latoMO che ha sia il IdT2, IdT1
    this->Adjacency.insert({idLatoMO,{idT1,IdT2}}); //aggiungo l'id del lato e lo associo all'id del triangolo

//==============================================================================================================
    //CASO IN CUI HA TRIANGOLO ADIACENTE//

    auto itA = this->Adjacency.find(idLL);
    if (itA!= this->Adjacency.end()) //controlla che l'elemento sia stato trovato correttamente
    {
 //iteriamo sugli elementi della lista che ha come chiave il lato lungo e prendiamo quello con id diverso da quello sopra per ricreare i 2 triangoli sotto
    if (itA->second.size() > 1)
        {
        for (const unsigned int& IdTA : itA -> second)
       {
         if (IdTA != idT)
         {
            //devo creare solo il lato tra il punto medio e il nuovo vertice opposto
             //AAAAAAAAAAAAAAAAAAA
             idVO = VerticeOpposto(IdTA, idLL);
             LatoMO[0] = idVM;
             LatoMO[1] = idVO;
             unsigned int idLatoMO =this->NumberCell1D;
             this->Cell1D.push_back(Lato(idLatoMO, LatoMO));
             this->DeleteCell1D.push_back(false);
             this->NumberCell1D++;

             Lato1O = LatoAccanto(IdTA, idLL, idV1);
             Lato2O = LatoAccanto(IdTA, idLL, idV2);

             //ADESSO CREO IL TRIANGOLO IdTr1A
             vertici1 = {idV1, idVO, idVM};
             lati1 = {Lato1O, idLatoMO, idLato1M};

             //salvo il triangolo
             unsigned int IdTr1A = this->NumberCell2D;
             double area1A = Area(this->Cell0D[vertici1[0]],this->Cell0D[vertici1[1]],this->Cell0D[vertici1[2]]);
             this->Cell2D.push_back(Triangolo(IdTr1A, vertici1, lati1,area1A)); //creo un IdTr1A
             this->DeleteCell2D.push_back(false);
             this->NumberCell2D++;

             //ADIACENZA
             //LATO PICCOLO
             this->Adjacency[idLato1M].push_back(IdTr1A); // aggiungi all'adiacenza del lato piccolo il nuovo triangolo

             //SOSTIUIAMO VECCHIO ID CON ID Tr1A
             auto it1A = this->Adjacency.find(Lato1O);
             if (it1A != this->Adjacency.end())
             {
                 auto& valueList = it1A->second;
                 auto itElement = std::find(valueList.begin(), valueList.end(), IdTA);
                 if (itElement != valueList.end()) {
                     valueList.erase(itElement); // Rimuovi vecchioId
                     valueList.push_back(IdTr1A); // Inserisci il nuovo IdT2
                  }
             };
             //_____________________________________________________________________________

             //CREO NUOVO TRIANGOLO IdTrA2
             vertici2 = {idV2, idVO, idVM};
             lati2 = {Lato2O,idLatoMO, idLato2M};

             unsigned int IdTr2A = this->NumberCell2D;
             double area2A = Area(this->Cell0D[vertici2[0]],this->Cell0D[vertici2[1]],this->Cell0D[vertici2[2]]);
             this->Cell2D.push_back(Triangolo(IdTr2A, vertici2,lati2,area2A)); //creo un IdT2
             this->NumberCell2D++;
             this->DeleteCell2D.push_back(false);

             //adiacenze
             //lato piccolo
             this->Adjacency[idLato2M].push_back(IdTr2A);

            //SOSTIUIAMO VECCHIO ID CON ID Tr1A
             auto it2A = this->Adjacency.find(Lato2O);
             if (it2A != this->Adjacency.end())
             {
                auto& valueList = it2A->second;
                auto itElement = std::find(valueList.begin(), valueList.end(), IdTA);
                if (itElement != valueList.end()) {
                    valueList.erase(itElement); // Rimuovi vecchioId
                    valueList.push_back(IdTr2A); // Inserisci il nuovo IdT2
                 }
             };
             //------
             //ADIACENZA LATO MEDIO
             this->Adjacency.insert({idLatoMO,{IdTr1A,IdTr2A}});
             this->DeleteCell2D[IdTA]=true; //ho cambiato da false a true
         };
       };
    };
    };

//=========================================================================
//    //elimino chiave e adiacenze lato lungo
    this->Adjacency.erase(idLL);

    this->DeleteCell2D[idT]=true;

    return 0; // zero xk elimini il triangolo bisezionato

}
} // chiudo project library


