#include "matrix_oop.h"

double& Matrix::operator()(int row, int col) {
  if (row >= rows_ || col >= cols_) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
  return matrix_[row][col];
}

bool Matrix::operator==(const Matrix& o) { return this->EqMatrix(o); }

Matrix& Matrix::operator=(Matrix&& other) {
  if (this != &other) {
    DeleteMatrix();
    matrix_ = other.matrix_;
    rows_ = other.rows_;
    cols_ = other.cols_;
    other.matrix_ = nullptr;
    other.rows_ = 0;
    other.cols_ = 0;
  }
  return *this;
}

Matrix& Matrix::operator=(const Matrix& other) {
  if (this != &other) {
    DeleteMatrix();
    rows_ = other.rows_;
    cols_ = other.cols_;
    CreateMatrix();
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
  return *this;
}

Matrix& Matrix::operator+=(const Matrix& o) {
  SumMatrix(o);
  return *this;
}

Matrix Matrix::operator+(const Matrix& o) {
  Matrix result(*this);
  result.SumMatrix(o);
  return result;
}

Matrix Matrix::operator-(const Matrix& o) {
  Matrix result(*this);
  result.SubMatrix(o);
  return result;
}

Matrix& Matrix::operator-=(const Matrix& o) {
  SubMatrix(o);
  return *this;
}

Matrix Matrix::operator*(const Matrix& o) {
  Matrix result(*this);
  result.MulMatrix(o);
  return result;
}

Matrix Matrix::operator*(const double B) {
  Matrix result(*this);
  result.MulNumber(B);
  return result;
}

Matrix operator*(const double B, Matrix& A) {
  Matrix result(A);
  result.MulNumber(B);
  return result;
}

Matrix& Matrix::operator*=(const double B) {
  this->MulNumber(B);
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Matrix& A) {
  for (int i = 0; i < A.rows_; i++) {
    for (int j = 0; j < A.cols_; j++) os << A.matrix_[i][j] << " ";
    os << std::endl;
  }
  os << std::endl;
  return os;
}
