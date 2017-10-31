#include "../headers/timer.hh"
#include "../headers/sparse_rcw.hh"

#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>

using namespace std;


void log_matrix_diamond(sparse_matrix<int> &a, int pot) {
  sparse_matrix<int> res = a;
  while (pot > 0) {
    if (pot & 1) res.diamond(a);
    a.diamond(a);
    pot >>= 1;
  }
  swap(a, res);
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    cout << "Use: $ program graph_file" << endl;
    return 0;
  }

  {
    Timer tmt("sparse naive diamond graph");
    sparse_matrix<int> sma(argv[1]), smb;
    log_matrix_diamond(sma, sma.n_rows - 1);
    sma.print_matrix();
  }

}
