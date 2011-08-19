#ifndef Pixel_h
#define Pixel_h

#include <vector>
#include <string>

namespace PPSolver {

class Pixel
{
  public:
    Pixel(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
    Pixel() : r(0), g(0), b(0) {}
    unsigned char r, g, b;
    std::vector <unsigned> getDiff(Pixel p);
    std::string toString();
    virtual ~Pixel() {}
};

}

#endif

