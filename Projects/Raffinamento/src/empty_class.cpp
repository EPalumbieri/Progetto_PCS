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
  // this->Cell0DCoordinates.reserve(this->NumberCell0D);

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
  //this->Cell1DVertices.reserve(this->NumberCell1D);
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
//    this->DeleteCell1D.push_back(true);
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

 double TriangularMesh::LunghezzaLato(const Punto &P1, const Punto &P2)
 {
    double lunghezza = sqrt(abs((P1.coordinate(0)-P2.coordinate(0))*(P1.coordinate(0)-P2.coordinate(0))-(P1.coordinate(1)-P2.coordinate(1))*(P1.coordinate(1)-P2.coordinate(1))));
    return lunghezza;
 }

//----------------------------------------------------------------------------
 unsigned int TriangularMesh::LatoLungo(const unsigned int &idT)
  {
    array<unsigned int, 3> idLati = Cell2D[idT].idL;
    // Calcola la lunghezza di ogni lato e determina l'id del lato più lungo
    double lunghezzaMax = 0.0;
    unsigned int idLatoLungo = 0; // qui 0 non va bene perché corrisponde ad un id >>>> unsigned int idLatoLungo;

    for (unsigned int i = 0; i < 3; i++)
    {
       unsigned int idL = idLati[i];
       double lunghezza = LunghezzaLato(Cell0D[Cell1D[idL].idV[0]],Cell0D[Cell1D[idL].idV[1]]);

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
   array<unsigned int, 2> idVertici = this->Cell1D[idL].idV; // array<unsigned int, 2> idVertici = this->Cell1DVertices[idL];
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
    Vector2d punto1 = Cell0D[Cell1D[idL].idV[0]].coordinate;
    Vector2d punto2 = Cell0D[Cell1D[idL].idV[1]].coordinate;

    Vector2d puntoMedio;
    puntoMedio(0) = (punto1[0] + punto2[0])/2;
    puntoMedio(1) = (punto1[1] + punto2[1])/2;

    Cell0D.push_back(Punto(NumberCell0D, puntoMedio));
    NumberCell0D ++;

    return NumberCell0D-1; //-1 perché parto da zero
}

//unsigned int TriangularMesh::PuntoMedio(Lato& L)
//{
//    Punto P1;
//    Punto P2;

//    P1.idP = L.idV(0);
//    P2.idP = L.idV(1);

//    Vector2d puntoMedio;
//    puntoMedio(0) = (P1.coordinate(0) + P2.coordinate(0))/2;
//    puntoMedio(1) = (P1.coordinate(1) + P2.coordinate(1))/2;

//    Cell0D.push_back(Punto(NumberCell0D, puntoMedio));
//    NumberCell0D ++;
//    return NumberCell0D -1;
//}
//----------------------------------------------------------------------------
unsigned int TriangularMesh::LatoAccanto(const unsigned int& idT,const unsigned int& idL, const unsigned int& idP)
{
    array<unsigned int, 3> lati = this->Cell2D[idT].idL;
    unsigned int LatoAccanto=0;
    for (unsigned int i = 0; i < 3; i++)
    {   // se il lato preso contiene v1 e non è le, lo prendo, altrimenti prendo l'altro
        if (lati[i] != idL)
        { //vado avanti
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

{ cout<< "bisezione per : "<< idT<< endl;

   unsigned int idLL = LatoLungo(idT);
   unsigned int idVM = PuntoMedio(idLL);
   cout<<"IDVM"<<idVM<<endl;
   unsigned int idVO = VerticeOpposto(idT, idLL);
   cout<<idVO<<endl;
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
    unsigned int IdTA=0;
    auto itA = this->Adjacency.find(idLL);
    if (itA != this->Adjacency.end()) //controlla che l'elemento sia stato trovato correttamente
    {
 //iteriamo sugli elementi della lista che ha come chiave il lato lungo e prendiamo quello con id diverso da quello sopra per ricreare i 2 triangoli sotto
    if (itA->second.size() > 1)
        {
         if (*(itA->second.begin()) != idT)
         {
            IdTA=*(itA->second.begin());
         }else{
            IdTA=*(++itA->second.begin());
         }
                cout<<"LATO LUNGO: " << idLL<< " triangolo adiacente: ";
                cout<< IdTA<<endl;
            //devo creare solo il lato tra il punto medio e il nuovo vertice opposto
             idVO = VerticeOpposto(IdTA, idLL);
             cout<<idVO<<endl;
             LatoMO[0] = idVM;
             LatoMO[1] = idVO;
             // unsigned int idLatoMO =this->NumberCell1D;
             idLatoMO =this->NumberCell1D;
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

//=========================================================================
//    //elimino chiave e adiacenze lato lungo
    this->Adjacency.erase(idLL);

    this->DeleteCell2D[idT]=true;

    return 0; // zero xk elimini il triangolo bisezionato

    // zero xk elimini il triangolo bisezionato

    this->DeleteCell2D[idT]=true;
    return 0;

}
    return 0;
}

bool TriangularMesh::ExportMesh(string file0D, string file1D, string file2D)
{
    if(!TriangularMesh::ExportCell0Ds(file0D))
    {
      cerr<<"FailedExportCell0";
      return false;
    }

    if(!TriangularMesh::ExportCell1Ds(file1D))
    {
      cerr<<"FailedExportCell1";
      return false;
    }

    if(!TriangularMesh::ExportCell2Ds(file2D))
    {
      cerr<<"FailedExportCell2";
      return false;
    }

    return true;

}

bool TriangularMesh::ExportCell0Ds(string nomeFile)
{
    ofstream file;
    file.open(nomeFile);

    if(file.fail())
    {
      cerr<<"wrong file";
      return false;

    }

    file<<"Id,X,Y\n";
            for(unsigned int i=0;i<this->Cell0D.size();i++)
            {
             Punto p=this->Cell0D[i];
             file<<i<<","<<p.coordinate(0)<<","<<p.coordinate(1)<<"\n";
            }
     return true;
}

bool TriangularMesh::ExportCell1Ds(string nomeFile)
{
    ofstream file;
    file.open(nomeFile);

    if(file.fail())
    {
      cerr<<"wrong file";
      return false;
    }

    file<<"IdL,X1,Y1,X2,Y2\n";
    // int i=0;
    for(unsigned int i=0;i<this->Cell1D.size();i++)
        {
            Lato l=this->Cell1D[i];
            if(!this->DeleteCell1D[i]){
            file<<i;
            file<<","<<Cell0D[l.idV[0]].coordinate(0)<<","<<Cell0D[l.idV[0]].coordinate(1)<<","<<Cell0D[l.idV[1]].coordinate(0)<<","<<Cell0D[l.idV[1]].coordinate(1);
            file<<"\n";
            }
        //i++;
    }
     return true;
}

bool TriangularMesh::ExportCell2Ds(string nomeFile)
{
    ofstream file;
    file.open(nomeFile);

    if(file.fail())
    {
      cerr<<"wrong file ";
      return false;
    }

    file<<"Id Vertices Edges\n";
    int i=0;
            for(auto it=this->Cell2D.begin();it!=this->Cell2D.end();it++)
            {
             if(!this->DeleteCell2D[i]){
             Triangolo t=(*it);
             if(t.idV[0]!=0 && t.idV[1]!=0 && t.idV[1]!=0)
               file<<i<<" "<<t.idV[0]<<" "<<t.idV[1]<<" "<<t.idV[2]
                    <<" "<<t.idL[0]<<" "<<t.idL[1]<<" "<<t.idL[2]<<"\n";
             }
             i++;
            }
     return true;
}
}

