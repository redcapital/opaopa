#ifndef ISolver_h
#define ISolver_h

#include <vector>
#include "ICellMatrix.h"
#include "Match.h"

namespace PPSolver {

class ISolver
{
  public:
    virtual void setCellMatrix(ICellMatrix& matrix) = 0;
    virtual ICellMatrix& getCellMatrix() = 0;
    virtual std::vector<Match> solve() = 0;
};

}

#endif

