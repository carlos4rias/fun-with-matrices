#include "../headers/utils.hh"
#include "../headers/timer.hh"
#include "../headers/graph.hh"
// #include "../headers/thread_pool.hh"
// #include "safe_deque.hh"

#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>

using namespace std;

int main(int argc, char ** argv) {
  if (argc != 2) {
    cout << "Use: $ program graph_file" << endl;
    return 0;
  }

  graph G(argv[1]);
  G.print_graph();

  //get not really all the components but the cluster of nodes that are
  //almost connected its made by an BFS
  vector<int> components = G.get_components();
  cout << components.size() << " components and their sizes are: ";
  for (auto i : components) cout << i << " "; cout << endl;

}
