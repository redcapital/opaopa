#ifndef Coords_h
#define Coords_h

namespace PPSolver {

struct Coords
{
  unsigned x;
  unsigned y;
  public:
    Coords() : x(0), y(0) {}
    Coords(unsigned x, unsigned y) : x(x), y(y) {}
    bool operator<(const Coords& c) const;
    bool operator==(const Coords& c) const;
};

}

#endif
