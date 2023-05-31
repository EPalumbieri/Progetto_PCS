#include "struct.hpp"
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
//#include "map"

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{
bool TriangularMesh::ImportCell0Ds()
{
    ifstream file;
    file.open("/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv");
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
    file.open("/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv");

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
  file.open("/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv");
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
// ***************************************************************************
 double TriangularMesh::Area(const Punto &P1, const Punto &P2, const Punto &P3)
{
    return abs((P1.coordinate(0)*(P2.coordinate(1)-P3.coordinate(1))+P2.coordinate(0)*(P3.coordinate(1)-P1.coordinate(1))+P3.coordinate(0)*(P1.coordinate(1)-P2.coordinate(1)))/2);
}
//----------------------------------------------------------------------------
 double TriangularMesh::LunghezzaLato(const unsigned int& idL)
 {
    //array<unsigned int, 2> vertici = this->Cell1D[idL].idV; // array<unsigned int, 2> vertici = mesh.Cell1DVertices[idL];
    Vector2d coord1 = this->Cell0D[Cell1D[idL].idV[0]].coordinate; // array<double, 2> = mesh.Cell0DCoordinates[vertici[0]];
    Vector2d coord2 = this->Cell0D[Cell1D[idL].idV[1]].coordinate; // stesso di su

    // io lo calcolerei senza radice per il costo computazionale (tanto non fa differenza)
    double lunghezza = sqrt((coord2[0] - coord1[0]) * (coord2[0] - coord1[0]) + (coord2[1] - coord1[1]) * (coord2[1] - coord1[1]));
    return lunghezza;
  }
//----------------------------------------------------------------------------
 unsigned int TriangularMesh::LatoLungo(const unsigned int& idT)
  {
    array<unsigned int, 3> idLati = this->Cell2D[idT].idL;
    // Calcola la lunghezza di ogni lato e determina l'id del lato più lungo
    double lunghezzaMax = 0.0;
    unsigned int idLatoLungo = 0; // qui 0 non va bene perché corrisponde ad un id >>>> unsigned int idLatoLungo;

    for (unsigned int i = 0; i < 3; i++)    {
       unsigned int idLato = idLati[i];
       double lunghezza = LunghezzaLato(idLato);
       //calcolo il lato massimo
       if (lunghezza > lunghezzaMax)    {
          lunghezzaMax = lunghezza;
          idLatoLungo = idLato;
       }
    }
    return idLatoLungo;
  }
//----------------------------------------------------------------------------
 unsigned int TriangularMesh::VerticeOpposto(const unsigned int& idT,const unsigned int& idL)
  {
   array<unsigned int, 2> idVertici = this->Cell1D[idL].idV; // array<unsigned int, 2> idVertici = mesh.Cell1DVertices[idL];
   unsigned int vertice1 = idVertici[0]; //non so a che serve, estrae i due vertici separatamente
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
     break;
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
    // mesh.NumberCell0D++;

    return this->NumberCell0D-1;
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
bool TriangularMesh::Bisezione(const unsigned int& IdT)
{

   unsigned int idLE = LatoLungo(IdT);
   unsigned int IdVPM = PuntoMedio(idLE);
   unsigned int IdVO = VerticeOpposto(IdT, idLE);
   this->DeleteCell1D[idLE]=true;
   this->DeleteCell2D[IdT]=true;

    array<unsigned int, 2> LatoMO;
    LatoMO[0] = IdVPM;
    LatoMO[1] = IdVO;
    unsigned int idLatoMO =this->NumberCell1D;

    this->Cell1D.push_back(Lato(idLatoMO, LatoMO));
    this->DeleteCell1D.push_back(false);
    this->NumberCell1D++;

    // creo i lati piccoli
    array<unsigned int, 2> IdVertici = this->Cell1D[idLE].idV;

    unsigned int IdV1 = IdVertici[0];
    array<unsigned int, 2> Lato1Mvertici;
    Lato1Mvertici[0] = IdV1;
    Lato1Mvertici[1] = IdVPM;
    unsigned int idLato1M = this->NumberCell1D -1 ;

    this->Cell1D.push_back(Lato(idLato1M, Lato1Mvertici));
    this->DeleteCell1D.push_back(false);
    this->NumberCell1D++;
    

    unsigned int IdV2 = IdVertici[1];
    array<unsigned int, 2> Lato2Mvertici;
    Lato2Mvertici[0] = IdV2;
    Lato2Mvertici[1] = IdVPM;
    unsigned int idLato2M =this->NumberCell1D -1;

    this->Cell1D.push_back(Lato(idLato2M, Lato2Mvertici));
    this->DeleteCell1D.push_back(false);
    this->NumberCell1D++;

    unsigned int Lato1Opp = LatoAccanto(IdT, idLE, IdV1);
    unsigned int Lato2Opp = LatoAccanto(IdT, idLE, IdV2);

//_____________________________________________________________________________
    //ADESSO CREO IL TRIANGOLO IdT1
    array<unsigned int, 3> vertici1 = {IdV1, IdVO, IdVPM};
    array<unsigned int, 3> lati1 = {Lato1Opp, idLatoMO, idLato1M};

    //salvo il triagolo
    unsigned int IdT1 = this->NumberCell2D -1;
    double area1=Area(this->Cell0D[vertici1[0]],this->Cell0D[vertici1[1]],this->Cell0D[vertici1[2]]);
    this->Cell2D.push_back(Triangolo(IdT1,vertici1,lati1,area1));
    this->DeleteCell2D.push_back(false);
    this->NumberCell2D++;
    cout<<"IdT1: "<< IdT1<<endl;


    //adiacenze
    //aggiorno per lato opposto Lato1Opp

    auto it1 = this->Adjacency.find(Lato1Opp);
    if (it1 != this->Adjacency.end())
    {
        // Sostituisci l'elemento nella lista
        it1->second.remove(IdT); // Rimuovi vecchioId
        it1->second.push_back(IdT1); // Inserisci il nuovo IdT2
    };

    // AGGIUNGO IL NUOVO LATO CON LE ADIACENZE idLato1M
    this->Adjacency.insert({idLato1M,{IdT1}});


 //_____________________________________________________________________________

    //CREO NUOVO TRIANGOLO IdT2
    array<unsigned int, 3> vertici2 = {IdV2, IdVO, IdVPM};
    array<unsigned int, 3> lati2 = {Lato2Opp,idLatoMO, idLato2M};
    unsigned int IdT2 = this->NumberCell2D -1;

    double area2=Area(this->Cell0D[vertici2[0]],this->Cell0D[vertici2[1]],this->Cell0D[vertici2[2]]);
    this->Cell2D.push_back(Triangolo(IdT2,vertici2,lati2,area2)); //creo un IdT2
    this->NumberCell2D++;
    this->DeleteCell2D.push_back(false);

    //adiacenze
    auto it2 = this->Adjacency.find(Lato2Opp);
    if (it2 != this->Adjacency.end())
    {
        it2->second.remove(IdT); // Rimuovi vecchioId
        it2->second.push_back(IdT2); // Inserisci il nuovo IdT2
    };

    // AGGIUNGO IL NUOVO LATO CON LE ADIACENZE idLato2M
    this->Adjacency.insert({idLato2M,{IdT2}});
//-------------------------------------------*

    //Aggiorno adiacenze per latoMO che ha sia il IdT2, IdT1
    this->Adjacency.insert({idLatoMO,{IdT1,IdT2}}); //aggiungo l'id del lato e lo associo all'id del triangolo

//==============================================================================================================
    //CASO IN CUI HA TRIANGOLO ADIACENTE//

    auto itAD = this->Adjacency.find(idLE);

    if (itAD!= this->Adjacency.end())
    {
        //iteriamo sugli elementi della lista che ha come chiave il lato lungo e prendiamo quello con id diverso da quello sopra per ricreare i 2 triangoli sotto
       for (const unsigned int& IdTrAd : itAD -> second)
       {
         if (IdTrAd != IdT)
         {
            //devo creare solo il lato tra il punto medio e il nuovo vertice opposto
             IdVO = TriangularMesh::VerticeOpposto(IdTrAd, idLE);
             LatoMO[0] = IdVPM;
             LatoMO[1] = IdVO;
             idLatoMO =this->NumberCell1D -1;
             this->Cell1D.push_back(Lato(idLatoMO, LatoMO));
             this->DeleteCell1D.push_back(false);
             this->NumberCell1D++;

             Lato1Opp = LatoAccanto(IdTrAd, idLE, IdV1);
             Lato2Opp = LatoAccanto(IdTrAd, idLE, IdV2);

             //ADESSO CREO IL TRIANGOLO IdTr1A
             vertici1 = {IdV1, IdVO, IdVPM};
             lati1 = {Lato1Opp, idLatoMO, idLato1M};

             //salvo il triangolo
             unsigned int IdTr1A = this->NumberCell2D -1;
             double area1A = Area(this->Cell0D[vertici1[0]],this->Cell0D[vertici1[1]],this->Cell0D[vertici1[2]]);
             this->Cell2D.push_back(Triangolo(IdTr1A, vertici1, lati1,area1A)); //creo un IdTr1A
             this->DeleteCell2D.push_back(false);
             this->NumberCell2D++;

             //ADIACENZA
             //LATO PICCOLO
             this->Adjacency[idLato1M].push_back(IdTr1A);

             //SOSTIUIAMO VECCHIO ID CON ID Tr1A
             auto it1A = this->Adjacency.find(Lato1Opp);
             if (it1A != this->Adjacency.end())
             {
                    it1A->second.remove(IdTrAd); // Rimuovi vecchioId
                    it1A->second.push_back(IdTr1A); // Inserisci il nuovo IdT
             };
             //_____________________________________________________________________________

             //CREO NUOVO TRIANGOLO IdTrA2
             vertici2 = {IdV2, IdVO, IdVPM};
             lati2 = {Lato2Opp,idLatoMO, idLato2M};

             unsigned int IdTr2A = this->NumberCell2D -1;
             double area2A = Area(this->Cell0D[vertici2[0]],this->Cell0D[vertici2[1]],this->Cell0D[vertici2[2]]);
             this->Cell2D.push_back(Triangolo(IdTr2A, vertici2,lati2,area2A)); //creo un IdT2
             this->NumberCell2D++;
             this->DeleteCell2D.push_back(false);

             //adiacenze
             //lato piccolo
             this->Adjacency[idLato2M].push_back(IdTr2A);

            //SOSTIUIAMO VECCHIO ID CON ID Tr1A
             auto it2A = this->Adjacency.find(Lato2Opp);
             if (it2A != this->Adjacency.end())
             {
                    it2A->second.remove(IdTrAd); // Rimuovi vecchioId
                    it2A->second.push_back(IdTr2A); // Inserisci il nuovo IdT2
              };
             //------
             //ADIACENZA LATO MEDIO
             this->Adjacency.insert({idLatoMO,{IdTr1A,IdTr2A}});
             this->DeleteCell2D[IdTrAd]=true; //ho cambiato da false a true
         };
       };
    };

//=========================================================================
//    //elimino chiave e adiacenze lato lungo
    this->Adjacency.erase(idLE);

    return 0; // zero xk elimini il triangolo bisezionato

    this->DeleteCell2D[IdT]=true;
}
} // chiudo project library


