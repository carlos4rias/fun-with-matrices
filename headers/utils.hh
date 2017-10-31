#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>



using Matrix = std::vector< std::vector<int> >;

Matrix read_matrix(char * filename) {
  std::fstream filein(filename);
  std::string line, tmp;
  Matrix graph;
  int n_nodes, u, v, w;

  while (std::getline(filein, line)) {
    std::istringstream ss(line);
    if (line[0] == 'p') {
      ss >> tmp >> tmp >> n_nodes >> tmp;
      graph = Matrix(n_nodes, std::vector<int>(n_nodes, 1000000000));
    } else if (line[0] == 'e') {
      ss >> tmp >> u >> v >> w;
      // ss >> tmp >> u >> v;
      graph[u - 1][v - 1] = w;
      // graph[u - 1][v - 1] = 1;
    }
  }
  filein.close();
  return graph;
}
