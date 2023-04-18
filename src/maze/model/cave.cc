#include "cave.h"

void Cave::ReadFile() {
  std::ifstream file(path_);
  if (file.is_open()) {
    std::string line;
    std::getline(file, line);
    if (std::sscanf(line.c_str(), "%d %d", &rows_, &cols_) != 2) {
      throw std::invalid_argument("wrong input");
    }
    matrix_.SetRows(rows_);
    matrix_.SetCols(cols_);
    WriteMatrix();
    file.close();
  } else {
    throw std::invalid_argument("wrong path to file");
  }
}

void Cave::WriteMatrix() {
  std::ifstream file(path_);
  if (file.is_open()) {
    std::string line;
    std::getline(file, line);
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        if (!(file >> matrix_(i, j))) {
          throw std::invalid_argument("wrong input");
        }
      }
    }
    file.close();
  }
}

void Cave::GenerateCave() {
  S21Matrix generated(rows_, cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      generated(i, j) = GenerateRandomPercent();
    }
  }
  matrix_ = generated;
}

int Cave::GenerateRandomPercent() {
  std::random_device random;
  std::default_random_engine generator(random());
  std::uniform_int_distribution<int> distribution(1, 100);
  return distribution(generator) <= chance_;
}

void Cave::UpdateMatrix() {
  S21Matrix new_matrix(rows_, cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      int neighbors = CountNeighbors(i, j);
      if (matrix_(i, j) == ALIVE) {
        new_matrix(i, j) = (neighbors < death_lim_) ? DEAD : ALIVE;
      } else {
        new_matrix(i, j) = (neighbors > birth_lim_) ? ALIVE : DEAD;
      }
    }
  }
  matrix_ = new_matrix;
}

int Cave::CountNeighbors(int i, int j) {
  int count = 0;
  for (int x = -1; x <= 1; ++x) {
    for (int y = -1; y <= 1; ++y) {
      int neighbor_x = i + x;
      int neighbor_y = j + y;
      if (x == 0 && y == 0) {
      } else if (neighbor_x < 0 || neighbor_y < 0 || neighbor_x >= rows_ ||
                 neighbor_y >= cols_) {
        ++count;
      } else if (matrix_(neighbor_x, neighbor_y)) {
        ++count;
      }
    }
  }
  return count;
}

void Cave::setBirth_lim(int new_birth_lim) { birth_lim_ = new_birth_lim; }

void Cave::setDeath_lim(int new_death_lim) { death_lim_ = new_death_lim; }

void Cave::setRows(int new_rows) { rows_ = new_rows; }

void Cave::setCols(int new_cols) { cols_ = new_cols; }

void Cave::setChance(int new_chance) { chance_ = new_chance; }
void Cave::setPath(std::string path) { path_ = path; }
