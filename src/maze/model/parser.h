#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>

#include "matrix/matrix_oop.h"

class Parser {
 public:
  Parser(){};
  ~Parser(){};

  void ReadFile();
  void WriteMatrix();

  void setPath(std::string new_path) { path_ = new_path; }

  Matrix *GetRightWalls();
  Matrix *GetLowerWalls();

 private:
  int rows_, cols_;
  Matrix right_walls_;
  Matrix lower_walls_;
  std::string path_;
};

#endif  // PARSER_H
