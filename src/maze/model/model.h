#ifndef MODEL_H
#define MODEL_H

#include "cave.h"
#include "maze.h"
#include "parser.h"

class Model {
 public:
  Model() {
    cave_ = new Cave();
    parser_ = new Parser();
    maze_ = new Maze();
  }

  ~Model() {
    delete cave_;
    delete parser_;
    delete maze_;
  }

  bool ReadFile(std::string path) {
    parser_->setPath(path);
    try {
      parser_->ReadFile();
    } catch (...) {
      return false;
    }
    l_matrix_ = parser_->GetLowerWalls();
    r_matrix_ = parser_->GetRightWalls();
    return true;
  }

  void GenerateMaze(int rows, int cols) {
    maze_->GenerateMaze(rows, cols);
    maze_->SaveDefault();
    l_matrix_ = maze_->GetLowerWalls();
    r_matrix_ = maze_->GetRightWalls();
  }

  std::vector<std::pair<int, int>> GetMazeSolution(int startX, int startY,
                                                   int endX, int endY) {
    return maze_->SolveMaze(startX, startY, endX, endY);
  }

  bool GenerateCave(int rows, int cols, int chance) {
    cave_->setRows(rows);
    cave_->setCols(cols);
    cave_->setChance(chance);
    try {
      cave_->GenerateCave();
    } catch (...) {
      return false;
    }
    return true;
  }

  bool LoadCave(std::string path) {
    cave_->setPath(path);
    try {
      cave_->ReadFile();
    } catch (...) {
      return false;
    }
    return true;
  }

  void UpdateCave(int birth, int death) {
    cave_->setBirth_lim(birth);
    cave_->setDeath_lim(death);
    cave_->UpdateMatrix();
  }

  Matrix& GetCave() { return cave_->GetCave(); }
  Matrix& GetLMatrix() { return *l_matrix_; }
  Matrix& GetRMatrix() { return *r_matrix_; }

 private:
  Parser* parser_;
  Maze* maze_;
  Matrix* l_matrix_;
  Matrix* r_matrix_;
  Cave* cave_;
};

#endif  // MODEL_H
