#include <queue>
#include <set>
#include "BfsSolver.h"

using namespace PPSolver;

Match BfsSolver::_makeMatch(Coords c1, Coords c2)
{
  Match m;
  if (c1 < c2) {
    m.first = c1;
    m.second = c2;
  } else {
    m.first = c2;
    m.second = c1;
  }
  return m;
}

std::vector<Match> BfsSolver::solve()
{
  std::set<Match> found;
  std::queue<BfsSolver::Vertex> q;
  // v = current vertex, n = new vertex
  BfsSolver::Vertex v, n;
  Coords c, nc, start;
  BfsSolver::Vertex::SearchDirection directions[4] = {
    BfsSolver::Vertex::Right,
    BfsSolver::Vertex::Down,
    BfsSolver::Vertex::Left,
    BfsSolver::Vertex::Up
  };

  for (unsigned i = 0; i < ICellMatrix::CELLS_PER_ROW; i++) {
    for (unsigned j = 0; j < ICellMatrix::CELLS_PER_COL; j++) {
      // Construct starting vertex
      start.x = i;
      start.y = j;
      v.coords.x = i;
      v.coords.y = j;
      v.turns = 3; // @todo define as constant value
      v.direction = BfsSolver::Vertex::None;
      q.push(v);
      while (!q.empty()) {
        v = q.front();
        q.pop();
        c = v.coords;
        for (unsigned k = 0; k < 4; k++) {
          // Go to k-th direction
          bool canMove = false;
          switch (directions[k]) {
            case BfsSolver::Vertex::Right:
              canMove =
                v.direction != BfsSolver::Vertex::Left
                && c.x < (int)ICellMatrix::CELLS_PER_ROW
                && (v.direction == BfsSolver::Vertex::Right || v.turns > 0);
              nc.x = c.x + 1;
              nc.y = c.y;
              break;
            case BfsSolver::Vertex::Down:
              canMove =
                v.direction != BfsSolver::Vertex::Up
                && c.y < (int)ICellMatrix::CELLS_PER_COL
                && (v.direction == BfsSolver::Vertex::Down || v.turns > 0);
              nc.x = c.x;
              nc.y = c.y + 1;
              break;
            case BfsSolver::Vertex::Left:
              canMove =
                v.direction != BfsSolver::Vertex::Right
                && c.x >= 0
                && (v.direction == BfsSolver::Vertex::Left || v.turns > 0);
              nc.x = c.x - 1;
              nc.y = c.y;
              break;
            case BfsSolver::Vertex::Up:
              canMove =
                v.direction != BfsSolver::Vertex::Down
                && c.y >= 0
                && (v.direction == BfsSolver::Vertex::Up || v.turns > 0);
              nc.x = c.x;
              nc.y = c.y - 1;
              break;
          }
          if (canMove) {
            // Whether the new coordinates are outside of board?
            bool isOutside = nc.x < 0 || nc.y < 0
                || nc.x == ICellMatrix::CELLS_PER_ROW
                || nc.y == ICellMatrix::CELLS_PER_COL;
            if (isOutside || this->matrix.isEmpty(nc)) {
              // Make new vertex
              n.turns = v.turns;
              if (v.direction != directions[k]) {
                n.turns--;
              }
              n.direction = directions[k];
              n.coords = nc;
              q.push(n);
            } else {
              // Check whether blocking cell is the same
              if (!isOutside && this->matrix.isEqual(start, nc)) {
                Match m = this->_makeMatch(start, nc);
                if (found.count(m) == 0) {
                  found.insert(m);
                }
              }
            }
          }
        }
      }
    }
  }
  std::vector<Match> result(found.begin(), found.end());
  return result;
}

