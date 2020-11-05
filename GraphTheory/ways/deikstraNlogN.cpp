
CLion
#include <algorithm>
#include <iostream>
#include <queue>
#include <limits>
#include <tuple>
#include <vector>

class IsLess {
public:

    bool operator () (
            std::tuple<size_t, ssize_t, size_t> &a,
            std::tuple<size_t, ssize_t, size_t> &b
    ) {
        return std::get<1>(a) < std::get<1>(b);
    }

};

ssize_t deikstra (
        std::vector < std::vector <std::pair <size_t, size_t > > >  &graph,
        size_t n,
        size_t k,
        size_t s,
        size_t f
) {
    std::vector <size_t> ways(n, std::numeric_limits<size_t>::max());
    std::priority_queue <
            std::tuple <size_t, ssize_t, size_t>,
            std::vector < std::tuple<size_t, ssize_t, size_t>>,
            IsLess
    > order ( (IsLess()) );
    ways[s] = 0;
    order.emplace(s, ways[s], 0);
    for (size_t i = 0; i < graph.size(); ++i) {
        size_t v = std::get<0>(order.top());
        size_t check = -std::get<1>(order.top());
        size_t step = std::get<2>(order.top());
        order.pop();
        if (check != ways[v] || step >= k) {
            continue;
        }
        if (ways[v] == std::numeric_limits<size_t>::max()) {
            break;
        }
        for (size_t j = 0; j < graph[v].size(); ++j) {
            if (check + graph[v][j].second < ways[graph[v][j].first]) {
                ways[graph[v][j].first] = check + graph[v][j].second;
                order.emplace(graph[v][j].first, -ways[graph[v][j].first], step + 1);
            }
        }
    }
    return ways[f] != std::numeric_limits<size_t>::max() ? ways[f] : -1;
}

void getInput (
        std::istream &in,
        std::vector < std::vector <std::pair <size_t, size_t > > > &graph,
        size_t &n,
        size_t &m,
        size_t &k,
        size_t &s,
        size_t &f
) {
    in >> n >> m >> k >> s >> f;
    --s;
    --f;
    graph.assign(n, std::vector <std::pair <size_t, size_t > > ());
    for (size_t i = 0; i < m; ++i) {
        size_t s_i, f_i, p_i;
        in >> s_i >> f_i >> p_i;
        graph[s_i - 1].emplace_back(f_i - 1,  p_i);
    }
}

void setOutput (
        std::ostream &out,
        ssize_t cost
) {
    out << cost;
}

int main() {
    std::vector < std::vector <std::pair <size_t, size_t > > > graph;
    size_t n, m, k, s, f;
    getInput(std::cin, graph, n, m, k, s, f);
    setOutput(std::cout, deikstra(graph, n, k, s, f));
    return 0;
}