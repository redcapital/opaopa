#ifndef InvalidDimensionException_h
#define InvalidDimensionException_h

#include <stdexcept>

namespace PPSolver {

class InvalidDimensionException : public std::runtime_error
{
  public:
    InvalidDimensionException(std::string msg) : std::runtime_error(msg) {}
};

}

#endif

