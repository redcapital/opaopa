#include "DummyCellMatrix.h"

using namespace PPSolver;

bool DummyCellMatrix::isEqual(Coords c1, Coords c2)
{
  return this->data[c1.x][c1.y] == this->data[c2.x][c2.y];
}

bool DummyCellMatrix::isEmpty(Coords c)
{
  return this->data[c.x][c.y] == -1;
}

void DummyCellMatrix::setData(std::vector< std::vector<int> >& data) {
  this->data = data;
}

