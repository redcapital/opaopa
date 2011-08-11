#include "MainWindow.h"
#include "Application.h"
#include "GuiException.h"
#include "ScreenshotGrabberException.h"

#include <iostream>

using namespace PPSolver;
using namespace std;

HWND MainWindow::handle = NULL;
std::vector<Match> MainWindow::results;

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

// Handler of WM_PAINT message
// Paints results of solving on application's main window
void MainWindow::doPaint()
{
  ScreenshotGrabber& scGrabber = Application::getInstance().getScreenshotGrabber();
  HDC memoryDc = scGrabber.getMemoryDC();
  if (memoryDc != NULL) {
    PixelCellMatrix& pcm = Application::getInstance().getCellMatrix();
    HDC myDc = GetDC(MainWindow::handle);
    Coords& offset = scGrabber.getClientAreaBaseCoords();
    for (auto i = MainWindow::results.begin(); i != MainWindow::results.end(); i++) {
      BitBlt(
        myDc,
        MainWindow::BASEX + i->first.x * PixelCellMatrix::CELL_WIDTH,
        MainWindow::BASEY + i->first.y * PixelCellMatrix::CELL_HEIGHT,
        PixelCellMatrix::CELL_WIDTH,
        PixelCellMatrix::CELL_HEIGHT,
        memoryDc,
        offset.x + pcm.getCellBaseX(i->first.x),
        offset.y + pcm.getCellBaseY(i->first.y),
        SRCCOPY
      );
      BitBlt(
        myDc,
        MainWindow::BASEX + i->second.x * PixelCellMatrix::CELL_WIDTH,
        MainWindow::BASEY + i->second.y * PixelCellMatrix::CELL_HEIGHT,
        PixelCellMatrix::CELL_WIDTH,
        PixelCellMatrix::CELL_HEIGHT,
        memoryDc,
        offset.x + pcm.getCellBaseX(i->second.x),
        offset.y + pcm.getCellBaseY(i->second.y),
        SRCCOPY
      );
    }
    ReleaseDC(MainWindow::handle, myDc);
  }
}

LRESULT CALLBACK MainWindow::WinProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
  HWND hWndButton;
  PAINTSTRUCT ps;
  HDC hdc;
  Application& app = Application::getInstance();
  ScreenshotGrabber& scGrabber = app.getScreenshotGrabber();
  HWND targetHandle;
  PixelMatrix pm;
	switch(msg) {
    case WM_CREATE:
      hWndButton = CreateWindowEx(
        0,
        "BUTTON",
        "GO!",
        WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
        350,
        10,
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
            app.getCellMatrix().setPixelMatrix(pm);
            MainWindow::results = app.getSolver().solve();
            InvalidateRect(handle, NULL, TRUE);
          }
          break;
      }
      break;

    case WM_PAINT:
      hdc = BeginPaint(handle, &ps);
      MainWindow::doPaint();
      EndPaint(handle, &ps);
      break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(handle, msg, wParam, lParam);
}

