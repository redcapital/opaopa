#ifndef PixelCellMatrix_h
#define PixelCellMatrix_h

#include <vector>

#include "ICellMatrix.h"
#include "Pixel.h"

namespace PPSolver {

typedef std::vector< std::vector <Pixel> > PixelMatrix;

class PixelCellMatrix : public ICellMatrix
{
  protected:
    static const unsigned SIMILARITY_DIFF = 10;
    PixelMatrix pm;
    unsigned getBaseX(unsigned row);
    unsigned getBaseY(unsigned col);

  public:
    PixelCellMatrix() {};
    static const unsigned PIXEL_MATRIX_WIDTH  = 768;
    static const unsigned PIXEL_MATRIX_HEIGHT = 550;
    static const Coords KEY_PIXEL_COORDS[3];
    static const Coords BLACK_PIXEL_COORDS[4];

    static const unsigned OFFSET_LEFT = 61;
    static const unsigned OFFSET_TOP  = 56;
    static const unsigned CELL_WIDTH  = 40;
    static const unsigned CELL_HEIGHT = 50;

    bool isEqual(Coords c1, Coords c2);
    bool isEmpty(Coords c);
    void setPixelMatrix(PixelMatrix matrix);
    bool isSimilar(Pixel p1, Pixel p2);
};

}

#endif

