#include "../headers/timer.hh"
#include "../headers/sparse_rcw.hh"

#include "../headers/thread_pool.hh"

#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>

using namespace std;


void log_matrix_diamond(sparse_matrix<int> &a, int pot) {
  thread_pool pool(thread::hardware_concurrency());

  sparse_matrix<int> res = a;
  while (pot > 0) {
    if (pot & 1) res.parallel_diamond(a, pool);
    a.parallel_diamond(a, pool);
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
    Timer tmt("sparse parallel diamond graph");
    sparse_matrix<int> sma(argv[1]), smb;
    log_matrix_diamond(sma, sma.n_rows - 1);
    sma.print_matrix();
  }
}
