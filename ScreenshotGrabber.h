#ifndef ScreenshotGrabber_h
#define ScreenshotGrabber_h

#include <windows.h>
#include "PixelCellMatrix.h"

namespace PPSolver {

class ScreenshotGrabber
{
  private:
    static HWND targetWindowHandle;
    static HDC memoryDc;
    static BOOL CALLBACK enumWindowsCallback(HWND hwnd, LPARAM lparam);
    // Helper function to call PrintWindow WinAPI function
    bool printWindow(HWND windowHandle, HDC dc);

  public:
    // Client area dimensions of PaoPao window
    static const int CLIENT_WIDTH = 768;
    static const int CLIENT_HEIGHT = 550;

    HWND findTargetWindow();
    HDC getMemoryDC();
    PixelMatrix getScreenshot(HWND handle);
};

}

#endif

