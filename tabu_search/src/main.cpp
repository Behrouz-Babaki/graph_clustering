#include <iostream>
#include <vector>
#include "reader.hpp"

using std::cout;
using std::endl;
using std::vector;

int main(int argc, char** argv) {
  GraphReader gr(argv[1]);
  vector< vector< int > > g = gr.get_graph();
  
  for (auto u : g) {
    for (auto v : u)
      cout << v << " ";
    cout << endl;
  }
  return 0;
}
