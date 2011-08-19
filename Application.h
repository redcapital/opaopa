#ifndef Application_h
#define Application_h

#include "ScreenshotGrabber.h"
#include "BfsSolver.h"

namespace PPSolver
{

class Application
{
  private:
    ScreenshotGrabber ssGrabber;
    PixelCellMatrix cellMatrix;
    BfsSolver* solver;
    Application();
    Application(Application const&);
    void operator=(Application const&);
    ~Application();

  public:
    int run();
    ScreenshotGrabber& getScreenshotGrabber();
    PixelCellMatrix& getCellMatrix();
    ISolver& getSolver();
    static Application& getInstance();
};

}

#endif

