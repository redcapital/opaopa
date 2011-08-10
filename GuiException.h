#ifndef GuiException_h
#define GuiException_h

#include <stdexcept>

namespace PPSolver {

class GuiException : public std::runtime_error
{
  public:
    GuiException(std::string msg) : std::runtime_error(msg) {}
};

}

#endif

