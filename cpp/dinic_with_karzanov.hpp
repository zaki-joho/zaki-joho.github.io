#ifndef ZAKI_DINIC_WITH_KARZANOV_HPP
#define ZAKI_DINIC_WITH_KARZANOV_HPP

#include <assert.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <tuple>
#include <vector>

template <class flow_t>
class dinic {
 public:
  dinic() : _n(0) {}
  dinic(int n) : _n(n), g(n), level(_n), frozen(_n), inv(_n), ex(_n) {}

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

    while (true) {
      bfs(s);
      if (level[t] == -1) break;
      karzanov(s, t);
    }
    return ex[t];
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
  std::vector<int> level, frozen, ord, inv;
  std::vector<flow_t> ex;
  // index of topological order
  std::priority_queue<int> active;
  using op = std::pair<_edge, flow_t>;

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

  void karzanov(int s, int t) {
    ord.clear();
    bool ok = topological_sort();
    assert(ok && (int)ord.size() == _n);
    for (int i = 0; i < _n; i++) {
      inv[ord[i]] = i;
    }
    std::fill(frozen.begin(), frozen.end(), 0);
    // e に沿って f 流した
    std::vector<std::stack<op>> ops(_n);
    for (auto& e : g[s]) {
      if (!e.cap) continue;
      if (level[s] + 1 != level[e.to]) continue;
      flow_t f = e.cap;
      e.cap = 0;
      g[e.to][e.rev].cap += f;
      ex[e.to] += f;

      ops[e.to].push({e, f});
      if (e.to != s && e.to != t && ex[e.to] > 0) active.push(inv[e.to]);
    }
    while (!active.empty()) {
      push(s, t, ops);
      balance(s, t, ops);
    }
  }

  void push(int s, int t, std::vector<std::stack<op>>& ops) {
    for (auto v : ord) {
      if (ex[v] == 0) continue;
      for (auto& e : g[v]) {
        if (!e.cap) continue;
        if (level[v] + 1 != level[e.to]) continue;
        if (frozen[e.to]) continue;
        flow_t f = std::min(ex[v], e.cap);
        if (!f) continue;
        e.cap -= f;
        g[e.to][e.rev].cap += f;
        ex[v] -= f;
        ex[e.to] += f;
        ops[e.to].push({e, f});

        if (v != s && v != t && ex[v] > 0) active.push(inv[v]);
        if (e.to != s && e.to != t && ex[e.to] > 0) active.push(inv[e.to]);
      }
    }
  }

  void balance(int s, int t, std::vector<std::stack<op>>& ops) {
    int v = ord[active.top()];
    active.pop();
    if (ex[v] == 0 || v == t) return;
    if (frozen[v]) return;
    while (!ops[v].empty() && ex[v] > 0) {
      auto op = ops[v].top();
      ops[v].pop();

      flow_t f = std::min(op.second, ex[v]);
      auto& re = g[op.first.to][op.first.rev];
      auto& e = g[re.to][re.rev];

      re.cap -= f;
      e.cap += f;
      ex[v] -= f;
      ex[re.to] += f;
      assert(!frozen[re.to]);

      if (re.to != s && re.to != t && ex[re.to] > 0) active.push(inv[re.to]);
    }
    frozen[v] = 1;
  }

  bool visit(int v, std::vector<int>& col) {
    col[v] = 1;
    for (const auto& e : g[v]) {
      if (!e.cap) continue;
      if (level[v] + 1 != level[e.to]) continue;

      if (col[e.to] == 2) continue;
      if (col[e.to] == 1) return false;
      if (!visit(e.to, col)) return false;
    }
    ord.push_back(v);
    col[v] = 2;
    return true;
  }

  bool topological_sort() {
    std::vector<int> col(_n);
    for (int v = 0; v < _n; v++) {
      if (!col[v] && !visit(v, col)) return false;
    }
    std::reverse(ord.begin(), ord.end());
    return true;
  }
};

#endif
