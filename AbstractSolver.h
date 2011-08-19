#ifndef AbstractSolver_h
#define AbstractSolver_h

#include "ISolver.h"

namespace PPSolver {

class AbstractSolver : public ISolver
{
  protected:
    ICellMatrix& matrix;

  public:
    AbstractSolver(ICellMatrix& _matrix) : matrix(_matrix) {};
    virtual void setCellMatrix(ICellMatrix& matrix);
    virtual ICellMatrix& getCellMatrix();
    virtual std::vector<Match> solve() = 0;
};

}

#endif

