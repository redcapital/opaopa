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
};
