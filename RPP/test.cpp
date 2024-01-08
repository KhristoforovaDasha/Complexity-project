#include "rpp.h"
#include <vector>
#include <benchmark/benchmark.h>
#include <ctime>

using std::vector;
using std::pair;
using std::max;

vector<vector<int>> generate_graph(int n, int p) {
  int max_dist = 10000;
  vector<vector<int>> res(n, vector<int>(n, 0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      int r = rand() % max_dist;
      res[i][j] = (r < max_dist * p / 100) ? INF : r;
      res[j][i] = res[i][j];
    }
  }
  return res;
}

vector<vector<pair<int, int>>> generate_components(int c, vector<vector<int>>& gr) {
  vector<vector<pair<int, int>>> res(c);
  vector<int> vertex_to_comp(gr.size(), -1);
  int count = 0;
  for (int i = 0; i < gr.size() * 100; ++i) {
    int v = rand() % gr.size();
    int u = rand() % gr.size();
    if (u != v && gr[u][v] < INF && vertex_to_comp[u] == -1 && vertex_to_comp[v] == -1) {
      res[count].emplace_back(u, v);
      vertex_to_comp[u] = count;
      vertex_to_comp[v] = count;
      ++count;
      if (count == c) break;
    }
  }

  for (int j = 0; j < gr.size() * 100; ++j) {
    int v = rand() % gr.size();
    int u = rand() % gr.size();
    if (u != v && gr[u][v] < INF && ((vertex_to_comp[u] == -1) ^ (vertex_to_comp[v] == -1))) {
      int comp = max(vertex_to_comp[u], vertex_to_comp[v]);
      res[comp].emplace_back(u, v);
      vertex_to_comp[u] = comp;
      vertex_to_comp[v] = comp;
    }
  }

  return res;
}

static void test(benchmark::State& state) {
  for (auto _ : state) {
    srand(time(0));

    int n = state.range(0);
    int p = state.range(1);
    auto gr = generate_graph(n, p);
    int c = state.range(2);
    auto components = generate_components(c, gr);

    RPP rpp(gr, components);

    int ans = INF;
    vector <pair<int, int>> cycle;
    rpp.find_cycle(ans, cycle);

    for (const auto &c: components) {
      for (auto [u, v]: c) {
        ans += gr[u][v];
      }
    }
  }
}

BENCHMARK(test)
->ArgsProduct({benchmark::CreateDenseRange(8, 25, 1), benchmark::CreateDenseRange(10, 90, 10), benchmark::CreateDenseRange(1, 6, 1)})
->Threads(8)
->Repetitions(1)
->Complexity();