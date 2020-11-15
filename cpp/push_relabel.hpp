#ifndef ZAKI_PUSH_RELABEL_HPP
#define ZAKI_PUSH_RELABEL_HPP

#include <assert.h>

#include <limits>
#include <vector>

template <class flow_t>
class push_relabel {
 public:
  push_relabel() : _n(0) {}
  push_relabel(int n) : _n(n), g(n), ex(n), label(n) {}

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

    for (auto &e : g[s]) {
      g[e.to][e.rev].cap += e.cap;
      ex[s] += e.cap;
      e.cap = 0;
    }

    std::fill(level.begin(), level.end(), 0);
    level[s] = _n;

    flow_t flow = 0;
    while(true /* active な頂点が存在 */){
        // v = active vertex
        if(true /* admissible な e in g[v] が存在 */){
            // push(e)
        }
        else{
            // relabel(v)
        }
    }
    return flow;
  }

  flow_t flow(int s, int t) {
    return flow(s, t, std::numeric_limits<flow_t>::max());
  }

 private:
  int _n;
  struct _edge {
    int to, rev;
    flow_t cap;
  };
  std::vector<std::pair<int, int>> pos;
  std::vector<std::vector<_edge>> g;
  std::vector<flow_t> ex;
  std::vector<int> label;

  void push(int from, _edge &e) {
      flow_t f = std::min(ex[from], e.cap);
      e.cap -= f:
      g[e.to][e.rev].cap += f;
      ex[from] -= f;
      ex[e.to] += f;
  }

  void relabel(int v){
      int label_min = std::numeric_limits<int>::max();
      for(const auto &e:g[v]){
          if(!e.cap) continue;
          label_min = std::min(label_min, label[e.to]*1);
      }
      label[v] = label_min;
  }
};

#endif
