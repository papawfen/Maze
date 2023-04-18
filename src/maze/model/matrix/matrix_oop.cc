#include "matrix_oop.h"

Matrix::Matrix() : rows_(1), cols_(1) { CreateMatrix(); }

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows_ <= 0 || cols_ <= 0)
    throw std::invalid_argument("Error: invalid matrix sizes");
  CreateMatrix();
}

Matrix::Matrix(const Matrix& other) : matrix_(nullptr) {
  *this = other;
}

Matrix::Matrix(Matrix&& other) { *this = std::move(other); }

Matrix::~Matrix() { DeleteMatrix(); }

int Matrix::GetCols() { return cols_; }

int Matrix::GetRows() { return rows_; }

void Matrix::SetRows(int rows) {
  if (rows != rows_) {
    Matrix temp(rows, cols_);
    for (int i = 0; i < std::min(rows, rows_); i++) {
      for (int j = 0; j < cols_; j++) {
        temp.matrix_[i][j] = matrix_[i][j];
      }
    }
    if (matrix_) {
      DeleteMatrix();
    }
    *this = temp;
  }
}

void Matrix::SetCols(int cols) {
  if (cols != cols_) {
    Matrix temp(rows_, cols);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < std::min(cols, cols_); j++) {
        temp.matrix_[i][j] = matrix_[i][j];
      }
    }
    if (matrix_) {
      DeleteMatrix();
    }
    *this = temp;
  }
}

void Matrix::CreateMatrix() {
  if (rows_ <= 0 || cols_ <= 0)
    throw std::invalid_argument("Error: invalid matrix sizes");
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; i++) matrix_[i] = new double[cols_]();
}

void Matrix::DeleteMatrix() {
  if (matrix_) {
    for (int i = 0; i < GetRows(); i++) {
      if (matrix_[i] != nullptr) delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
}

bool Matrix::EqMatrix(const Matrix& other) {
  if (other.rows_ != rows_ || other.cols_ != cols_)
    throw std::invalid_argument("Error: invalid matrix sizes");
  bool res = true;
  for (int i = 0; i < rows_ && res == true; i++) {
    for (int j = 0; j < cols_ && res == true; j++) {
      if (fabs(other.matrix_[i][j] - matrix_[i][j]) > _EPS) {
        res = false;
      }
    }
  }
  return res;
}

void Matrix::SumMatrix(const Matrix& other) {
  if (other.rows_ != rows_ || other.cols_ != cols_) {
    throw std::invalid_argument("Error: matrix sizes aren't equal");
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j];
    }
  }
}

void Matrix::SubMatrix(const Matrix& other) {
  if (other.rows_ != rows_ || other.cols_ != cols_) {
    throw std::invalid_argument("Error: matrix sizes aren't equal");
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void Matrix::MulNumber(const double num) {
  if (isnan(num) || isinf(num))
    throw std::invalid_argument("Error: argument is NAN or inf");
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = matrix_[i][j] * num;
    }
  }
}

void Matrix::MulMatrix(const Matrix& other) {
  if (other.rows_ != rows_ || other.cols_ != cols_) {
    throw std::invalid_argument("Error: matrix sizes aren't equal");
  }
  Matrix result(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      result.matrix_[i][j] = 0.0;
      for (int k = 0; k < cols_; k++)
        result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
    }
  }
  *this = result;
}

Matrix Matrix::Transpose() {
  Matrix result(cols_, rows_);
  for (int i = 0; i < cols_; i++) {
    for (int j = 0; j < rows_; j++) {
      result.matrix_[i][j] = matrix_[j][i];
    }
  }
  return result;
}

Matrix Matrix::CalcComplements() {
  if (rows_ != cols_) {
    throw std::invalid_argument("Error: invalid matrix sizes");
  }
  Matrix result(rows_, cols_);
  if (rows_ == 1) {
    result.matrix_[0][0] = 1;
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        double determ = 0.0;
        Matrix m(rows_ - 1, cols_ - 1);
        Minor(m, i, j);
        determ = m.Determinant();
        result.matrix_[i][j] = determ * pow(-1, i + j);
      }
    }
  }
  return result;
}

Matrix Matrix::Minor(Matrix& m, int row, int col) {
  int mi = 0;
  for (int i = 0; i < rows_ - 1; i++) {
    if (i == row) mi = 1;
    int mj = 0;
    for (int j = 0; j < cols_ - 1; j++) {
      if (j == col) mj = 1;
      m.matrix_[i][j] = matrix_[i + mi][j + mj];
    }
  }
  return m;
}

double Matrix::Determinant() {
  if (rows_ != cols_)
    throw std::invalid_argument("Error: invalid matrix sizes");
  double result = 0;
  if (rows_ == 1) {
    result = matrix_[0][0];
    return result;
  }
  if (rows_ == 2) {
    result = (matrix_[0][0] * matrix_[1][1]) - (matrix_[0][1] * matrix_[1][0]);
    return result;
  }
  int sign = 1;
  for (int i = 0; i < rows_; i++) {
    double det = 0;
    Matrix temp(rows_ - 1, cols_ - 1);
    det = Minor(temp, i, 0).Determinant();
    result += (sign * matrix_[i][0] * det);
    sign *= -1;
  }
  return result;
}

Matrix Matrix::InverseMatrix() {
  if (rows_ != cols_) {
    throw std::invalid_argument("Error: invalid matrix sizes");
  }
  double check = 0;
  if (!(check = this->Determinant())) {
    throw std::invalid_argument("Error: invalid matrix sizes");
  }
  Matrix temp(rows_, cols_);
  temp = CalcComplements().Transpose();
  temp.MulNumber(1.0 / check);
  return temp;
}
