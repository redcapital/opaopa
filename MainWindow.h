#ifndef MainWindow_h
#define MainWindow_h

#include <windows.h>

#define IDC_MAIN_BUTTON 101

namespace PPSolver
{

class MainWindow
{
  protected:
    // Handler of all messages received by this window
    static LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

  public:
    static HWND handle;
    MainWindow(HINSTANCE hInst, int nShowCmd);
};

}

#endif

