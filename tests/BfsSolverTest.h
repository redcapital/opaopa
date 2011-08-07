#include <cxxtest/TestSuite.h>
#include <algorithm>
#include <iostream>
#include "../BfsSolver.h"
#include "../DummyCellMatrix.h"

using namespace PPSolver;

class BfsSolverTest : public CxxTest::TestSuite
{
  private:
    // Data for DummyCellMatrix
    std::vector< std::vector<int> > data;

  public:
    void _debug(std::vector<Match> results)
    {
      for (int i = 0; i < results.size(); i++) {
        std::cout << results[i].first.x << ", " << results[i].first.y <<
          " and " << results[i].second.x << ", " << results[i].second.y <<
          std::endl;
      }
    }

    void setUp()
    {
      // Set all cells different by default
      data.clear();
      data.resize(ICellMatrix::CELLS_PER_ROW);
      unsigned counter = 1;
      for (unsigned i = 0; i < ICellMatrix::CELLS_PER_ROW; i++) {
        data[i].resize(ICellMatrix::CELLS_PER_COL);
        for (unsigned j = 0; j < ICellMatrix::CELLS_PER_COL; j++) {
          data[i][j] = counter++;
        }
      }
    }

    void testHorizontallyAdjacent()
    {
      DummyCellMatrix cm;
      this->data[2][1] = this->data[3][1] = 500000;
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();
      std::cout << std::endl;
      this->_debug(results);

      Match m = std::make_pair(Coords(2, 1), Coords(3, 1));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testVerticallyAdjacent()
    {
      DummyCellMatrix cm;
      this->data[15][7] = this->data[15][8] = 500000;
      this->data[15][0] = this->data[15][1] = 400000;
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();
      std::cout << std::endl;
      this->_debug(results);

      Match m = std::make_pair(Coords(15, 7), Coords(15, 8));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
      m.first.x = 15, m.first.y = 0;
      m.second.x = 15, m.second.y = 1;
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testHorizontalLine()
    {
      DummyCellMatrix cm;
      this->data[2][1] = this->data[15][1] = 500000;
      for (int i = 3; i < 15; i++) {
        this->data[i][1] = -1; // empty
      }
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();
      std::cout << std::endl;
      this->_debug(results);

      Match m = std::make_pair(Coords(2, 1), Coords(15, 1));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }
};

