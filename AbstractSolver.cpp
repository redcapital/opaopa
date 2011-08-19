#include "AbstractSolver.h"

using namespace PPSolver;

void AbstractSolver::setCellMatrix(ICellMatrix& matrix)
{
  this->matrix = matrix;
}

ICellMatrix& AbstractSolver::getCellMatrix()
{
  return this->matrix;
}

