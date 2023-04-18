#include <gtest/gtest.h>

#include "../maze/model/maze.h"
#include "../maze/model/parser.h"

namespace s21 {

class MazeTester {
 public:
  MazeTester() {
    maze_ = new Maze;
    parser_ = new Parser;
  }
  ~MazeTester() {
    delete maze_;
    delete parser_;
  }

  void GenerateMaze(int rows, int cols) {
    rows_ = rows;
    cols_ = cols;
    maze_->GenerateMaze(rows, cols);
    was_here = S21Matrix(rows, cols);
    l_matrix_ = maze_->GetLowerWalls();
    r_matrix_ = maze_->GetRightWalls();
  }
  void ReadMazeFromFile(std::string path) {
    parser_->setPath(path);
    parser_->ReadFile();
    l_matrix_ = parser_->GetLowerWalls();
    r_matrix_ = parser_->GetRightWalls();
    rows_ = l_matrix_->GetRows();
    cols_ = l_matrix_->GetCols();
    was_here = S21Matrix(rows_, cols_);
  }
  bool Test() {
    std::queue<std::vector<std::pair<int, int>>> paths;
    std::vector<std::pair<int, int>> path;
    path.push_back({0, 0});
    was_here.MulNumber(0);
    was_here(0, 0) = 1;
    paths.push(path);

    while (!paths.empty()) {
      path = paths.front();
      paths.pop();
      std::pair<int, int> last = path[path.size() - 1];

      std::vector<std::pair<int, int>> possible_moves;
      if (last.second > 0 &&
          (*(l_matrix_))(last.second - 1, last.first) != 1) {  // up
        possible_moves.push_back({last.first, last.second - 1});
      }
      if (last.second < rows_ - 1 &&
          (*(l_matrix_))(last.second, last.first) != 1) {  // down
        possible_moves.push_back({last.first, last.second + 1});
      }
      if (last.first > 0 &&
          (*(r_matrix_))(last.second, last.first - 1) != 1) {  // left
        possible_moves.push_back({last.first - 1, last.second});
      }
      if (last.first < cols_ - 1 &&
          (*(r_matrix_))(last.second, last.first) != 1) {  // right
        possible_moves.push_back({last.first + 1, last.second});
      }
      std::pair<int, int> prev_last;
      if (path.size() > 1)
        prev_last = path[path.size() - 2];
      else
        prev_last = {-1, -1};

      for (auto point : possible_moves) {
        if (IsPairInVector(path, point) && (point.first != prev_last.first ||
                                            point.second != prev_last.second)) {
          return false;
        }
        if (!IsPairInVector(path, point)) {
          std::vector<std::pair<int, int>> newpath(path);
          was_here(point.second, point.first) = 1;
          newpath.push_back({point.first, point.second});
          paths.push(newpath);
        }
      }
    }
    for (int i = 0; i < rows_; ++i)
      for (int j = 0; j < cols_; ++j)
        if (was_here(i, j) != 1) return false;
    return true;
  }

 private:
  bool IsPairInVector(std::vector<std::pair<int, int>> &v,
                      std::pair<int, int> &p) {
    for (auto &el : v)
      if (el.first == p.first && el.second == p.second) return true;
    return false;
  }
  int rows_, cols_;
  Maze *maze_;
  Parser *parser_;
  S21Matrix was_here;
  S21Matrix *l_matrix_;
  S21Matrix *r_matrix_;
};
}  // namespace s21

TEST(maze, perfect_maze) {
  s21::MazeTester toster;
  for (int rows = 1; rows <= 50; ++rows) {
    for (int cols = 1; cols <= 50; ++cols) {
      toster.GenerateMaze(rows, cols);
      ASSERT_TRUE(toster.Test() == true);
    }
  }
  ASSERT_TRUE(1 == 1);
}

TEST(maze, faulty_maze) {
  s21::MazeTester toster;
  std::vector<std::string> files = {"tests/bad_maze.0.txt"};
  for (auto &file : files) {
    toster.ReadMazeFromFile(file);
    ASSERT_TRUE(toster.Test() == false);
  }
}