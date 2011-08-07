#include <vector>

#include <cxxtest/TestSuite.h>
#include "../Pixel.h"

using namespace PPSolver;

class PixelTest : public CxxTest::TestSuite
{
  public:
    void testGetDifference()
    {
      Pixel a, b;
      a.r = 20, a.g = 10, a.b = 200;
      b.r = 23, b.g = 15, b.b = 50;

      std::vector <unsigned> diff = a.getDiff(b);
      TS_ASSERT_EQUALS(3, diff.size());
      TS_ASSERT_EQUALS(3, diff[0]);
      TS_ASSERT_EQUALS(5, diff[1]);
      TS_ASSERT_EQUALS(150, diff[2]);
    }
};
