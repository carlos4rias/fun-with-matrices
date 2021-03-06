#include "../headers/utils.hh"
#include "../headers/timer.hh"
#include <iostream>
#include <vector>
#include <cassert>
#include <thread>

using namespace std;

using Matrix = std::vector< std::vector<int> >;

void dot(const Matrix &A, const Matrix &B, int row, int col, vector<int> & C) {
  int limit = B.size();
  for (int i = 0; i < limit; i++) {
    C[col] += A[row][i] * B[i][col];
  }
}

void matrix_multiplication(const Matrix &A, const Matrix &B, Matrix &C) {
  assert(A[0].size() == B.size());

  int a_rows = (int) A.size();
  int a_cols = (int) A[0].size();
  int b_rows = (int) B.size();
  int b_cols = (int) B[0].size();

  vector<thread> ts(b_rows * a_cols);

  for (int i = 0; i < a_rows; i++) {
    for (int j = 0; j < b_cols; j++) {
      ts[i * a_rows + j] = (thread(&dot, cref(A), cref(B), i, j, ref(C[i])));
    }
  }
  for (auto &th : ts) th.join();
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
