#ifndef CAVE_H
#define CAVE_H

#include <filesystem>
#include <fstream>
#include <random>
#include <string>

#include "matrix/matrix_oop.h"

enum condition { DEAD, ALIVE };

class Cave {
 public:
  Cave(){};
  ~Cave(){};

  void ReadFile();
  void GenerateCave();
  int GenerateRandomPercent();
  void UpdateMatrix();
  int CountNeighbors(int i, int j);
  void WriteMatrix();
  Matrix& GetCave() { return matrix_; }

  void setBirth_lim(int new_birth_lim);
  void setDeath_lim(int new_death_lim);
  void setRows(int new_rows);
  void setCols(int new_cols);
  void setChance(int new_chance);
  void setMatrix(Matrix cave) { matrix_ = cave; }
  void setPath(std::string path_);

 private:
  Matrix matrix_;
  int birth_lim_, death_lim_;
  int rows_, cols_;
  int chance_;
  std::string path_;
};

#endif  // CAVE_H
