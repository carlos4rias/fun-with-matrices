#include <deque>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <queue>

using namespace std;


struct edge {
  int to, w;
  edge() {}
  edge(int a, int _w) : to(a), w(_w) {}
  bool operator < (const edge &e) const {
    return w < e.w;
  }
};

typedef edge edge;

class graph {
public:
  //using g = std::vector< std::vector<edge> >;
  std::vector< std::vector<edge> > G;
  const int oo = 100000000;
  int nodes;

  graph() {
    nodes = 0;
  }

  graph(int nnodes) {
    nodes = nnodes;
  }

  graph(char * filename) {
    nodes = 0;
    read_graph(filename);
  }

  void read_graph(char * filename) {
    std::ifstream infile(filename);

    std::string line, tmp;
    int u, v, w;
    while (std::getline(infile, line)) {
      std::istringstream ss(line);

      if (line[0] == 'p') {
        ss >> tmp >> tmp >> u >> tmp;
        nodes = u;
        G.resize(nodes);
        // std::cout << "FUCK" << std::endl;
      } else if (line[0] == 'a') {
        ss >> tmp >> u >> v >> w ;
        if (u > G.size()) std::cout << "FUCK :o "  <<  u<< std::endl;
        // std::cout << u << " " << v << " " << " " << w <<" "<< G.size() << std::endl;
        G[u - 1].emplace_back(edge(v - 1, w));
      }
    }
    infile.close();
    return;
  }

  void print_graph() {
    int node = 0;
    for (auto i : G) {
      std::cout << node++ << ": ";
      for (auto j : i) std::cout << j.to << "," << j.w << " ";
      std::cout << std::endl;
    }
  }

  int bfs (int node, std::vector<bool> &vis) {
    // std::cout << node << std::endl;
    std::queue<int> Q;
    Q.push(node);

    int ans = 0;
    vis[node] = true;
    while (!Q.empty()) {
      int nn = Q.front();
      ans++;
      Q.pop();
      for (int i = 0; i < G[nn].size(); i++) {
        int cur = G[nn][i].to;
        if (vis[cur]) continue;
        vis[cur] = true;
        // std::cout << nn << std::endl;
        Q.push(cur);
      }
    }
    return ans;
  }

  std::vector<int> get_components () {
    std::vector<bool> visited(nodes);
    std::vector<int> components;
    for (int i = 0; i < nodes; i++) {
      if (visited[i]) continue;
      components.push_back(bfs(i, visited));
    }
    return components;
  }

  void get_number_nodes (int node, graph &G) {
    cout  << G.G[node].size() << endl;
  }

  ~ graph(){}
};
