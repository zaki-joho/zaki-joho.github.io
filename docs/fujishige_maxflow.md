# Fujishige's algorithm

[Back](../docs/maxflows.md)

## アルゴリズム

$\alpha = \max_{e\in E} u(e)$ とし, 始点から $\alpha$ 流せる頂点のみを使って終点に
流量 $\alpha$ のフローを流す. 流せない場合, $\alpha = \lfloor \frac{\alpha}{2} \rfloor$ とし, 繰り返す.

## 計算量

$O(nm\log u_{max})$

各 $\alpha$ で流せる流量は高々 $3\alpha n$ であることが示せる.

## [実装](https://github.com/zaki-joho/zaki-pages/blob/master/cpp/fujishige_maxflow.hpp)

```cpp
#include <assert.h>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

template <class flow_t>
class fujishige_maxflow {
 public:
  fujishige_maxflow() : _n(0) {}
  fujishige_maxflow(int n)
      : _n(n), g(n), vs(_n, _n + 1), b(_n, 0), cap(_n, 0) {}

  struct edge {
    int from, to;
    flow_t cap, flow;
  };

  int add_edge(int from, int to, flow_t _cap) {
    assert(0 <= from && from < _n);
    assert(0 <= to && to < _n);
    assert(0 <= _cap);
    int from_id = int(g[from].size()), to_id = int(g[to].size()) + (from == to);
    int m = int(pos.size());
    pos.push_back({from, from_id});
    g[from].push_back(_edge{to, to_id, _cap});
    g[to].push_back(_edge{from, from_id, 0});
    return m;
  }

  edge get_edge(int i) const {
    int m = int(pos.size());
    assert(0 <= i && i < m);
    auto _e = g[pos[i].first][pos[i].second];
    auto _re = g[_e.to][_e.rev];
    return edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap};
  }

  std::vector<edge> get_edges() const {
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

    flow_t flow = 0, f = 0;
    for (const auto &es : g) {
      for (const auto &e : es) {
        f = std::max(f, e.cap);
      }
    }
    // 1
    f = std::min(f, flow_limit);
    while (f > 0 && flow < flow_limit) {
      while (f > flow_limit - flow) {
        f /= 2;
      }
      if (augment(s, t, f)) {
        flow += f;
        continue;
      }
      f /= 2;
    }
    return flow;
  }

  flow_t flow(int s, int t) {
    return flow(s, t, std::numeric_limits<flow_t>::max());
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
  std::vector<int> vs;
  std::vector<flow_t> b, cap;

  bool augment(int s, int t, flow_t f) {
    // 2
    std::queue<int> que({s});
    std::vector<int> path;
    std::fill(vs.begin(), vs.end(), _n + 1);
    std::fill(b.begin(), b.end(), 0);
    auto update_vs = [&](int v) {
      vs[v] = path.size();
      path.push_back(v);
    };
    // 5
    while (!que.empty()) {
      int v = que.front();
      que.pop();
      update_vs(v);
      // 3
      for (const auto &e : g[v]) {
        if (!e.cap) continue;
        if (vs[e.to] != _n + 1) continue;
        if (b[e.to] < f && f <= b[e.to] + e.cap) {
          que.push(e.to);
        }
        b[e.to] += e.cap;
      }
      if (v != t) continue;
      // 6
      std::fill(cap.begin(), cap.end(), 0);
      cap[t] = f;
      assert(path.back() == t);
      while (path.size() > 1) {
        for (auto &e : g[path.back()]) {
          if (int(path.size()) <= vs[e.to]) continue;
          auto &re = g[e.to][e.rev];
          flow_t df = std::min(cap[path.back()], re.cap);
          re.cap -= df;
          e.cap += df;
          cap[re.to] -= df;
          cap[e.to] += df;
        }
        path.pop_back();
      }
      assert(path.back() == s);
      return true;
    }
    // 4
    return false;
  }
};
```

## メモ

## 参考文献

[1] B.Korte J.Vygen, Combinatorial Optimization Theory and Algorithms 6th ed.
