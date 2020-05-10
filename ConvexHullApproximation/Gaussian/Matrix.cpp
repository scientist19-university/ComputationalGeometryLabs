#include "Matrix.h"

#include <stdexcept>

Matrix::Matrix(const std::vector<std::vector<double>>& i_values):
  values(i_values)
{
}

Matrix::Matrix(size_t rows, size_t columns)
{
  values.resize(rows);
  for (size_t i = 0; i < rows; i++)
    values[i].resize(columns);
}

Matrix::~Matrix()
{
}

size_t Matrix::rows() const
{
  return values.size();
}

size_t Matrix::columns() const
{
  if (rows() == 0) return 0;
  return values[0].size();
}

std::vector<double>& Matrix::operator[](int i) const
{
  return values[i];
}

Matrix Matrix::operator*(const Matrix & i_other) const
{
  if (this->columns() != i_other.rows()) throw std::logic_error("Bad matrix multiplication");
  Matrix result(this->rows(), i_other.columns());

  for (size_t i = 0; i < result.rows(); i++)
    for (size_t j = 0; j < result.columns(); j++) {
      double sum = 0;
      for (size_t k = 0; k < this->columns(); k++)
        sum += (*this)[i][k] * i_other[k][j];
      result[i][j] = sum;
    }

  return result;
}

Matrix Matrix::Eye(size_t n)
{
  Matrix result(n, n);
  for (size_t i = 0; i < n; i++)
    result[i][i] = 1;

  return result;
}

