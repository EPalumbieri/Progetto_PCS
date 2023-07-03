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
    file.open("/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/Cell0Ds.csv");
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
    file.open("/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/Cell1Ds.csv");
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
  file.open("/Users/claudia/Desktop/Progetto/Progetto_PCS/Projects/Raffinamento/Dataset/Test2/Cell2Ds.csv");
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
    if(area>this->tolleranza){
        this->idAreeDaBisezionare.push_back(std::pair<unsigned int,double>(id,area));
    }
    this->Cell2D.push_back(Triangolo(id, vertices, edges, area));
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
    double lunghezza = sqrt(abs((P1.coordinate(0)-P2.coordinate(0))*(P1.coordinate(0)-P2.coordinate(0))+(P1.coordinate(1)-P2.coordinate(1))*(P1.coordinate(1)-P2.coordinate(1))));
    return lunghezza;
 }

//----------------------------------------------------------------------------
 unsigned int TriangularMesh::LatoLungo(const unsigned int &idT)
  {
    array<unsigned int, 3> idLati = this->Cell2D[idT].idL;
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
    Vector2d punto1 = this->Cell0D[Cell1D[idL].idV[0]].coordinate;
    Vector2d punto2 = this->Cell0D[Cell1D[idL].idV[1]].coordinate;

    Vector2d puntoMedio;
    puntoMedio(0) = (punto1[0] + punto2[0])/2;
    puntoMedio(1) = (punto1[1] + punto2[1])/2;

    this->Cell0D.push_back(Punto(this->NumberCell0D, puntoMedio));
    this->NumberCell0D ++;

    return NumberCell0D-1; //-1 perché parto da zero
}
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

{
    // Dato il triangolo T0, trovo il suo lato maggiore L12
    unsigned int L12 = LatoLungo(idT);
    unsigned int PM = PuntoMedio(L12);
    unsigned int P0 = VerticeOpposto(idT, L12);

    this->DeleteCell1D[L12]=true;  // cancello ID lato lungo
    this->DeleteCell2D[idT]=true;  // cancello id triangolo

    // Costruisco LM0
    array<unsigned int, 2> LM0ver;
    LM0ver[0] = PM;
    LM0ver[1] = P0;
    unsigned int LM0 =this->NumberCell1D;
    this->Cell1D.push_back(Lato(LM0, LM0ver));
    this->DeleteCell1D.push_back(false);
    this->NumberCell1D++;

    // Costruisco L1M
    unsigned int P1 = this->Cell1D[L12].idV[0];
    array<unsigned int, 2> L1Mver;
    L1Mver[0] = P1;
    L1Mver[1] = PM;
    unsigned int L1M = this->NumberCell1D;
    this->Cell1D.push_back(Lato(L1M, L1Mver));
    this->DeleteCell1D.push_back(false);
    this->NumberCell1D++;
    
    // Costruisco L2M
    unsigned int P2 = this->Cell1D[L12].idV[1];
    array<unsigned int, 2> L2Mver;
    L2Mver[0] = P2;
    L2Mver[1] = PM;
    unsigned int L2M = this->NumberCell1D;
    this->Cell1D.push_back(Lato(L2M, L2Mver));
    this->DeleteCell1D.push_back(false);
    this->NumberCell1D++;

    unsigned int L1O = LatoAccanto(idT, L12, P1);
    unsigned int L2O = LatoAccanto(idT, L12, P2);

    // Costruisco il triangolo T1
    unsigned int T1 = this->NumberCell2D;
    array<unsigned int, 3> T1vertici = {P1, P0, PM};
    array<unsigned int, 3> T1lati = {L1O, LM0, L1M};
    double T1area=Area(this->Cell0D[T1vertici[0]],this->Cell0D[T1vertici[1]],this->Cell0D[T1vertici[2]]);
    if(T1area>this->tolleranza)
    {
        this->idAreeDaBisezionare.push_back(std::pair<unsigned int,double>(T1,T1area));
    }
    this->Cell2D.push_back(Triangolo(T1,T1vertici,T1lati,T1area));
    this->DeleteCell2D.push_back(false);
    this->NumberCell2D++;

    // Aggiorno l'adiacenza per L1O
    auto it1 = this->Adjacency.find(L1O);
    if (it1 != this->Adjacency.end())
    {
        auto& valueList = it1->second;
        auto itElement = std::find(valueList.begin(), valueList.end(), idT);
        if (itElement != valueList.end())
        {
            valueList.erase(itElement);
            valueList.push_back(T1);
        }
    }

    // Creo l'adiacenza di L1M
    this->Adjacency.insert({L1M,{T1}});

    // Costruisco il triangolo T2
    unsigned int T2 = this->NumberCell2D;
    array<unsigned int, 3> T2vertici = {P2, P0, PM};
    array<unsigned int, 3> T2lati = {L2O,LM0, L2M};
    double T2area=Area(this->Cell0D[T2vertici[0]],this->Cell0D[T2vertici[1]],this->Cell0D[T2vertici[2]]);
    if(T2area>this->tolleranza)
    {
        this->idAreeDaBisezionare.push_back(std::pair<unsigned int,double>(T2,T2area));
    }
    this->Cell2D.push_back(Triangolo(T2,T2vertici,T2lati,T2area));
    this->DeleteCell2D.push_back(false);
    this->NumberCell2D++;

    // Aggiorno adiacenza per L20
    auto it2 = this->Adjacency.find(L2O);
    if (it2 != this->Adjacency.end())
    {
        auto& valueList = it2->second;
        auto itElement = std::find(valueList.begin(), valueList.end(), idT);
        if (itElement != valueList.end())
        {
            valueList.erase(itElement);
            valueList.push_back(T2);
        }
    }

    // Aggiorno l'adiacenza di L2M
    this->Adjacency.insert({L2M,{T2}});

    // Creo adiacenza per LMO
    this->Adjacency.insert({LM0,{T1,T2}});

    // Se T0 ha un triangolo adiacente T3
    unsigned int T3=0;
    auto it3 = this->Adjacency.find(L12);
    if (it3 != this->Adjacency.end())
    {
        if (it3->second.size() > 1)
        {
            if (*(it3->second.begin()) != idT)
            {
                T3=*(it3->second.begin());
            }
            else
            {
                T3=*(++it3->second.begin());
            }

            // Trovo il vertice opposto P3
            unsigned int P3 = VerticeOpposto(T3, L12);

            // Costruisco LM3
            unsigned int LM3 =this->NumberCell1D;
            array<unsigned int, 2> LM3ver;
            LM3ver[0] = PM;
            LM3ver[1] = P3;
            this->Cell1D.push_back(Lato(LM3, LM3ver));
            this->DeleteCell1D.push_back(false);
            this->NumberCell1D++;

            unsigned int L13 = LatoAccanto(T3, L12, P1);
            unsigned int L23 = LatoAccanto(T3, L12, P2);

            // Costruisco il triangolo T4
            unsigned int T4 = this->NumberCell2D;
            array<unsigned int, 3> T4vertici = {P1, P3, PM};
            array<unsigned int, 3> T4lati = {L13, LM3, L1M};
            double T4area = Area(this->Cell0D[T4vertici[0]],this->Cell0D[T4vertici[1]],this->Cell0D[T4vertici[2]]);
            if(T4area>this->tolleranza)
            {
                this->idAreeDaBisezionare.push_back(std::pair<unsigned int,double>(T4,T4area));
            }
            this->Cell2D.push_back(Triangolo(T4, T4vertici, T4lati,T4area));
            this->DeleteCell2D.push_back(false);
            this->NumberCell2D++;

            // Aggiungo T4 all'adiacenza di L1M
            this->Adjacency[L1M].push_back(T4);

            // Aggiorno adiacenza per L13
            auto it4 = this->Adjacency.find(L13);
            if (it4 != this->Adjacency.end())
            {
                auto& valueList = it4->second;
                auto itElement = std::find(valueList.begin(), valueList.end(), T3);
                if (itElement != valueList.end())
                {
                    valueList.erase(itElement);
                    valueList.push_back(T4);
                }
            }

            // Costruisco il triangolo T5
            unsigned int T5 = this->NumberCell2D;
            array<unsigned int, 3> T5vertici = {P2, P3, PM};
            array<unsigned int, 3> T5lati = {L23,LM3, L2M};
            double T5area = Area(this->Cell0D[T5vertici[0]],this->Cell0D[T5vertici[1]],this->Cell0D[T5vertici[2]]);
            if(T5area>this->tolleranza)
            {
                this->idAreeDaBisezionare.push_back(std::pair<unsigned int,double>(T5,T5area));
            }
            this->Cell2D.push_back(Triangolo(T5, T5vertici,T5lati,T5area));
            this->DeleteCell2D.push_back(false);
            this->NumberCell2D++;

            // Aggiungo T5 all'adiacenza di L2M
            this->Adjacency[L2M].push_back(T5);

            // Aggiorno adiacenza per L23
            auto it5 = this->Adjacency.find(L23);
            if (it5 != this->Adjacency.end())
            {
                auto& valueList = it5->second;
                auto itElement = std::find(valueList.begin(), valueList.end(), T3);
                if (itElement != valueList.end())
                {
                    valueList.erase(itElement);
                    valueList.push_back(T5);
                }
            }

            // Creo adiacenza per LM3
            this->Adjacency.insert({LM3,{T4,T5}});

            // Elimino il triangolo T3
            this->DeleteCell2D[T3]=true; //ho cambiato da false a true
        }
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
    for(unsigned int i=0; i<this->Cell1D.size(); i++)
        {
            Lato l=this->Cell1D[i];
            if(!this->DeleteCell1D[i])
            {
            file<<i;
            file<<","<<Cell0D[l.idV[0]].coordinate(0)<<","<<Cell0D[l.idV[0]].coordinate(1)<<","<<Cell0D[l.idV[1]].coordinate(0)<<","<<Cell0D[l.idV[1]].coordinate(1);
            file<<"\n";
            }
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

    file<<"Id,Vertices,Edges\n";
    int i=0;
            for(auto it=this->Cell2D.begin();it!=this->Cell2D.end();it++)
            {
                if(!this->DeleteCell2D[i])
                {
                    Triangolo t=(*it);
                    if(t.idV[0]!=0 && t.idV[1]!=0 && t.idV[1]!=0)
                    file<< i <<","<<t.idV[0]<<","<<t.idV[1]<<","<<t.idV[2]<<","<<t.idL[0]<<","<<t.idL[1]<<","<<t.idL[2]<<"\n";
                }
                i++;
            }
    return true;
}

}

