#ifndef ICellMatrix_h
#define ICellMatrix_h

#include "Coords.h"

namespace PPSolver {

class ICellMatrix
{
  public:
    static const unsigned CELLS_PER_ROW = 16;
    static const unsigned CELLS_PER_COL = 9;
    virtual bool isEqual(Coords c1, Coords c2) = 0;
    virtual bool isEmpty(Coords c) = 0;
};

}

#endif
