# Push Relabel (Goldberg-Tarjan)

[Back](../docs/maxflows.md)

## アルゴリズム

## 計算量

$O(n^2 \sqrt{m})$

[実装](../cpp/push_relabel.hpp)

```cpp
#include <assert.h>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

template <class flow_t>
class push_relabel {
 public:
  push_relabel() : _n(0) {}
  push_relabel(int n) : _n(n), g(n), ex(n), label(n), relabel_count(0) {}

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

  flow_t flow(int s, int t) {
    assert(0 <= s && s < _n);
    assert(0 <= t && t < _n);
    assert(s != t);

    initialize(s, t);

    while (!active.empty()) {
      // v = active vertex
      auto p = active.top();
      active.pop();
      int v = p.second;
      if (ex[v] == 0 || v == s || v == t) continue;
      bool admissible = false;
      for (auto &e : g[v]) {
        // e is admissible <=> (e in G_f) and (label[v] = label[e.to] + 1)
        if (e.cap > 0 && label[v] == label[e.to] + 1) {
          push(v, e);
          admissible = true;
        }
      }
      if (!admissible) {
        relabel(v);
        relabel_count++;
        if (relabel_count % _n == 0) {
          global_relabel(s, t);
        }
      }
    }
    return ex[t];
  }

  void output_edges() const {
    for (const auto &e : get_edges()) {
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
  // ex[v] := (flow to v) - (flow from v)
  std::vector<flow_t> ex;
  std::vector<int> label;
  // (label[i], i)
  using P = std::pair<int, int>;
  // v is active <=> ex[v] > 0
  std::priority_queue<P> active;
  unsigned long long relabel_count;

  void initialize(int s, int t) {
    global_relabel(s, t);
    std::fill(ex.begin(), ex.end(), 0);

    for (auto &e : g[s]) {
      ex[s] += e.cap;
      push(s, e);
    }
  }

  void push(int from, _edge &e) {
    flow_t f = std::min(ex[from], e.cap);
    e.cap -= f;
    g[e.to][e.rev].cap += f;
    ex[from] -= f;
    ex[e.to] += f;

    if (ex[from] > 0) active.push({label[from], from});
    if (ex[e.to] > 0) active.push({label[e.to], e.to});
  }

  void relabel(int v) {
    int new_label = std::numeric_limits<int>::max();
    for (const auto &e : g[v]) {
      if (!e.cap) continue;
      new_label = std::min(new_label, label[e.to] + 1);
    }
    label[v] = new_label;

    if (ex[v] > 0) active.push({label[v], v});
  }

  void global_relabel(int s, int t) {
    std::queue<int> que({t});
    std::fill(label.begin(), label.end(), std::numeric_limits<int>::max() / 2);
    label[s] = _n;
    label[t] = 0;
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      for (const auto &e : g[v]) {
        if (e.to == s) continue;
        if (g[e.to][e.rev].cap > 0 && label[v] + 1 < label[e.to]) {
          label[e.to] = label[v] + 1;
          que.push(e.to);
        }
      }
    }
  }
};
```

## メモ

Global labeling: 定期的に label を計算する

## 参考文献

- [1] B.Korte J.Vygen, Combinatorial Optimization Theory and Algorithms 6th ed.
- [2] [Spaghetti Source 最大流 (Goldberg-Tarjan)](http://www.prefield.com/algorithm/graph/goldberg_tarjan.html)
- [3] [Push-Relabel による最大フローアルゴリズム](https://kenkoooo.hatenablog.com/entry/2016/12/22/143638)
