#include "MainWindow.h"
#include "Application.h"
#include "GuiException.h"
#include "ScreenshotGrabberException.h"

#include <iostream>

using namespace PPSolver;
using namespace std;

HWND MainWindow::handle = NULL;

MainWindow::MainWindow(HINSTANCE hInst, int nShowCmd)
{
  // Create instance of window class
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX));
	wClass.cbClsExtra = 0;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = 0;
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wClass.hIcon = NULL;
	wClass.hIconSm = NULL;
	wClass.hInstance = hInst;
	wClass.lpfnWndProc = (WNDPROC)(MainWindow::WinProc);
	wClass.lpszClassName = "OpaOpa window class";
	wClass.lpszMenuName = NULL;
	wClass.style = CS_HREDRAW|CS_VREDRAW;

  // Register window class
	if (!RegisterClassEx(&wClass)) {
		int result = GetLastError();
    throw GuiException("Window class creation failed");
	}

  // Create main window
  MainWindow::handle = CreateWindowEx(
    0,
    "OpaOpa window class",
    "OpaOpa - sophisticated cheat tool",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    800,
    600,
    NULL,
    NULL,
    hInst,
    NULL
  );

	if (!MainWindow::handle) {
		int result = GetLastError();
    throw GuiException("Failed to create main window");
	}

	ShowWindow(MainWindow::handle, nShowCmd);
}

LRESULT CALLBACK MainWindow::WinProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
  HWND hWndButton, hwndPP = NULL;
  PAINTSTRUCT ps;
  HDC hdc;
  ScreenshotGrabber scGrabber;
  HWND targetHandle;
  PixelMatrix pm;
  std::vector<Match> results;
	switch(msg) {
    case WM_CREATE:
      hWndButton = CreateWindowEx(
        0,
        "BUTTON",
        "GO!",
        WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
        50,
        220,
        100,
        24,
        handle,
        (HMENU)IDC_MAIN_BUTTON,
        GetModuleHandle(NULL),
        NULL
      );
      break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case IDC_MAIN_BUTTON:
          // Button click handler
          scGrabber = Application::getInstance().getScreenshotGrabber();
          targetHandle = scGrabber.findTargetWindow();
          if (targetHandle == NULL) {
            MessageBox(
              handle,
              "Can't find PaoPao window. Is it running?",
              "Warning",
              MB_ICONERROR
            );
          } else {
            try {
              pm = scGrabber.getScreenshot(targetHandle);
            } catch (ScreenshotGrabberException e) {
              MessageBox(
                handle,
                e.what(),
                "Error",
                MB_ICONERROR
              );
              break;
            }
            Application::getInstance().getCellMatrix().setPixelMatrix(pm);
            results = Application::getInstance().getSolver().solve();
            for (int i = 0; i < results.size(); i++) {
              cout << results[i].first.x << ", " << results[i].first.y
                << " and " << results[i].second.x << ", " << results[i].second.y << endl;
            }
          }
          break;
      }
      break;

    case WM_PAINT:
      //hdc = BeginPaint(hWnd, &ps);
      //dopaint(hWnd);
      //EndPaint(hWnd, &ps);
      break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(handle, msg, wParam, lParam);
}

