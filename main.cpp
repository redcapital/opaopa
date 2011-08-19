#include <windows.h>
#include "Application.h"
#include "MainWindow.h"
#include "GuiException.h"

using namespace PPSolver;

// Application entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
  Application& app = Application::getInstance();
  try {
    MainWindow window(hInst, nShowCmd);
  } catch (GuiException e) {
		MessageBox(
      NULL,
      e.what(),
			"Application error",
			MB_ICONERROR
    );
    return 1;
  }
  return app.run();
}

