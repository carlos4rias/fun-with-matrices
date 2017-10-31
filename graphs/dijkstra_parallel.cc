#include "../headers/utils.hh"
#include "../headers/timer.hh"
#include "../headers/graph.hh"
#include "../headers/thread_pool.hh"
// #include "safe_deque.hh"

#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include <map>

using namespace std;

int main(int argc, char ** argv) {
  if (argc != 2) {
    cout << "Use: $ program graph_file" << endl;
    return 0;
  }

  graph G(argv[1]);
  G.print_graph();

  unordered_map<int, map<int, int> > assp;

  thread_pool pool(thread::hardware_concurrency());
  for (int i = 0; i < G.nodes; i++) {
    pool.AddJob(
      [i, &assp, &G] () {G.dijkstra(i, ref(assp));}
    );
  }

  pool.WaitAll();
  cout << "shortest path from each node to every";
  cout << "node that belongs to the graph" << endl;
  for (auto i : assp) {
    cout << i.first << ": ";
    for (auto j : i.second) {
      cout << j.first << "," << j.second << " :: ";
    }
    cout << endl;
  }
}
