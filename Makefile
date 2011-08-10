CXX := g++
CXX_FLAGS := -std=c++0x
LINK_FLAGS := -lgdi32
CXXTEST_DIR := ./cxxtest
TESTDIR := ./tests
LIB_OBJECTS := Application.o AbstractSolver.o BfsSolver.o Coords.o DummyCellMatrix.o Pixel.o PixelCellMatrix.o
APP_OBJECTS := main.o MainWindow.o ScreenshotGrabber.o

all: app

app: $(APP_OBJECTS) $(LIB_OBJECTS)
	$(CXX) $(CXX_FLAGS) $(APP_OBJECTS) $(LIB_OBJECTS) -o app.exe $(LINK_FLAGS) 

test: runner.exe
	runner.exe

runner.exe: runner.o $(LIB_OBJECTS)
	$(CXX) $(CXX_FLAGS) runner.o $(LIB_OBJECTS) -o runner.exe

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

runner.o: runner.cpp
	$(CXX) $(CXX_FLAGS) -I $(TESTDIR)/include -c runner.cpp -o runner.o

runner.cpp: $(TESTDIR)/*.h
	python $(CXXTEST_DIR)/cxxtestgen.py --error-printer -o runner.cpp $(TESTDIR)/*.h

clean:
	del *.o *.exe runner.cpp
