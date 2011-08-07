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

unsigned PixelCellMatrix::getCellBaseX(unsigned cellX)
{
  return cellX * PixelCellMatrix::CELL_WIDTH + PixelCellMatrix::OFFSET_LEFT;
}

unsigned PixelCellMatrix::getCellBaseY(unsigned cellY)
{
  return cellY * PixelCellMatrix::CELL_HEIGHT + PixelCellMatrix::OFFSET_TOP;
}

/**
 * Retrieves coordinates of pixel from cell background.
 * Background has four key pixels. Parameter whichCorner determines
 * which pixel should be handled:
 *  0 - upper left
 *  1 - upper right
 *  2 - bottom right
 *  3 - bottom left
 */
Coords PixelCellMatrix::getCellBackgroundCoords(Coords cellCoords, unsigned whichCorner)
{
  unsigned bx = this->getCellBaseX(cellCoords.x);
  unsigned by = this->getCellBaseY(cellCoords.y);
  switch (whichCorner) {
    case 0:
      bx += PixelCellMatrix::BACKGROUND_CORNER_OFFSET;
      by += PixelCellMatrix::BACKGROUND_CORNER_OFFSET;
      break;
    case 1:
      bx += PixelCellMatrix::CELL_WIDTH - PixelCellMatrix::BACKGROUND_CORNER_OFFSET;
      by += PixelCellMatrix::BACKGROUND_CORNER_OFFSET;
      break;
    case 2:
      bx += PixelCellMatrix::CELL_WIDTH - PixelCellMatrix::BACKGROUND_CORNER_OFFSET;
      by += PixelCellMatrix::CELL_HEIGHT - PixelCellMatrix::BACKGROUND_CORNER_OFFSET;
      break;
    case 3:
      bx += PixelCellMatrix::BACKGROUND_CORNER_OFFSET;
      by += PixelCellMatrix::CELL_HEIGHT - PixelCellMatrix::BACKGROUND_CORNER_OFFSET;
      break;
  }
  return Coords(bx, by);
}

bool PixelCellMatrix::isEqual(Coords c1, Coords c2)
{
  // Iterate over three key pixels of each cell and see whether
  // they are similar.
  // If so, cells are similar
  for (unsigned i = 0; i < 3; i++) {
    if (!this->isSimilar(this->pixelData[c1.x][c1.y][i], this->pixelData[c2.x][c2.y][i])) {
      return false;
    }
  }
  return true;
}

bool PixelCellMatrix::isEmpty(Coords c)
{
  // Iterate over four corner pixels of cell background.
  // Cell is considered as NOT empty if both 2 conditions are met:
  // - four points are similar
  //   because background is monotone
  // - each of them are lighter than RGB(150, 150, 150)
  //   because it can be monotone but dark. Dark background is a "hole"
  //   on game board, meaning it's an empty cell
  for (unsigned i = 1; i < 4; i++) {
    if (!this->isSimilar(bgData[c.x][c.y][0], bgData[c.x][c.y][i])) {
      return true;
    }
  }
  for (unsigned i = 0; i < 4; i++) {
    if (bgData[c.x][c.y][i].r <= 150
        && bgData[c.x][c.y][i].g <= 150
        && bgData[c.x][c.y][i].b <= 150
    ) {
      return true;
    }
  }
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

  // Process pixel matrix and store only needed data
  // Storing whole array of pixel data is inefficient.

  // Store cell data
  for (unsigned i = 0; i < ICellMatrix::CELLS_PER_ROW; i++) {
    for (unsigned j = 0; j < ICellMatrix::CELLS_PER_COL; j++) {
      unsigned bx = this->getCellBaseX(i);
      unsigned by = this->getCellBaseY(j);
      for (unsigned k = 0; k < 3; k++) {
        unsigned px = bx + PixelCellMatrix::KEY_PIXEL_COORDS[k].x;
        unsigned py = by + PixelCellMatrix::KEY_PIXEL_COORDS[k].y;
        this->pixelData[i][j][k] = matrix[px][py];
      }
    }
  }

  // Store backgroun color data
  for (unsigned i = 0; i < ICellMatrix::CELLS_PER_ROW; i++) {
    for (unsigned j = 0; j < ICellMatrix::CELLS_PER_COL; j++) {
      for (unsigned k = 0; k < 4; k++) {
        Coords pc = this->getCellBackgroundCoords(Coords(i, j), k);
        this->bgData[i][j][k] = matrix[pc.x][pc.y];
      }
    }
  }
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

