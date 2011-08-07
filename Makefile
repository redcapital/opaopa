CXX := g++
CXX_FLAGS := -std=c++0x
LINK_FLAGS := -lgdi32
CXXTEST_DIR := ./cxxtest
TESTDIR := ./tests
OBJECTS := AbstractSolver.o BfsSolver.o Coords.o DummyCellMatrix.o Pixel.o PixelCellMatrix.o

all: app

app: main.o $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(LINK_FLAGS) main.o $(OBJECTS) -o app.exe

test: runner.exe
	runner.exe

runner.exe: runner.o $(OBJECTS)
	$(CXX) $(CXX_FLAGS) runner.o $(OBJECTS) -o runner.exe

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

runner.o: runner.cpp
	$(CXX) $(CXX_FLAGS) -I $(TESTDIR)/include -c runner.cpp -o runner.o

runner.cpp: $(TESTDIR)/*.h
	python $(CXXTEST_DIR)/cxxtestgen.py --error-printer -o runner.cpp $(TESTDIR)/*.h

clean:
	del *.o *.exe runner.cpp
