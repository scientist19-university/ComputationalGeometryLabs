#pragma once

#include<vector>

class Matrix
{
public:
  Matrix(const std::vector<std::vector<double>>& i_values);
  Matrix(const Matrix& i_matrix) = default;
  Matrix(size_t rows, size_t columns);
  ~Matrix();

  size_t rows() const;
  size_t columns() const;
  std::vector<double>& operator[](int i) const;
  Matrix operator *(const Matrix& i_other) const;
  Matrix& operator =(const Matrix& i_other) = default;
  static Matrix Eye(size_t n);

private:
  mutable std::vector<std::vector<double>> values;
};

