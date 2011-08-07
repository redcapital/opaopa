#ifndef DummyCellMatrix_h
#define DummyCellMatrix_h

#include <vector>
#include "ICellMatrix.h"

namespace PPSolver {

class DummyCellMatrix : public ICellMatrix
{
  protected:
    std::vector< std::vector<int> > data;

  public:
    void setData(std::vector< std::vector<int> >& data);
    bool isEqual(Coords c1, Coords c2);
    bool isEmpty(Coords c);
};

}

#endif

