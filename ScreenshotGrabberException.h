#ifndef ScreenshotGrabberException_h
#define ScreenshotGrabberException_h

#include <stdexcept>

namespace PPSolver {

class ScreenshotGrabberException : public std::runtime_error
{
  public:
    ScreenshotGrabberException(std::string msg) : std::runtime_error(msg) {}
};

}

#endif

