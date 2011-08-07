#ifndef ICellMatrix_h
#define ICellMatrix_h

#include "Coords.h"

namespace PPSolver {

class ICellMatrix
{
  public:
    virtual bool isEqual(Coords c1, Coords c2) = 0;
    virtual bool isEmpty(Coords c) = 0;
};

}

#endif
