#include "ScreenshotGrabber.h"
#include "ScreenshotGrabberException.h"
#include <iostream>
#include <sstream>
using namespace std;

using namespace PPSolver;

HWND ScreenshotGrabber::targetWindowHandle;
HDC ScreenshotGrabber::memoryDc = NULL;
HBITMAP ScreenshotGrabber::bitmap = NULL;
Coords ScreenshotGrabber::clientAreaBaseCoords;

HDC ScreenshotGrabber::getMemoryDC()
{
  return ScreenshotGrabber::memoryDc;
}

// Finds PaoPao window
HWND ScreenshotGrabber::findTargetWindow()
{
  ScreenshotGrabber::targetWindowHandle = NULL;
  EnumWindows((WNDENUMPROC)ScreenshotGrabber::enumWindowsCallback, 0);
  return ScreenshotGrabber::targetWindowHandle;
}

// Callback function used in enumerating windows when searching for
// PaoPao window.
BOOL CALLBACK ScreenshotGrabber::enumWindowsCallback(HWND handle, LPARAM lParam)
{
  char buf[16];
  GetWindowText(handle, buf, 16);
  // Array represents text: caption of target window
  char title[7] = {(char)179, (char)115, (char)179, (char)115, (char)172, (char)221, (char)50};
  int length = strlen(buf);
  bool ok = false;
  if (length == 7) {
    ok = true;
    // Check every byte
    for (int i = 0; i < 7; i++) {
      if ((0x000000ff & buf[i]) != (0x000000ff & title[i])) {
        ok = false;
        break;
      }
    }
  }
  if (ok) {
    ScreenshotGrabber::targetWindowHandle = handle;
    return FALSE;
  }
  return TRUE;
}

bool ScreenshotGrabber::printWindow(HWND windowHandle, HDC dc)
{
  typedef BOOL (WINAPI *tPrintWindow)(HWND, HDC, UINT);
  tPrintWindow pPrintWindow = NULL;
  HINSTANCE handle = LoadLibrary("User32.dll");

	pPrintWindow = (tPrintWindow)GetProcAddress(handle, "PrintWindow");
  bool result = false;
	if (pPrintWindow) {
    if (pPrintWindow(windowHandle, dc, 0)) {
      result = true;
    }
  }
	FreeLibrary(handle);
  return result;
}

PixelMatrix ScreenshotGrabber::getScreenshot(HWND handle)
{
  // Get device context (DC) of source window
  HDC sourceDc = GetDC(handle);

  // Get dimensions and coordinates of client area of source window
  RECT rc;
  POINT p;
  GetWindowRect(handle, &rc);
  p.x = 0;
  p.y = 0;
  ClientToScreen(handle, &p);
  int left = p.x - rc.left;
  int top = p.y - rc.top;
  ScreenshotGrabber::clientAreaBaseCoords.x = left;
  ScreenshotGrabber::clientAreaBaseCoords.y = top;

  if (ScreenshotGrabber::memoryDc == NULL) {
    // Create an in-memory DC to temporarily hold screenshot data
    ScreenshotGrabber::memoryDc = CreateCompatibleDC(sourceDc);
    // Create a bitmap for memory DC
    ScreenshotGrabber::bitmap = CreateCompatibleBitmap(
      sourceDc,
      rc.right - rc.left,
      rc.bottom - rc.top
    );
  }

  // Select this bitmap into memory DC
  HBITMAP oldBitmap = (HBITMAP)SelectObject(ScreenshotGrabber::memoryDc, ScreenshotGrabber::bitmap);
  if (!this->printWindow(handle, ScreenshotGrabber::memoryDc)) {
    throw ScreenshotGrabberException("Can not find PrintWindow function used to grab screenshots");
  }
  // Getting raw pixel data
  BITMAP bmp;
  GetObject(ScreenshotGrabber::bitmap, sizeof(BITMAP), &bmp);
  if (bmp.bmWidth < ScreenshotGrabber::CLIENT_WIDTH || bmp.bmHeight < ScreenshotGrabber::CLIENT_HEIGHT) {
    throw ScreenshotGrabberException("Got invalid dimensions of screenshot. Is PaoPao's window minimized or so?");
  }

  BITMAPINFOHEADER binfoHeader;
  ZeroMemory(&binfoHeader, sizeof(BITMAPINFOHEADER));

  binfoHeader.biSize = sizeof(BITMAPINFOHEADER);
  binfoHeader.biWidth = bmp.bmWidth;
  binfoHeader.biHeight = -bmp.bmHeight;
  binfoHeader.biPlanes = 1;
  binfoHeader.biBitCount = 32;
  binfoHeader.biCompression = BI_RGB;
  binfoHeader.biSizeImage = 0;
  binfoHeader.biXPelsPerMeter = 0;
  binfoHeader.biYPelsPerMeter = 0;
  binfoHeader.biClrUsed = 0;
  binfoHeader.biClrImportant = 0;

  DWORD bmpSize = ((bmp.bmWidth * binfoHeader.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
  HANDLE hDIB = GlobalAlloc(GHND, bmpSize);
  char* data = (char *)GlobalLock(hDIB);

  // We can't copy pixel data until selecting different bitmap into memory DC
  SelectObject(ScreenshotGrabber::memoryDc, oldBitmap);
  int numLines = GetDIBits(
    ScreenshotGrabber::memoryDc, ScreenshotGrabber::bitmap, 0, (UINT)bmp.bmHeight,
    data, (BITMAPINFO *)&binfoHeader, DIB_RGB_COLORS
  );
  PixelMatrix pm;
  pm.resize(ScreenshotGrabber::CLIENT_WIDTH);
  for (int i = 0; i < pm.size(); i++) {
    pm[i].resize(ScreenshotGrabber::CLIENT_HEIGHT);
  }
  for (int i = top; i < top + ScreenshotGrabber::CLIENT_HEIGHT; i++) {
    int lineOffset = i * bmp.bmWidth * 4;
    for (int j = left; j < left + ScreenshotGrabber::CLIENT_WIDTH; j++) {
      int offset = lineOffset + j * 4;
      pm[j - left][i - top].r = (0x000000ff & data[offset + 2]);
      pm[j - left][i - top].g = (0x000000ff & data[offset + 1]);
      pm[j - left][i - top].b = (0x000000ff & data[offset + 0]);
    }
  }

  // Select our bitmap into memory DC again
  SelectObject(ScreenshotGrabber::memoryDc, ScreenshotGrabber::bitmap);

  // Clean up
  GlobalUnlock(hDIB);
  GlobalFree(hDIB);
  DeleteObject(oldBitmap);
  ReleaseDC(handle, sourceDc);

  return pm;
}

ScreenshotGrabber::~ScreenshotGrabber()
{
  // Remove in memory DC
  if (ScreenshotGrabber::memoryDc != NULL) {
    DeleteDC(ScreenshotGrabber::memoryDc);
    DeleteObject(ScreenshotGrabber::bitmap);
  }
}

Coords& ScreenshotGrabber::getClientAreaBaseCoords()
{
  return ScreenshotGrabber::clientAreaBaseCoords;
}

