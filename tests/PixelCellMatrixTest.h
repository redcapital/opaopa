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

    void _putPixel(unsigned cellX, unsigned cellY, Pixel p, unsigned keyCoordNum)
    {
      PixelCellMatrix pcm;
      unsigned px = pcm.getCellBaseX(cellX) + PixelCellMatrix::KEY_PIXEL_COORDS[keyCoordNum].x;
      unsigned py = pcm.getCellBaseY(cellY) + PixelCellMatrix::KEY_PIXEL_COORDS[keyCoordNum].y;
      this->pm[px][py] = p;
    }

    void _putBgPixel(Coords cellCoords, Pixel p, unsigned whichCorner)
    {
      PixelCellMatrix pcm;
      Coords c = pcm.getCellBackgroundCoords(cellCoords, whichCorner);
      this->pm[c.x][c.y] = p;
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
      Coords c1(0, 0);
      Coords c2(0, 4);

      // So, everything is black and therefore all cells should be equal
      pcm.setPixelMatrix(this->pm);
      TS_ASSERT(pcm.isEqual(c1, c2));

      // Check 1, 1 and 7, 8 cells. Use first key pixel
      // a and b are similar pixels.
      Pixel a(10, 10, 10), b(20, 20, 20);
      this->_putPixel(1, 1, a, 0);
      this->_putPixel(7, 8, b, 0);
      c1.x = 1; c1.y = 1;
      c2.x = 7; c2.y = 8;
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

    void testFourDarkBackgroundPixelShouldBeDetectedAsEmpty()
    {
      PixelCellMatrix pcm;
      // Empty-ness is determined by doing crazy (in fact, everything in
      // PixelCellMatrix is crazy heuristic) checks of four pixels
      // of cell background. So we need to put correctly those 4 pixels

      // Put the same dark pixel on four positions
      // This should be considered as empty cell, because non-empty
      // cells has light background with four similar pixels.
      Coords c(3, 3);
      Pixel a(140, 140, 140);
      this->_putBgPixel(c, a, 0);
      this->_putBgPixel(c, a, 1);
      this->_putBgPixel(c, a, 2);
      this->_putBgPixel(c, a, 3);
      pcm.setPixelMatrix(this->pm);
      TS_ASSERT(pcm.isEmpty(c));
    }

    void testFourDifferentBackgroundPixelsShouldBeDetectedAsEmpty()
    {
      PixelCellMatrix pcm;
      Coords c(4, 4);
      this->_putBgPixel(c, Pixel(200, 200, 200), 0);
      this->_putBgPixel(c, Pixel(201, 201, 201), 1);
      this->_putBgPixel(c, Pixel(180, 180, 180), 2); // 1 and 3 not similar
      this->_putBgPixel(c, Pixel(190, 190, 190), 3);
      pcm.setPixelMatrix(this->pm);
      TS_ASSERT(pcm.isEmpty(c));
    }

    void testFourSimilarLightBackgroundPixelsShouldBeDetectedAsNotEmpty()
    {
      PixelCellMatrix pcm;
      Coords c(10, 6);
      // Look. The are similar
      this->_putBgPixel(c, Pixel(200, 200, 200), 0);
      this->_putBgPixel(c, Pixel(201, 201, 201), 1);
      this->_putBgPixel(c, Pixel(205, 208, 206), 2);
      this->_putBgPixel(c, Pixel(199, 201, 203), 3);
      pcm.setPixelMatrix(this->pm);
      TS_ASSERT(!pcm.isEmpty(c));
    }
};

