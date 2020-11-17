#include <cxxabi.h>
#include <string>
#include "../cpp/dinic.hpp"
#include "../cpp/dinic_with_karzanov.hpp"
#include "../cpp/edmonds_karp.hpp"
#include "../cpp/ford_fulkerson.hpp"
#include "../cpp/fujishige_maxflow.hpp"
#include "../cpp/push_relabel.hpp"
#include "../cpp/utils.hpp"

template <class flow_t>
struct problem_instance {
  int V, E;
  int source, sink;
  // 0-index
  std::vector<int> u, v;
  std::vector<flow_t> c;
};

template <class maxflow, class flow_t>
struct maxflow_solver {
  flow_t solve(const problem_instance<flow_t> &ins) {
    maxflow solver(ins.V);
    assert(ins.E == (int)ins.u.size());
    assert(ins.E == (int)ins.v.size());
    assert(ins.E == (int)ins.c.size());
    for (int i = 0; i < ins.E; i++) {
      solver.add_edge(ins.u[i], ins.v[i], ins.c[i]);
    }
    return solver.flow(ins.source, ins.sink);
  }

  void solve_and_report(const problem_instance<flow_t> &ins) {
    int status;
    auto name = abi::__cxa_demangle(typeid(this).name(), 0, 0, &status);
    stop_watch watch;
    watch.start();
    auto f = solve(ins);

    std::cerr << name << std::endl;
    std::cerr << "  answer : " << f << std::endl;
    std::cerr << "  elapsed: " << watch.stop() << " (msec)" << std::endl;
  }
};

template <class flow_t>
problem_instance<flow_t> generate_sparse_random() {
  xor_shift rnd(114514);
  const int MAX_V = 10000, MAX_E = 100000;
  const flow_t MAX_C = 100000;
  const int source = 0, sink = MAX_V - 1;
  std::vector<int> u(MAX_E), v(MAX_E);
  std::vector<flow_t> c(MAX_E);
  for (int i = 0; i < MAX_E; i++) {
    int a = rnd() % MAX_V, b = rnd() % MAX_V;
    if (a == b) continue;
    u[i] = a;
    v[i] = b;
    c[i] = rnd.rand_int(1, MAX_C);
  }
  problem_instance<flow_t> ins{MAX_V, MAX_E, source, sink, u, v, c};
  return ins;
}

template <class flow_t>
problem_instance<flow_t> generate_dense_random() {
  xor_shift rnd(114514);
  const double prob = 0.5;
  const int MAX_V = 1000;
  const flow_t MAX_C = 10000;
  const int source = 0, sink = MAX_V - 1;
  std::vector<int> u, v;
  std::vector<flow_t> c;
  for (int s = 0; s < MAX_V; s++) {
    for (int t = s + 1; t < MAX_V; t++) {
      if (rnd.rand_double() > prob) {
        continue;
      }
      int from = s, to = t;
      if (rnd() % 2) std::swap(from, to);
      u.push_back(from);
      v.push_back(to);
      c.push_back(rnd.rand_int(1, MAX_C));
    }
  }
  const int E = u.size();
  problem_instance<flow_t> ins{MAX_V, E, source, sink, u, v, c};
  return ins;
}

void test_sparse_random(int testcases) {
  for (int c = 0; c < testcases; c++) {
    using flow_t = int;
    auto ins = generate_sparse_random<flow_t>();
    std::cerr << "#vertex: " << ins.V << std::endl;
    std::cerr << "#edge  : " << ins.E << std::endl;
    {
      maxflow_solver<dinic<flow_t>, flow_t> dinic_solver;
      dinic_solver.solve_and_report(ins);
    }
    {
      maxflow_solver<dinic_with_karzanov<flow_t>, flow_t> karzanov_solver;
      karzanov_solver.solve_and_report(ins);
    }
    {
      maxflow_solver<edmonds_karp<flow_t>, flow_t> edmonds_solver;
      edmonds_solver.solve_and_report(ins);
    }
    {
        // very slow!
        // maxflow_solver<ford_fulkerson<flow_t>, flow_t> ford_solver;
        // ford_solver.solve_and_report(ins);
    } {
      maxflow_solver<fujishige_maxflow<flow_t>, flow_t> fujishige_solver;
      fujishige_solver.solve_and_report(ins);
    }
    {
      maxflow_solver<push_relabel<flow_t>, flow_t> tarjan_solver;
      tarjan_solver.solve_and_report(ins);
    }
  }
}

void test_dense_random(int testcases) {
  for (int c = 0; c < testcases; c++) {
    using flow_t = long long;
    auto ins = generate_dense_random<flow_t>();
    std::cerr << "#vertex: " << ins.V << std::endl;
    std::cerr << "#edge  : " << ins.E << std::endl;
    {
      maxflow_solver<dinic<flow_t>, flow_t> dinic_solver;
      dinic_solver.solve_and_report(ins);
    }
    {
      maxflow_solver<dinic_with_karzanov<flow_t>, flow_t> karzanov_solver;
      karzanov_solver.solve_and_report(ins);
    }
    {
      maxflow_solver<edmonds_karp<flow_t>, flow_t> edmonds_solver;
      edmonds_solver.solve_and_report(ins);
    }
    {
        // very slow!
        // maxflow_solver<ford_fulkerson<flow_t>, flow_t> ford_solver;
        // ford_solver.solve_and_report(ins);
    } {
      maxflow_solver<fujishige_maxflow<flow_t>, flow_t> fujishige_solver;
      fujishige_solver.solve_and_report(ins);
    }
    {
      maxflow_solver<push_relabel<flow_t>, flow_t> tarjan_solver;
      tarjan_solver.solve_and_report(ins);
    }
  }
}

int main() {
  test_sparse_random(1);
  test_dense_random(1);
}
