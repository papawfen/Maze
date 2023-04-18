#include "parser.h"

void Parser::ReadFile() {
  std::ifstream file(path_);
  if (file.is_open()) {
    std::string line;
    std::getline(file, line);
    if (std::sscanf(line.c_str(), "%d %d", &rows_, &cols_) != 2) {
      throw std::invalid_argument("wrong input");
    }
    lower_walls_.SetRows(rows_);
    lower_walls_.SetCols(cols_);
    right_walls_.SetRows(rows_);
    right_walls_.SetCols(cols_);
    file.close();
    WriteMatrix();
  }
}

void Parser::WriteMatrix() {
  std::ifstream file(path_);
  if (file.is_open()) {
    std::string line;
    std::getline(file, line);
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        if (!(file >> right_walls_(i, j))) {
          throw std::invalid_argument("wrong input matrix");
        }
      }
    }
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        if (!(file >> lower_walls_(i, j))) {
          throw std::invalid_argument("wrong input");
        }
      }
    }
    file.close();
  }
}

Matrix *Parser::GetRightWalls() { return &right_walls_; }

Matrix *Parser::GetLowerWalls() { return &lower_walls_; }
