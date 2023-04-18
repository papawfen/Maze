#ifndef _MATRIX_OOP_H_
#define _MATRIX_OOP_H_

#include <math.h>

#include <algorithm>
#include <cstring>
#include <iostream>

#define _EPS 1e-7

class Matrix {
 public:
  Matrix();
  Matrix(int rows, int cols);
  Matrix(Matrix&& other);
  Matrix(const Matrix& other);
  ~Matrix();

  int GetCols();
  int GetRows();
  void SetCols(const int cols);
  void SetRows(const int rows);

  Matrix& operator=(const Matrix& o);
  Matrix& operator=(Matrix&& other);
  Matrix& operator+=(const Matrix& o);
  Matrix operator+(const Matrix& o);
  Matrix& operator-=(const Matrix& o);
  Matrix operator-(const Matrix& o);
  bool operator==(const Matrix& o);
  Matrix operator*(const Matrix& o);
  Matrix operator*(const double B);
  friend Matrix operator*(const double B, Matrix& A);
  Matrix& operator*=(const Matrix& o);
  Matrix& operator*=(const double B);
  double& operator()(int row, int col);
  friend std::ostream& operator<<(std::ostream& os, const Matrix& A);

  void SumMatrix(const Matrix& other);
  void CreateMatrix();
  bool EqMatrix(const Matrix& other);
  void DeleteMatrix();
  void SubMatrix(const Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const Matrix& other);
  Matrix Transpose();
  Matrix CalcComplements();
  Matrix Minor(Matrix& m, int row, int col);
  double Determinant();
  Matrix InverseMatrix();

 private:
  int rows_, cols_;
  double** matrix_;
};

#endif
