// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A

#include "../cpp/push_relabel.hpp"

using namespace std;

int main() {
  int V, E;
  cin >> V >> E;
  push_relabel<int> flow(V);
  for (int i = 0; i < E; i++) {
    int u, v, c;
    cin >> u >> v >> c;
    flow.add_edge(u, v, c);
  }
  cout << flow.flow(0, V - 1) << endl;
  return 0;
}
