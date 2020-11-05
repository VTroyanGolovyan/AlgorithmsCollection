#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

enum {
   unused = 0,
   used = 1,
};

size_t primaMinOstovTree (
    size_t &n,
    size_t &m,
    std::vector < std::vector < std::pair <size_t, size_t> > > &graph
) {
    std::set < std::pair <size_t, size_t> > queue;
    std::vector <size_t> min_edges(n, std::numeric_limits<size_t>::max());
    std::vector <long long> end_edges(n, -1);
    std::vector <char> is_used(n, unused);
    min_edges[0] = 0;
    queue.emplace(0, 0);
    for (size_t i = 0; i < n; ++i) {
        size_t v = queue.begin()->second;
        is_used[v] = used;
        queue.erase(queue.begin());
        for (size_t j = 0; j < graph[v].size(); ++j) {
            if (is_used[graph[v][j].first] == unused && graph[v][j].second < min_edges[graph[v][j].first]) {
                queue.erase(std::pair <size_t, size_t> (min_edges[graph[v][j].first], graph[v][j].first));
                min_edges[graph[v][j].first] = graph[v][j].second;
                end_edges[graph[v][j].first] = v;
                queue.emplace(min_edges[graph[v][j].first], graph[v][j].first);
            }
        }
    }
    size_t cost = 0;
    for (size_t i = 0; i < n; ++i) {
        cost += min_edges[i];
    }
    return cost;
}

void getInput (
    std::istream &in,
    size_t &n,
    size_t &m,
    std::vector < std::vector < std::pair <size_t, size_t> > > &graph
) {
    in >> n >> m;
    graph.assign(n, std::vector < std::pair <size_t, size_t> > ());
    size_t from, to, weight;
    for (size_t i = 0; i < m; ++i) {
        in >> from >> to >> weight;
        if (from != to) {
            graph[from - 1].emplace_back(to - 1, weight);
            graph[to - 1].emplace_back(from - 1, weight);
        }
    }
}

void setOutput (
    std::ostream &out,
    size_t size
) {
    out << size;
}

int main () {
    size_t n, m;
    std::vector < std::vector < std::pair <size_t, size_t> > > graph;
    getInput(std::cin, n, m, graph);
    setOutput(std::cout, primaMinOstovTree(n, m, graph));
    return 0;
}