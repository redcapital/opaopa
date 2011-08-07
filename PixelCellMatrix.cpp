#include <vector>
#include <iostream>
#include "PixelCellMatrix.h"
#include "InvalidDimensionException.h"
#include "InvalidPixelMatrixException.h"

using namespace PPSolver;

const Coords PixelCellMatrix::KEY_PIXEL_COORDS[3] = {
  Coords(10, 20),
  Coords(20, 30),
  Coords(30, 40)
};
const Coords PixelCellMatrix::BLACK_PIXEL_COORDS[4] = {
  Coords(25, 56),
  Coords(732, 59),
  Coords(30, 506),
  Coords(735, 512)
};

unsigned PixelCellMatrix::getBaseX(unsigned row)
{
  return row * PixelCellMatrix::CELL_WIDTH + PixelCellMatrix::OFFSET_LEFT;
}

unsigned PixelCellMatrix::getBaseY(unsigned col)
{
  return col * PixelCellMatrix::CELL_HEIGHT + PixelCellMatrix::OFFSET_TOP;
}

bool PixelCellMatrix::isEqual(Coords c1, Coords c2)
{
  //unsigned bx1, bx2, by1, by2;
  //bx1 = this->getBaseX(row1);
  //by1 = this->getBaseY(col1);
  //bx2 = this->getBaseX(row2);
  //by2 = this->getBaseY(col2);
  return false;
}

bool PixelCellMatrix::isEmpty(Coords c)
{
  return false;
}

void PixelCellMatrix::setPixelMatrix(PixelMatrix matrix)
{
  // Check dimensions of pixel matrix
  if (matrix.size() != PixelCellMatrix::PIXEL_MATRIX_WIDTH) {
    throw InvalidDimensionException("PixelMatrix has invalid height");
  }
  if (matrix.begin()->size() != PixelCellMatrix::PIXEL_MATRIX_HEIGHT) {
    throw InvalidDimensionException("Pixel Matrix has invalid width");
  }
  // Check colors of particular pixels. They must be similar to black.
  Pixel black(0, 0, 0);
  for (unsigned i = 0; i < 4; i++) {
    unsigned x = PixelCellMatrix::BLACK_PIXEL_COORDS[i].x;
    unsigned y = PixelCellMatrix::BLACK_PIXEL_COORDS[i].y;
    if (!this->isSimilar(matrix[x][y], black)) {
      throw InvalidPixelMatrixException("It seems like pixel matrix does not contain data about game board");
    }
  }
  this->pm = matrix;
}

bool PixelCellMatrix::isSimilar(Pixel p1, Pixel p2)
{
  std::vector<unsigned> diff = p1.getDiff(p2);
  for (unsigned i = 0; i < 3; i++) {
    if (diff[i] > PixelCellMatrix::SIMILARITY_DIFF) {
      return false;
    }
  }
  return true;
}

