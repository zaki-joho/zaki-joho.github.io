# Edmonds-Karp

[Back](../docs/maxflows.md)

## アルゴリズム

## 計算量

$O(m^2 n)$

## [実装](https://github.com/zaki-joho/zaki-pages/blob/master/cpp/edmonds_karp.hpp)

```c++
#include <assert.h>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <vector>

template <class flow_t>
class edmonds_karp {
 public:
  edmonds_karp() : _n(0) {}
  edmonds_karp(int n) : _n(n), g(n), prev(_n) {}

  struct edge {
    int from, to;
    flow_t cap, flow;
  };

  int add_edge(int from, int to, flow_t cap) {
    assert(0 <= from && from < _n);
    assert(0 <= to && to < _n);
    assert(0 <= cap);
    int from_id = int(g[from].size()), to_id = int(g[to].size()) + (from == to);
    int m = int(pos.size());
    pos.push_back({from, from_id});
    g[from].push_back(_edge{to, to_id, cap});
    g[to].push_back(_edge{from, from_id, 0});
    return m;
  }

  edge get_edge(int i) {
    int m = int(pos.size());
    assert(0 <= i && i < m);
    auto _e = g[pos[i].first][pos[i].second];
    auto _re = g[_e.to][_e.rev];
    return edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap};
  }

  std::vector<edge> get_edges() {
    std::vector<edge> edges;
    for (int i = 0; i < int(pos.size()); i++) {
      edges.emplace_back(get_edge(i));
    }
    return edges;
  }

  flow_t flow(int s, int t, flow_t flow_limit) {
    assert(0 <= s && s < _n);
    assert(0 <= t && t < _n);
    assert(s != t);

    flow_t flow = 0;
    while (flow < flow_limit) {
      bfs(s, t);
      if (prev[t].first < 0) return flow;
      flow_t f = flow_limit;
      for (int v = t; prev[v].first != v; v = prev[v].first) {
        f = std::min(f, g[prev[v].first][prev[v].second].cap);
      }
      for (int v = t; prev[v].first != v; v = prev[v].first) {
        auto& e = g[prev[v].first][prev[v].second];
        e.cap -= f;
        g[e.to][e.rev].cap += f;
      }
      flow += f;
    }
    return flow;
  }

  flow_t flow(int s, int t) {
    return flow(s, t, std::numeric_limits<flow_t>::max());
  }

  void output_edges() const {
    for (const auto& e : get_edges()) {
      std::cerr << e.from << "->" << e.to << " : " << e.flow << "/" << e.cap
                << std::endl;
    }
  }

 private:
  int _n;
  struct _edge {
    int to, rev;
    flow_t cap;
  };
  std::vector<std::pair<int, int>> pos;
  std::vector<std::vector<_edge>> g;
  std::vector<std::pair<int, int>> prev;

  void bfs(int s, int t) {
    // vertex, index of the edge
    std::fill(prev.begin(), prev.end(), std::pair<int, int>(-1, -1));
    std::queue<int> que;
    prev[s] = {s, -1};
    que.push(s);
    while (!que.empty() && prev[t].first < 0) {
      int v = que.front();
      que.pop();
      for (int i = 0; i < (int)g[v].size(); i++) {
        const auto e = g[v][i];
        if (e.cap > 0 && prev[e.to].first < 0) {
          prev[e.to] = {v, i};
          que.push(e.to);
        }
      }
    }
  }
};
```

## メモ
