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

int PixelCellMatrix::getCellBaseX(int cellX)
{
  return cellX * PixelCellMatrix::CELL_WIDTH + PixelCellMatrix::OFFSET_LEFT;
}

int PixelCellMatrix::getCellBaseY(int cellY)
{
  return cellY * PixelCellMatrix::CELL_HEIGHT + PixelCellMatrix::OFFSET_TOP;
}

bool PixelCellMatrix::isEqual(Coords c1, Coords c2)
{
  // If any of the cells is empty return false
  if (this->isEmpty(c1) || this->isEmpty(c2)) {
    return false;
  }

  std::pair<Coords, Coords> p(c1, c2);
  if (c2 < c1) {
    p.first = c2;
    p.second = c1;
  }
  auto it = this->equalityLookups.find(p);
  if (it == this->equalityLookups.end()) {
    // Miss

    // Iterate over three key pixels of each cell and see whether
    // they are similar.
    // If so, cells are similar
    bool notSimilar = false;
    for (unsigned i = 0; i < 3; i++) {
      if (!this->isSimilar(this->pixelData[c1.x][c1.y][i], this->pixelData[c2.x][c2.y][i])) {
        notSimilar = true;
        break;
      }
    }
    if (notSimilar) {
      this->equalityLookups[p] = false;
    } else {
      this->equalityLookups[p] = true;
    }
  }
  return this->equalityLookups[p];
}

bool PixelCellMatrix::isEmpty(Coords c)
{
  return this->emptyCells[c];
}

void PixelCellMatrix::iteratePixelMatrix()
{
  for (unsigned i = 0; i < ICellMatrix::CELLS_PER_ROW; i++) {
    for (unsigned j = 0; j < ICellMatrix::CELLS_PER_COL; j++) {
      if (this->emptyCells[Coords(i, j)]) {
        continue;
      }
      // Make sure that all 3 pixels are not black
      // (if so, it is empty cell probably)
      bool notBlackAtLeastOne = false;
      for (unsigned t = 0; t < 3; t++) {
        if (!this->isSimilar(this->pixelData[i][j][t], Pixel(0, 0, 0), 60)) {
          notBlackAtLeastOne = true;
          break;
        }
      }
      if (!notBlackAtLeastOne) {
        this->emptyCells[Coords(i, j)] = true;
        continue;
      }
      bool hasOneEqualCellAtLeast = false;
      for (unsigned k = 0; k < ICellMatrix::CELLS_PER_ROW; k++) {
        for (unsigned h = 0; h < ICellMatrix::CELLS_PER_COL; h++) {
          if (i == k && j == h) {
            continue;
          }
          hasOneEqualCellAtLeast = this->isEqual(Coords(i, j), Coords(k, h));
          if (hasOneEqualCellAtLeast) {
            break;
          }
        }
        if (hasOneEqualCellAtLeast) {
          break;
        }
      }
      if (!hasOneEqualCellAtLeast) {
        this->emptyCells[Coords(i, j)] = true;
      }
    }
  }
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
    int x = PixelCellMatrix::BLACK_PIXEL_COORDS[i].x;
    int y = PixelCellMatrix::BLACK_PIXEL_COORDS[i].y;
    if (!this->isSimilar(matrix[x][y], black)) {
      throw InvalidPixelMatrixException("It seems like pixel matrix does not contain data about game board");
    }
  }

  // Process pixel matrix and store only needed data
  // Storing whole array of pixel data is inefficient.

  // Store cell data
  for (unsigned i = 0; i < ICellMatrix::CELLS_PER_ROW; i++) {
    for (unsigned j = 0; j < ICellMatrix::CELLS_PER_COL; j++) {
      int bx = this->getCellBaseX(i);
      int by = this->getCellBaseY(j);
      for (unsigned k = 0; k < 3; k++) {
        int px = bx + PixelCellMatrix::KEY_PIXEL_COORDS[k].x;
        int py = by + PixelCellMatrix::KEY_PIXEL_COORDS[k].y;
        this->pixelData[i][j][k] = matrix[px][py];
      }
    }
  }

  // Clear lookup table
  this->equalityLookups.clear();
  this->emptyCells.clear();
  this->iteratePixelMatrix();
}

bool PixelCellMatrix::isSimilar(Pixel p1, Pixel p2, unsigned maxSimilarityDiff)
{
  std::vector<unsigned> diff = p1.getDiff(p2);
  for (unsigned i = 0; i < 3; i++) {
    if (diff[i] > maxSimilarityDiff) {
      return false;
    }
  }
  return true;
}

