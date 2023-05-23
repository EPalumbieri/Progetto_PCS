#include "empty_class.hpp"
#include "Eigen/Eigen"
#include <fstream>
#include "map"

using namespace std;
using namespace ProjectLibrary;


int main()
{

  Triangle mesh;

  ImportMesh(mesh);

  return 0;
}
