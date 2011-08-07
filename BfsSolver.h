#ifndef BfsSolver_h
#define BfsSolver_h

#include "AbstractSolver.h"

namespace PPSolver {


class BfsSolver : public AbstractSolver
{
  private:
    Match _makeMatch(Coords c1, Coords c2);

  public:
    struct Vertex
    {
      // Current coordinates
      Coords coords;

      // How many turns left?
      unsigned turns;

      // Current direction of search
      enum SearchDirection
      {
        None,
        Up,
        Right,
        Down,
        Left
      } direction;
    };
    BfsSolver(ICellMatrix& _matrix) : AbstractSolver(_matrix) {};
    std::vector<Match> solve();
};

}

#endif

