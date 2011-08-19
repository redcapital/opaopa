#ifndef InvalidPixelMatrixException_h
#define InvalidPixelMatrixException_h

#include <stdexcept>

namespace PPSolver {

class InvalidPixelMatrixException : public std::runtime_error
{
  public:
    InvalidPixelMatrixException(std::string msg) : std::runtime_error(msg) {}
};

}

#endif

