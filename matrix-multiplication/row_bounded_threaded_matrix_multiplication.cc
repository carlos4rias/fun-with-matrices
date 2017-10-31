#include "../headers/utils.hh"
#include "../headers/timer.hh"
#include <iostream>
#include <vector>
#include <cassert>
#include <thread>

using namespace std;

using Matrix = std::vector< std::vector<int> >;

const int chunk_sz = 8;

void dot(const Matrix &A, const Matrix &B, int row, int b_cols, Matrix &C) {

  int limit = B.size();
  for (int j = 0; j < b_cols; j++) {
    for (int i = 0; i < limit; i++) {
      C[row][j] += A[row][i] * B[i][j];
    }
  }
}

void matrix_multiplication(const Matrix &A, const Matrix &B, Matrix &C) {
  assert(A[0].size() == B.size());

  int a_rows = (int) A.size();
  // int a_cols = (int) A[0].size();
  // int b_rows = (int) B.size();
  int b_cols = (int) B[0].size();

  vector<thread> ts(chunk_sz);

  for (int i = 0; i < a_rows; i++) {
    int it = 0;
    for (it = 0; i < a_rows && it < chunk_sz; it++) {
      ts[it] = thread(&dot, cref(A), cref(B), i, b_cols, ref(C));
      i++;
    }
    i--;
    for (int j = 0; j < it; j++)
      ts[j].join();
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    cout << "Use: $ program graph_file" << endl;
    return 0;
  }
  Matrix A = read_matrix(argv[1]), B = A, C;
  C = Matrix((int)A[0].size(), vector<int>((int) B.size()));
  {
    Timer t("naive mult");
    matrix_multiplication(A, B, C);
  }

  for (auto i : C) {
    for (auto j : i) cout << j << " ";
    cout << endl;
  }
}
