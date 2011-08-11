#include <cxxtest/TestSuite.h>
#include "../PixelCellMatrix.h"
#include "../InvalidDimensionException.h"
#include "../InvalidPixelMatrixException.h"
#include <iostream>

using namespace PPSolver;

class PixelCellMatrixTest : public CxxTest::TestSuite
{
  private:
    PixelMatrix pm;

  public:
    void setUp()
    {
      this->pm.clear();
      this->pm.resize(PixelCellMatrix::PIXEL_MATRIX_WIDTH);
      for (auto i = this->pm.begin(); i != this->pm.end(); i++) {
        i->resize(PixelCellMatrix::PIXEL_MATRIX_HEIGHT);
      }
    }

    void _makeEverythingEqual()
    {
      int sx = PixelCellMatrix::OFFSET_LEFT;
      int sy = PixelCellMatrix::OFFSET_TOP;
      int ex = sx + PixelCellMatrix::CELL_WIDTH * ICellMatrix::CELLS_PER_ROW;
      int ey = sy + PixelCellMatrix::CELL_HEIGHT * ICellMatrix::CELLS_PER_COL;
      for (auto i = sx; i < ex; i++) {
        for (auto j = sy; j != ey; j++) {
          this->pm[i][j].r =
          this->pm[i][j].g =
          this->pm[i][j].b = 200;
        }
      }
    }

    void _makeCellBlack(Coords cellCoords)
    {
      PixelCellMatrix pcm;
      int bx = pcm.getCellBaseX(cellCoords.x);
      int by = pcm.getCellBaseY(cellCoords.y);
      for (unsigned i = bx; i < PixelCellMatrix::CELL_WIDTH + bx; i++) {
        for (unsigned j = by; j < PixelCellMatrix::CELL_HEIGHT + by; j++) {
          this->pm[i][j].r = this->pm[i][j].g = this->pm[i][j].b = 0;
        }
      }
    }

    void _putPixel(unsigned cellX, unsigned cellY, Pixel p, unsigned keyCoordNum)
    {
      PixelCellMatrix pcm;
      unsigned px = pcm.getCellBaseX(cellX) + PixelCellMatrix::KEY_PIXEL_COORDS[keyCoordNum].x;
      unsigned py = pcm.getCellBaseY(cellY) + PixelCellMatrix::KEY_PIXEL_COORDS[keyCoordNum].y;
      this->pm[px][py] = p;
    }

    void testWrongColorsShouldThrowException()
    {
      this->pm[25][56].r = 11;
      this->pm[25][56].g = 11;
      this->pm[25][56].b = 11;
      PixelCellMatrix pcm;
      TS_ASSERT_THROWS(pcm.setPixelMatrix(this->pm), InvalidPixelMatrixException);

      this->pm[25][56].r = 0;
      this->pm[25][56].g = 0;
      this->pm[25][56].b = 0;
      this->pm[30][506].r = 11;
      TS_ASSERT_THROWS(pcm.setPixelMatrix(this->pm), InvalidPixelMatrixException);
    }

    void testRightColorsShouldNotThrowException()
    {
      PixelCellMatrix pcm;
      TS_ASSERT_THROWS_NOTHING(pcm.setPixelMatrix(this->pm));

      this->pm[25][56].r = 8;
      this->pm[25][56].r = 10;
      this->pm[25][56].r = 9;
      TS_ASSERT_THROWS_NOTHING(pcm.setPixelMatrix(this->pm));
    }

    void testIsSimilarShouldBeTrue()
    {
      PixelCellMatrix pcm;
      Pixel a(255, 255, 255);
      Pixel b(245, 246, 250);
      TS_ASSERT(pcm.isSimilar(a, b));
    }

    void testIsSimilarShouldBeFalse()
    {
      PixelCellMatrix pcm;
      Pixel a(100, 100, 100);
      Pixel b(100, 100, 89);
      TS_ASSERT(!pcm.isSimilar(a, b));
    }

    void testWrongMatrixDimensionsShouldThrowException()
    {
      PixelMatrix pm;
      pm.resize(1);
      PixelCellMatrix pcm;
      TS_ASSERT_THROWS(pcm.setPixelMatrix(pm), InvalidDimensionException);
    }

    void testIsEqualShouldBeTrue()
    {
      PixelCellMatrix pcm;
      // Remember, here Coords represent coordinates of cells, not pixels.
      // Coordinates are 0-based of course. As everything else in C/C++ :)
      Coords c1(1, 1);
      Coords c2(7, 8);
      this->_makeEverythingEqual();

      // Check 1, 1 and 7, 8 cells. Use first key pixel
      // a and b are similar pixels.
      Pixel a(40, 40, 40), b(50, 50, 50);
      this->_putPixel(1, 1, a, 0);
      this->_putPixel(7, 8, b, 0);
      pcm.setPixelMatrix(this->pm);
      TS_ASSERT(pcm.isEqual(c1, c2));
    }

    void testIsEqualShouldBeFalse()
    {
      PixelCellMatrix pcm;

      // Check 3, 4 and 10, 7 cells. Use third key pixel
      Coords c1(3, 4);
      Coords c2(10, 7);

      // a and b are not similar pixels.
      Pixel a(10, 10, 10), b(10, 10, 55);
      this->_putPixel(3, 4, a, 2);
      this->_putPixel(10, 7, b, 2);
      pcm.setPixelMatrix(this->pm);
      TS_ASSERT(!pcm.isEqual(c1, c2));
    }

    void testIsEmptyWhenCellIsBlack()
    {
      PixelCellMatrix pcm;
      this->_makeEverythingEqual();
      this->_makeCellBlack(Coords(0, 0));

      pcm.setPixelMatrix(this->pm);
      TS_ASSERT(pcm.isEmpty(Coords(0, 0)));
    }

    void testIsEmptyWhenCellIsNonBlackPieceOfBackground()
    {
      // Such pieces doesn't have equal cells to them
      PixelCellMatrix pcm;
      this->_makeEverythingEqual();
      // Random pixels will make this cell not equal to any of other cells
      this->_putPixel(3, 4, Pixel(56, 78, 90), 0);
      this->_putPixel(3, 4, Pixel(66, 102, 200), 1);

      pcm.setPixelMatrix(this->pm);
      TS_ASSERT(pcm.isEmpty(Coords(3, 4)));
    }
};

