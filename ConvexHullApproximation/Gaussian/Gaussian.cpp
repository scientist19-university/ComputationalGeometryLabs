#include "Matrix.h"

bool modifyP(const Matrix& P, const Matrix& A, size_t k) {
  double max = -2e9;
  size_t k0;
  for (size_t i = k; i < A.rows(); i++)
    if (A[k][i] > max) {
      max = A[k][i];
      k0 = i;
    }

  if (k0 == k) return false;

  std::swap(P[k], P[k0]);
  return true;
}

std::vector<double> backSubstitution(const Matrix& A) {

  size_t n = A.rows();
  std::vector<double> x(n);

  x[n - 1] = A[n - 1][n];
  for (int i = n - 2; i >= 0; i--) {
    double tmp_sum = 0;
    for (size_t j = i + 1; j < n; j++)
      tmp_sum += A[i][j] * x[j];
    x[i] = A[i][n] - tmp_sum;
  }

  return x;
}

std::pair<std::vector<double>, double> solve(const Matrix& matrix) {

  size_t n = matrix.rows();
  Matrix A = matrix;
  size_t l = 0;
  double det = 1;

  for (size_t k = 0; k < n; k++) {

    Matrix P = Matrix::Eye(n);
    bool modified = modifyP(P, A, k);
    if (modified) l++;

    A = P * A;
    det *= A[k][k];

    Matrix M = Matrix::Eye(n);
    M[k][k] = 1 / A[k][k];
    for (size_t i = k + 1; i < n; i++)
      M[i][k] = -A[i][k] / A[k][k];

    A = M * A;
  }

  if (l % 2 != 0) det *= -1;

  return { backSubstitution(A), det };
}

