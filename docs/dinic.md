# Dinic's algorithm

[Back](../docs/maxflows.md)

## アルゴリズム

残余グラフ上で最短経路グラフを求め, 最短経路グラフ上で増加路を選べなくなるまでフローを流す.
同一の最短経路グラフを用いる期間を phase と呼び, ここで流されるフローをブロッキングフローと呼ぶ.(不正確)

## 計算量

$O(m n^2)$

phase 毎に最短経路長が単調増加であることが示せて, phase 数は高々 $n$.
dfs で増加路を見つけていて, 各 phase が $O(nm)$.

## [実装](https://github.com/zaki-joho/zaki-pages/blob/master/cpp/dinic.hpp)

```cpp
#include <assert.h>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

template <class flow_t>
class dinic {
 public:
  dinic() : _n(0) {}
  dinic(int n) : _n(n), g(n), level(_n), iter(_n) {}

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
      bfs(s);
      if (level[t] == -1) break;
      std::fill(iter.begin(), iter.end(), 0);
      flow_t f = dfs(s, t, flow_limit - flow);
      if (!f) break;
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
  std::vector<int> level, iter;

  void bfs(int s) {
    std::fill(level.begin(), level.end(), -1);
    std::queue<int> que;
    level[s] = 0;
    que.push(s);
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      for (const auto& e : g[v]) {
        if (e.cap > 0 && level[e.to] < 0) {
          level[e.to] = level[v] + 1;
          que.push(e.to);
        }
      }
    }
  }

  flow_t dfs(int v, int t, flow_t f) {
    if (v == t) return f;
    int level_v = level[v];
    for (int i = iter[v]; i < (int)g[v].size(); i++) {
      auto& e = g[v][i];
      if (e.cap > 0 && level_v < level[e.to]) {
        flow_t d = dfs(e.to, t, std::min(f, e.cap));
        if (d > 0) {
          e.cap -= d;
          g[e.to][e.rev].cap += d;
          return d;
        }
      }
    }
    return 0;
  }
};
```

## メモ

## 参考文献

[1] B.Korte J.Vygen, Combinatorial Optimization Theory and Algorithms 6th ed.
