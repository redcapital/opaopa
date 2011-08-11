#include <windows.h>
#include "Application.h"

using namespace PPSolver;

Application::Application()
{

}

Application& Application::getInstance()
{
  static Application instance;
  return instance;
}

// Main GUI loop
int Application::run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
  return 0;
}

ScreenshotGrabber& Application::getScreenshotGrabber()
{
  return this->ssGrabber;
}

PixelCellMatrix& Application::getCellMatrix()
{
  return this->cellMatrix;
}

ISolver& Application::getSolver()
{
  if (this->solver == NULL) {
    this->solver = new BfsSolver(this->cellMatrix);
  }
  return *(this->solver);
}

Application::~Application()
{
  delete this->solver;
}

