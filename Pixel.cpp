#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Pixel.h"

using namespace PPSolver;

std::vector <unsigned> Pixel::getDiff(Pixel p)
{
  std::vector <unsigned> result(3, 0);
  result[0] = abs(this->r - p.r);
  result[1] = abs(this->g - p.g);
  result[2] = abs(this->b - p.b);
  return result;
}

std::string Pixel::toString()
{
  std::stringstream ss;
  ss << "r: " << (0x000000ff & this->r);
  ss << ", g: " << (0x000000ff & this->g);
  ss << ", b: " << (0x000000ff & this->b);
  return ss.str();
}

