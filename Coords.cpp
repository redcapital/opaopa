#include "Coords.h"

using namespace PPSolver;

bool Coords::operator==(const Coords& c) const
{
  return (this->x == c.x && this->y == c.y);
}

bool Coords::operator<(const Coords& c) const
{
  if (this->x < c.x) {
    return true;
  }
  else if (c.x < this->x) {
    return false;
  }
  else if (this->y < c.y) {
    return true;
  }
  return false;
}

