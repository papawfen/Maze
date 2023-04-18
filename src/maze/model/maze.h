#ifndef MAZE_H
#define MAZE_H

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <vector>

#include "matrix/matrix_oop.h"

#ifndef ABS
#define ABS "./"
#endif

class Maze {
 public:
  Maze() { srand((unsigned)time(NULL)); }
  ~Maze() {}

  Matrix *GetRightWalls() { return &right_walls; }
  Matrix *GetLowerWalls() { return &lower_walls; }

  void SaveDefault() {
    std::string abs = ABS;
    auto dirIter =
        std::filesystem::directory_iterator(abs + "/../generated_mazes");
    int fileCount = 0;

    for (auto &entry : dirIter) {
      if (entry.is_regular_file()) {
        ++fileCount;
      }
    }

    std::ofstream file(abs + "/../generated_mazes/maze." +
                       std::to_string(fileCount) + ".txt");
    if (file.is_open()) {
      file << rows_ << " " << cols_ << std::endl;
      file << right_walls;
      file << lower_walls;
    }
  }

  bool IsPairInVector(std::vector<std::pair<int, int>> &v,
                      std::pair<int, int> &p) {
    for (auto &el : v)
      if (el.first == p.first && el.second == p.second) return true;
    return false;
  }

  std::vector<std::pair<int, int>> SolveMaze(int startX, int startY, int endX,
                                             int endY) {
    std::queue<std::vector<std::pair<int, int>>> paths;
    std::vector<std::pair<int, int>> path;
    path.push_back({startX, startY});

    paths.push(path);
    std::vector<std::pair<int, int>> shortest_path;
    while (!paths.empty()) {
      path = paths.front();
      paths.pop();
      std::pair<int, int> last = path[path.size() - 1];

      if (last.first == endX && last.second == endY) {
        if (shortest_path.size() == 0 || shortest_path.size() < path.size()) {
          shortest_path = path;
        }
      }

      std::vector<std::pair<int, int>> possible_moves;
      if (last.second > 0 &&
          lower_walls(last.second - 1, last.first) != 1) {  // up
        possible_moves.push_back({last.first, last.second - 1});
      }
      if (last.second < rows_ - 1 &&
          lower_walls(last.second, last.first) != 1) {  // down
        possible_moves.push_back({last.first, last.second + 1});
      }
      if (last.first > 0 &&
          right_walls(last.second, last.first - 1) != 1) {  // left
        possible_moves.push_back({last.first - 1, last.second});
      }
      if (last.first < cols_ - 1 &&
          right_walls(last.second, last.first) != 1) {  // right
        possible_moves.push_back({last.first + 1, last.second});
      }
      for (auto point : possible_moves) {
        if (!IsPairInVector(path, point)) {
          std::vector<std::pair<int, int>> newpath(path);
          newpath.push_back({point.first, point.second});
          paths.push(newpath);
        }
      }
    }
    std::pair<int, int> last = path[path.size() - 1];
    if (last.first == endX && last.second == endY) {
      if (shortest_path.size() > path.size()) {
        shortest_path = path;
      }
    }
    return shortest_path;
  }

  void GenerateMaze(int rows, int cols) {
    rows_ = rows;
    cols_ = cols;
    right_walls = Matrix(rows, cols);
    lower_walls = Matrix(rows, cols);
    uniq_sets.clear();
    uniq_count_ = 0;
    uniq_sets.resize(cols);

    for (int i = 0; i < rows_; ++i) {
      Uniqify();
      AddRightWalls(i);
      if (i != rows_ - 1) AddLowerWalls(i);
      if (i != rows_ - 1) CleanSets(i);
    }
    AddEndLine();
    CheckEndLine();
  }

 private:
  void AddEndLine() {
    for (int i = 0; i < cols_; ++i) {
      lower_walls(rows_ - 1, i) = 1;
    }
  }

  void CheckEndLine() {
    for (int i = 0; i < cols_ - 1; ++i) {
      if (uniq_sets[i] != uniq_sets[i + 1]) {
        right_walls(rows_ - 1, i) = 0;
        MergeSets(i);
      }
    }
  }

  void CleanSets(int row) {
    for (int i = 0; i < cols_; ++i) {
      if (lower_walls(row, i)) uniq_sets[i] = 0;
    }
  }

  void Uniqify() {
    for (int i = 0; i < cols_; ++i)
      if (uniq_sets[i] == 0) uniq_sets[i] = ++uniq_count_;
  }

  void AddRightWalls(int row) {
    for (int i = 0; i < cols_ - 1; ++i) {
      if (uniq_sets[i] == uniq_sets[i + 1] || RandomBool()) {
        right_walls(row, i) = 1;
      } else {
        MergeSets(i);
      }
    }
    right_walls(row, cols_ - 1) = 1;
  }

  void MergeSets(int cur_set_pos) {
    int right_set_val = uniq_sets[cur_set_pos + 1];
    for (std::size_t i = 0; i < uniq_sets.size(); ++i)
      if (uniq_sets[i] == right_set_val) uniq_sets[i] = uniq_sets[cur_set_pos];
  }

  void AddLowerWalls(int row) {
    for (int i = 0; i < cols_; ++i) {
      int cur_set_lower_walls_size = CountSetLowerWalls(uniq_sets[i], row);
      int set_size = CountSet(uniq_sets[i]);
      if (RandomBool() && (set_size - cur_set_lower_walls_size) > 1) {
        lower_walls(row, i) = 1;
      }
    }
  }

  int CountSet(int set_val) {
    int res = 0;
    for (int i = 0; i < cols_; ++i) {
      if (uniq_sets[i] == set_val) ++res;
    }
    return res;
  }

  int CountSetLowerWalls(int set_val, int row) {
    int res = 0;
    for (int i = 0; i < cols_; ++i) {
      if (lower_walls(row, i) != 0 && uniq_sets[i] == set_val) ++res;
    }
    return res;
  }
  bool RandomBool() { return rand() % 2 == 0; }

  int rows_{0}, cols_{0};
  int uniq_count_{0};
  std::vector<int> uniq_sets;
  Matrix right_walls;
  Matrix lower_walls;
};

#endif  // MAZE_H
