#pragma once

#include <vector>
#include <unordered_set>

using std::vector;
using std::pair;
using std::unordered_set;
const int INF = 100000007;

class RPP {
public:
  RPP(const vector <vector<int>>& graph, const vector <vector<pair < int, int>>>& components) :
  graph_(graph) {
    floyd();
    set_components_vertex(components);
    set_degrees(components);
  }

  void find_cycle(int& ans, vector <pair<int, int>> cycle, int i = 1) {
    if (cycle.empty()) {
      cycle = vector <pair<int, int>>(components_vertex_.size());
    }
    if (i == components_vertex_.size()) {
      int cur = 0;
      for (auto [u, v]: cycle) {
        cur += graph_[u][v];
      }
      cur += hungarian_method();
      ans = std::min(ans, cur);
      return;
    }
    for (int u = 0; u < components_vertex_[i].size(); ++u) {
      for (int j = 0; j < i; ++j) {
        for (int v = 0; v < components_vertex_[j].size(); ++v) {
          cycle[i] = {u, v};
          ++degrees_[u];
          ++degrees_[v];
          find_cycle(ans, cycle, i + 1);
          --degrees_[u];
          --degrees_[v];
        }
      }
    }
  }

private:
  void set_components_vertex(const vector <vector<pair < int, int>>>& components) {
    components_vertex_.resize(components.size());
    for (size_t i = 0; i < components.size(); ++i) {
      unordered_set<int> set;
      for (auto [u, v]: components[i]) {
        set.insert(u);
        set.insert(v);
      }
      for (int v: set) {
        components_vertex_[i].push_back(v);
      }
    }
  }

  void set_degrees(const vector <vector<pair < int, int>>>& components) {
    degrees_.assign(graph_.size(), 0);
    for (const auto& c: components) {
      for (auto [u, v]: c) {
        ++degrees_[u];
        ++degrees_[v];
      }
    }
  }

  void floyd() {
    size_t n = graph_.size();
    for (int k = 0; k < n; ++k)
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
          graph_[i][j] = std::min(graph_[i][j], graph_[i][k] + graph_[k][j]);
        }
  }

  int hungarian_method() {
    int n = graph_.size();
    vector <vector<int>> graph(n + 1, vector<int>(n + 1));
    for (int i = 1; i < graph.size(); ++i) {
      for (int j = 1; j < graph.size(); ++j) {
        if (i == j) {
          graph[i][j] = INF;
        } else {
          graph[i][j] = ((((degrees_[i - 1] % 2) == 0) || ((degrees_[j - 1] % 2) == 0)) ? INF : graph_[i - 1][j - 1]);
        }
      }
    }
    int m;
    n = m = graph.size();
    std::vector<int> potential_1side(n), potential_2side(m), matching(m), way(m);
    for (int i = 1; i < n; ++i) {
      matching[0] = i;
      int j0 = 0;
      std::vector<int> minv(m, INF + 1);
      std::vector<char> used(m, false);
      do {
        used[j0] = true;
        int i0 = matching[j0], delta = INF + 1, j1;
        for (int j = 1; j < m; ++j)
          if (!used[j]) {
            int cur = graph[i0][j] - potential_1side[i0] - potential_2side[j];
            if (cur < minv[j]) {
              minv[j] = cur;
              way[j] = j0;
            }
            if (minv[j] < delta) {
              delta = minv[j];
              j1 = j;
            }
          }
        for (int j = 0; j < m; ++j)
          if (used[j]) {
            potential_1side[matching[j]] += delta;
            potential_2side[j] -= delta;
          }
          else {
            minv[j] -= delta;
          }
        j0 = j1;
      } while (matching[j0] != 0);
      do {
        int j1 = way[j0];
        matching[j0] = matching[j1];
        j0 = j1;
      } while (j0);
    }
    int ans = 0;
    for (int j = 1; j < m; ++j) {
      ans += (graph[matching[j]][j] == INF ? 0 : graph[matching[j]][j]);
    }
    return ans / 2;
  }

  vector <vector<int>> graph_;
  vector<int> degrees_;
  vector <vector<int>> components_vertex_;

};
