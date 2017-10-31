#include <deque>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>
#include <iomanip>
#include <unordered_map>

#include "thread_pool.hh"

template <class T>
class sparse_matrix {
public:
  using compressed_matrix = std::vector< std::map<T, T> >;
  compressed_matrix CSR, CSC;
  int n_cols, n_rows, oo;

  sparse_matrix() {
    oo = 100000000;
    n_cols = 0;
    n_rows = 0;
    CSC = compressed_matrix(n_cols);
    CSR = compressed_matrix(n_rows);
  }

  sparse_matrix(int rows, int cols) {
    oo = 100000000;
    n_rows = rows;
    n_cols = cols;
    CSC = compressed_matrix(n_cols);
    CSR = compressed_matrix(n_rows);
  }

  sparse_matrix(char * filename) {
    oo = 100000000;
    read_matrix(filename);
  }

  void read_matrix(char * filename) {
    std::ifstream infile(filename);

    std::string line, tmp;
    int u, v, w;

    while (std::getline(infile, line)) {
      std::istringstream ss(line);

      if (line[0] == 'p') {
        ss >> tmp >> tmp >> n_cols >> tmp;
        n_rows = n_cols;
        CSC = compressed_matrix(n_rows);
        CSR = compressed_matrix(n_rows);

      } else if (line[0] == 'a') {
        ss >> tmp >> u >> v >> w ;
        CSR[u - 1][v - 1] = w;
        CSC[v - 1][u - 1] = w;
      }
    }

    for (int i = 0; i < n_rows; i++) {
      CSR[i][i] = 0;
      CSC[i][i] = 0;
    }

    infile.close();
    return;
  }

  void print_matrix() {
    int it = 0;
    for (auto i : CSR) {
      std::cout << it++ << " : ";
      for (auto &j : i) {
        std::cout << j.first << "," << j.second << " ";
      }
      std::cout << std::endl;
    }
  }


  void diamond(const sparse_matrix &B) {
    sparse_matrix<int> C(n_rows, B.n_cols);
    for (int i = 0; i < n_rows; i++) {
      for (int j = 0; j < B.n_cols; j++) {
        auto mit = CSR[i].begin();
        auto mjt = B.CSC[j].begin();
        int mmin = oo;
        while (mit != CSR[i].end() && mjt != B.CSC[j].end()) {
          int vi = mit->first;
          int vj = mjt->first;
          if (vi == vj) {
            mmin = std::min(mmin, mit->second + mjt->second);
            mit++; mjt++;
          } else if(vi < vj) { mit++; }
            else { mjt++; }
        }
        if (mmin < oo) {
          C.CSR[i][j] = mmin;
          C.CSC[j][i] = mmin;
        }
      }
    }
    swap(CSR, C.CSR);
    swap(CSC, C.CSC);
  }

  void parallel_diamond (sparse_matrix &B, thread_pool &pool) {
    sparse_matrix<int> C(n_rows, B.n_cols);
    for (int i = 0; i < n_rows; i++) {
      pool.AddJob (
        [&B, &C, i, this]() {diamond_row(std::ref(B), std::ref(C), i);}
      );
    }
    pool.WaitAll();
    swap(CSR, C.CSR);
    swap(CSC, C.CSC);
  }

  void diamond_row(sparse_matrix &B, sparse_matrix &C, int row) {
    for (int j = 0; j < B.n_cols; j++) {
      auto mit = CSR[row].begin();
      auto mjt = B.CSC[j].begin();
      int mmin = oo;
      while (mit != CSR[row].end() && mjt != B.CSC[j].end()) {
        int vi = mit->first;
        int vj = mjt->first;
        if (vi == vj) {
          mmin = std::min(mmin, mit->second + mjt->second);
          mit++; mjt++;
        } else if(vi < vj) { mit++; }
          else { mjt++; }
      }
      if (mmin < oo) {
        C.CSR[row][j] = mmin;
        C.CSC[j][row] = mmin;
      }
    }
  }

  ~ sparse_matrix(){}
};
