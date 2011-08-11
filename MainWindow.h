#ifndef MainWindow_h
#define MainWindow_h

#include <windows.h>
#include "ISolver.h"

#define IDC_MAIN_BUTTON 101

namespace PPSolver
{

class MainWindow
{
  protected:
    static const int BASEX = 20;
    static const int BASEY = 40;
    static void doPaint();
    static std::vector<Match> results;
    // Handler of all messages received by this window
    static LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

  public:
    static HWND handle;
    MainWindow(HINSTANCE hInst, int nShowCmd);
};

}

#endif

