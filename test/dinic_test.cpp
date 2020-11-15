// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A

#include "../cpp/dinic.hpp"

#include <iostream>

using namespace std;

int main() {
  int V, E;
  cin >> V >> E;
  dinic<int> flow(V);
  for (int i = 0; i < E; i++) {
    int u, v, c;
    cin >> u >> v >> c;
    flow.add_edge(u, v, c);
  }
  cout << flow.flow(0, V - 1) << endl;
  return 0;
}
