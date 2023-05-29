#include "struct.hpp"
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
//#include "map"

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{
//_________________________________________________________

bool ImportCell0Ds(TriangularMesh& mesh)
{
    // Apro il file
    ifstream file;

    file.open("C:/Users/marti/OneDrive/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv");
    //C:/Users/marti/OneDrive/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv
    if (file.fail())
    {
    cerr<<"file open failed 0"<< endl;
    return false;
    }

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

  }
  file.close();
  return true;
}
// ***************************************************************************
bool ImportCell1Ds(TriangularMesh& mesh)
{
    // Apro il file
    ifstream file;

    file.open("C:/Users/marti/OneDrive/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv");

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

  }

  file.close();

  return true;
}


// ***************************************************************************
bool ImportCell2Ds(TriangularMesh& mesh)
{ 
  // Apro il file
  ifstream file;

  file.open("C:/Users/marti/OneDrive/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv");


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
         if (mesh.Adjacency.find(edges[i]) == mesh.Adjacency.end()) //se non c'è nella mappa
              mesh.Adjacency.insert({edges[i],{id}}); //aggiungo l'id del lato e lo associo all'id del triangolo
          else
              mesh.Adjacency[edges[i]].push_back(id); // aggiungo l'id del triangolo
    }
    mesh.Cell2DId.push_back(id);
    mesh.Cell2DVertices.push_back(vertices);
    mesh.Cell2DEdges.push_back(edges);

  }
  file.close();
  return true;
}

//____________________________________________________

double Area(TriangularMesh& mesh,unsigned int& idT)
{
  array<unsigned int, 3> vertici = mesh.Cell2DVertices[idT];

  Vector2d coord1 = mesh.Cell0DCoordinates[vertici[0]];
  Vector2d coord2 =mesh.Cell0DCoordinates[vertici[1]];
  Vector2d coord3 = mesh.Cell0DCoordinates[vertici[2]];

  double area =(coord1[0] * (coord2[1] - coord3[1]) +coord2[0] * (coord3[1] - coord1[1]) + coord3[0] * (coord1[1] - coord2[1]));
  area= abs(area);
  area= area*0.5 ; // anche questo è inutile perché tanto l'ordinamento lo fa giusto anche senza

  return area;
}

//____________________________________________________

double LunghezzaLato(TriangularMesh& mesh,unsigned int idL)
 {
    Vector2i vertici = mesh.Cell1DVertices[idL]; // array<unsigned int, 2> vertici = mesh.Cell1DVertices[idL];
    Vector2d coord1 = mesh.Cell0DCoordinates[vertici[0]]; // array<double, 2> = mesh.Cell0DCoordinates[vertici[0]];
    Vector2d coord2 = mesh.Cell0DCoordinates[vertici[1]]; // stesso di su

    // io lo calcolerei senza radice per il costo computazionale (tanto non fa differenza)
    double lunghezza = sqrt((coord2[0] - coord1[0]) * (coord2[0] - coord1[0]) + (coord2[1] - coord1[1]) * (coord2[1] - coord1[1]));

    return lunghezza;
  }

 //----------------------------------------------------------------------------*

 unsigned int LatoLungo(TriangularMesh& mesh,unsigned int idT)
  {
    array<unsigned int, 3> idLati = mesh.Cell2DEdges[idT];
    // Calcola la lunghezza di ogni lato e determina l'id del lato più lungo
    double lunghezzaMax = 0.0;
    unsigned int idLatoLungo = 0; // qui 0 non va bene perché corrisponde ad un id >>>> unsigned int idLatoLungo;

    for (unsigned int i = 0; i < 3; i++)    {
       unsigned int idLato = idLati[i];
       double lunghezza = LunghezzaLato(mesh, idLato);
       //calcolo il lato massimo
       if (lunghezza > lunghezzaMax)    {
          lunghezzaMax = lunghezza;
          idLatoLungo = idLato;
       }
    }
    return idLatoLungo;
  }
//*-----------------------------------------------

 unsigned int VerticeOpposto(TriangularMesh& mesh,unsigned int idT,unsigned int idL)
  {
   Vector2i idVertici = mesh.Cell1DVertices[idL]; // array<unsigned int, 2> idVertici = mesh.Cell1DVertices[idL];
   unsigned int vertice1 = idVertici[0]; //non so a che serve, estrae i due vertici separatamente
   unsigned int vertice2 = idVertici[1];

   // Ottieni gli id dei tre vertici del triangolo idT considerato
   array<unsigned int, 3> idVerticiTriangolo = mesh.Cell2DVertices[idT];

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

 // creo funzione punto medio
unsigned int PuntoMedio(TriangularMesh& mesh,unsigned int idL)
{
    Vector2i vertici = mesh.Cell1DVertices[idL]; // array<unsigned int, 2> vertici = mesh.Cell1DVertices[idL];
    Vector2d punto1 = mesh.Cell0DCoordinates[vertici[0]]; // array<double, 2> = mesh.Cell0DCoordinates[vertici[0]];
    Vector2d punto2 = mesh.Cell0DCoordinates[vertici[1]];

    Vector2d pm;
    pm(0) = (punto1[0] + punto2[0])/2;
    pm(1) = (punto1[1] + punto2[1])/2;

    mesh.Cell0DCoordinates.push_back(pm);
    mesh.Cell0DId.push_back(mesh.NumberCell0D); //perché
    mesh.NumberCell0D++;

    return mesh.NumberCell0D-1;
}

//* --------------------------------------------- *
// funzione di bisezione del lato lungo
bool Bisezione(TriangularMesh& mesh,unsigned int IdT)
{
   unsigned int IdLE = LatoLungo(mesh, IdT);
   unsigned int IdVPM = PuntoMedio(mesh, IdLE);
   unsigned int IdVO = VerticeOpposto(mesh, IdT, IdLE);

    Vector2i LatoMO;
    LatoMO(0) = IdVPM;
    LatoMO(1) = IdVO;
    unsigned int idLatoMO =mesh.NumberCell1D;

    mesh.Cell1DId.push_back(idLatoMO);
    mesh.Cell1DVertices.push_back(LatoMO);
    mesh.NumberCell1D++;

    // creo i lati piccoli
    Vector2i IdVertici = mesh.Cell1DVertices[IdLE];

    unsigned int IdV1 = IdVertici(0);
    Vector2i Lato1Mvertici;
    Lato1Mvertici(0) = IdV1;
    Lato1Mvertici(1) = IdVPM;
    unsigned int idLato1M =mesh.NumberCell1D ;

    // mi salvo ID per cell2d edge

    mesh.Cell1DId.push_back(idLato1M);
    mesh.Cell1DVertices.push_back(Lato1Mvertici);
    mesh.NumberCell1D++;

    unsigned int IdV2 = IdVertici(1);
    Vector2i Lato2Mvertici;
    Lato2Mvertici(0) = IdV2;
    Lato2Mvertici(1) = IdVPM;
    unsigned int idLato2M =mesh.NumberCell1D;

    mesh.Cell1DId.push_back(idLato2M);
    mesh.Cell1DVertices.push_back(Lato2Mvertici);
    mesh.NumberCell1D++;



    //cerco id di Lato1Opp e Lato2Opp
    // cerco i vertici del triangolo
    array<unsigned int, 3> lati = mesh.Cell2DEdges[IdT]; // so gli id dei lati


    unsigned int Lato1Opp;
    unsigned int Lato2Opp;
    for (unsigned int i = 0; i < 3; i++)
    {   // se il lato preso contiene v1 e non è le, lo prendo, altrimenti prendo l'altro
        if (lati[i] != IdLE){ //cioè vado avanti

        Vector2i vertici = mesh.Cell1DVertices[lati[i]];
        if (static_cast<unsigned int> (vertici[0]) == IdV1 || static_cast<unsigned int> (vertici[1]) == IdV1)
        {
            Lato1Opp = lati[i]; //legato a 1m
        }
        if ( static_cast<unsigned int> (vertici[0]) == IdV2 || static_cast<unsigned int> (vertici[1]) == IdV2)
        {
            Lato2Opp = lati[i];//legato a 2m
        }
        };
    }

//_____________________________________________________________________________
    //ADESSO CREO IL TRIANGOLO IdT1
    array<unsigned int, 3> vertici1 = {IdV1, IdVO, IdVPM};
    array<unsigned int, 3> lati1 = {Lato1Opp, idLatoMO, idLato1M};


    //salvo il triagolo

    unsigned int IdT1 = mesh.NumberCell2D;
    mesh.Cell2DId.push_back(IdT1); //creo un IdT1
    mesh.Cell2DVertices.push_back(vertici1);
    mesh.Cell2DEdges.push_back(lati1);
    cout<<"IdT1: "<< IdT1<<endl;


    //adiacenze
    //aggiorno per lato opposto Lato1Opp

    auto it1 = mesh.Adjacency.find(Lato1Opp);
    if (it1 != mesh.Adjacency.end())
    {
        list<unsigned int>& lista1 = it1->second;

        // Sostituisci l'elemento nella lista

        lista1.remove(IdT); // Rimuovi vecchioId
        lista1.push_back(IdT1); // Inserisci il nuovo IdT2
    };

    // AGGIUNGO IL NUOVO LATO CON LE ADIACENZE idLato1M
    mesh.Adjacency.insert({idLato1M,{IdT1}});

    mesh.NumberCell2D++;
 //_____________________________________________________________________________

    //CREO NUOVO TRIANGOLO IdT2
    array<unsigned int, 3> vertici2 = {IdV2, IdVO, IdVPM};
    array<unsigned int, 3> lati2 = {Lato2Opp,idLatoMO, idLato2M};
    unsigned int IdT2 = mesh.NumberCell2D;
    cout<<"IdT1: "<< IdT1<<endl;

    mesh.Cell2DId.push_back(IdT2); //creo un IdT2
    mesh.Cell2DVertices.push_back(vertici2);
    mesh.Cell2DEdges.push_back(lati2);

    //adiacenze
    //aggiorno per lato opposto Lato2Opp

    auto it2 = mesh.Adjacency.find(Lato2Opp);
    if (it2 != mesh.Adjacency.end())
    {
        list<unsigned int>& lista1 = it2->second;

        // Sostituisci l'elemento nella lista

        lista1.remove(IdT); // Rimuovi vecchioId
        lista1.push_back(IdT2); // Inserisci il nuovo IdT2
    };

    // AGGIUNGO IL NUOVO LATO CON LE ADIACENZE idLato2M
    mesh.Adjacency.insert({idLato2M,{IdT2}});
    mesh.NumberCell2D++;
//-------------------------------------------*

    //Aggiorno adiacenze per latoMO che ha sia il IdT2, IdT1
    mesh.Adjacency.insert({idLatoMO,{IdT1,IdT2}}); //aggiungo l'id del lato e lo associo all'id del triangolo

//==============================================================================================================
    //CASO IN CUI A TRIANGOLO ADIACENTE//

    auto itAD = mesh.Adjacency.find(IdLE);

    if (itAD!= mesh.Adjacency.end())
    {
        // La chiave idLE esiste nella mappa
        const list<unsigned int>& listaIdTrAd = itAD -> second;

        //iteriamo sugli elementi della lista che ha come chiave il lato lungo e prendiamo quello con id diverso da quello sopra per ricreare i 2 triangoli sotto

       for (const unsigned int& IdTrAd : listaIdTrAd)
          {
             if (IdTrAd != IdT)
             {
                //devo creare solo il lato tra il punto medio e il nuovo vertice opposto
                 IdVO = VerticeOpposto(mesh, IdTrAd, IdLE);
                 LatoMO(0) = IdVPM;
                 LatoMO(1) = IdVO;
                 idLatoMO =mesh.NumberCell1D;
                 mesh.Cell1DId.push_back(idLatoMO);
                 mesh.Cell1DVertices.push_back(LatoMO);
                 mesh.NumberCell1D++;

                 // ci interessano i lati opposti  per creare i  triangoli IdTr1A e IdTr1A
                 lati = mesh.Cell2DEdges[IdTrAd];

                 for (unsigned int i = 0; i < 3; i++)
                 {   // se il lato preso contiene v1 e non è le, lo prendo, altrimenti prendo l'altro
                    if (lati[i] != IdLE)
                        {
                            //cioè vado avanti
                             Vector2i vertici = mesh.Cell1DVertices[lati[i]];
                            if (static_cast<unsigned int> (vertici[0]) == IdV1 || static_cast<unsigned int> (vertici[1]) == IdV1)
                            {
                                Lato1Opp = lati[i];// legato a 1m
                            }
                            if ( static_cast<unsigned int> (vertici[0]) == IdV2 || static_cast<unsigned int> (vertici[1]) == IdV2)
                            {
                                Lato2Opp = lati[i];// legato a 2M
                            }
                        };
                 };

                 //ADESSO CREO IL TRIANGOLO IdTr1A
                 vertici1 = {IdV1, IdVO, IdVPM};
                 lati1 = {Lato1Opp, idLatoMO, idLato1M};


                 //salvo il triangolo

                 unsigned int IdTr1A = mesh.NumberCell2D;
                 mesh.Cell2DId.push_back(IdTr1A); //creo un IdTr1A
                 mesh.Cell2DVertices.push_back(vertici1);
                 mesh.Cell2DEdges.push_back(lati1);
                 mesh.NumberCell2D++;

                 //ADIACENZA
                 //LATO PICCOLO
                 mesh.Adjacency[idLato1M].push_back(IdTr1A);

                 //SOSTIUIAMO VECCHIO ID CON ID Tr1A
                 auto it1A = mesh.Adjacency.find(Lato1Opp);
                 if (it1A != mesh.Adjacency.end())
                 {
                        list<unsigned int>& lista1 = it1A->second;

                        // Sostituisci l'elemento nella lista

                        lista1.remove(IdTrAd); // Rimuovi vecchioId
                        lista1.push_back(IdTr1A); // Inserisci il nuovo IdT
                 };
                 //_____________________________________________________________________________

                 //CREO NUOVO TRIANGOLO IdTrA2
                 vertici2 = {IdV2, IdVO, IdVPM};
                 lati2 = {Lato2Opp,idLatoMO, idLato2M};

                 unsigned int IdTr2A = mesh.NumberCell2D;

                 mesh.Cell2DId.push_back(IdTr2A); //creo un IdT2
                 mesh.Cell2DVertices.push_back(vertici2);
                 mesh.Cell2DEdges.push_back(lati2);

                 //adiacenze
                 //lato piccolo
                 mesh.Adjacency[idLato2M].push_back(IdTr2A);

                //SOSTIUIAMO VECCHIO ID CON ID Tr1A
                 auto it2A = mesh.Adjacency.find(Lato2Opp);
                 if (it2A != mesh.Adjacency.end())
                 {
                        list<unsigned int>& lista1 = it2A->second;

                        // Sostituisci l'elemento nella lista

                        lista1.remove(IdTrAd); // Rimuovi vecchioId
                        lista1.push_back(IdTr2A); // Inserisci il nuovo IdT2
                  };
                 //------
                 //ADIACENZA LATO MEDIO
                 mesh.Adjacency.insert({idLatoMO,{IdTr1A,IdTr2A}});
                 mesh.NumberCell2D++;
             };


           };
    };

//=========================================================================
//    //elimino chiave e adiacenze lato lungo
    mesh.Adjacency.erase(IdLE);

    return 0; // zero xk elimini il triangolo bisezionato

}

};


