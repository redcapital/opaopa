#include <cxxtest/TestSuite.h>
#include <algorithm>
#include <iostream>
#include <cstdio>
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
      printf("\nOUTPUTTING MATCHES\n");
      for (int i = 0; i < results.size(); i++) {
        std::cout << results[i].first.x << ", " << results[i].first.y <<
          " and " << results[i].second.x << ", " << results[i].second.y <<
          std::endl;
      }
    }

    void _debugData()
    {
      printf("\nOUTPUTTING DATA\n");
      for (unsigned i = 0; i < ICellMatrix::CELLS_PER_COL; i++) {
        for (unsigned j = 0; j < ICellMatrix::CELLS_PER_ROW; j++) {
          printf("%8d", data[j][i]);
        }
        printf("\n");
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

      Match m = std::make_pair(Coords(2, 1), Coords(15, 1));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testVerticalLine()
    {
      DummyCellMatrix cm;
      this->data[3][0] = this->data[3][8] = 500000;
      for (int i = 1; i < 8; i++) {
        this->data[3][i] = -1; // empty
      }
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();

      Match m = std::make_pair(Coords(3, 0), Coords(3, 8));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testLeftUp()
    {
      DummyCellMatrix cm;
      this->data[4][4] = this->data[2][0] = 500000;
      this->data[2][1] = this->data[2][2] = this->data[2][3]
      = this->data[2][4] = this->data[3][4] = -1;
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();

      Match m = std::make_pair(Coords(2, 0), Coords(4, 4));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testLeftDown()
    {
      DummyCellMatrix cm;
      this->data[4][4] = this->data[2][7] = 500000;
      this->data[3][4] = this->data[2][4] = this->data[2][5]
      = this->data[2][6] = -1;
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();

      Match m = std::make_pair(Coords(2, 7), Coords(4, 4));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testLeftDownRight()
    {
      DummyCellMatrix cm;
      this->data[2][7] = this->data[0][3] = 500000;
      this->data[1][7] = this->data[0][7] = -1;
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();

      Match m = std::make_pair(Coords(0, 3), Coords(2, 7));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testLeftDownLeft()
    {
      DummyCellMatrix cm;
      this->data[3][0] = this->data[0][3] = 500000;
      this->data[2][0] = this->data[2][1] = this->data[2][2]
      = this->data[2][3] = this->data[1][3] = -1;
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();

      Match m = std::make_pair(Coords(0, 3), Coords(3, 0));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testUpRightDown()
    {
      DummyCellMatrix cm;
      this->data[0][0] = this->data[2][0] = 500000;
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();

      Match m = std::make_pair(Coords(0, 0), Coords(2, 0));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testRightDownLeft()
    {
      DummyCellMatrix cm;
      this->data[15][1] = this->data[15][7] = 500000;
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();

      Match m = std::make_pair(Coords(15, 1), Coords(15, 7));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }

    void testDownRightUp()
    {
      DummyCellMatrix cm;
      this->data[3][8] = this->data[7][8] = 500000;
      cm.setData(this->data);
      BfsSolver solver(cm);
      auto results = solver.solve();

      Match m = std::make_pair(Coords(3, 8), Coords(7, 8));
      TS_ASSERT(std::find(results.begin(), results.end(), m) != results.end());
    }
};

