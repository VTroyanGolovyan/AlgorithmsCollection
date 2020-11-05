/* Troyan-Golovyan Vladislav */
/* Contest 1, Task C */
#include <iostream>
#include <vector>
#include <utility>
#include <functional>
/* DFS */
void dfs (
        const std::vector< std::vector<size_t> > &graph,
        size_t v,
        std::vector<char> &colors,
        const std::function<void (const size_t)> &in_callback,
        const std::function<void (const size_t)> &out_callback
) {
    colors[v] = 1;
    in_callback(v);
    for (size_t i = 0; i < graph[v].size(); ++i) {
        if (colors[graph[v][i]] == 0) {
            dfs(graph, graph[v][i], colors, in_callback, out_callback);
        }
    }
    out_callback(v);
    colors[v] = 2;
}
/* Function for graph condensation */
void condense_graph (
        const std::vector< std::vector<size_t> > &graph,
        std::vector< std::vector<size_t> > &condensed_graph,
        std::vector<size_t> v_comp
) {
    for (size_t i = 0; i < graph.size(); ++i) {
        for (size_t j = 0; j < graph[i].size(); ++j) {
            if (v_comp[i] != v_comp[graph[i][j]]) {
                condensed_graph[v_comp[i]].push_back(v_comp[graph[i][j]]);
            }
        }
    }
}
/* Function for getting condensed oriented graph from */
void make_condensed_graph (
        const std::vector<std::vector<size_t>> &graph,
        const std::vector<std::vector<size_t>> &trans_graph,
        std::vector<std::vector<size_t>> &condensed_graph
) {
    std::vector<size_t> order;
    std::vector<char> colors(graph.size(), 0);
    for (size_t i = 0; i < graph.size(); ++i) {
        if (colors[i] == 0) {
            dfs(
                    graph, i, colors,
                    [&] (const size_t v) { },
                    [&] (const size_t v) {
                        order.push_back(v);
                    }
            );
        }
    }

    colors.assign(trans_graph.size(), 0);
    std::vector<std::vector<size_t>> components;
    std::vector<size_t> v_comp(graph.size(), 0);

    for (size_t i = 0; i < trans_graph.size(); ++i) {
        if (colors[order[graph.size() - i  - 1]] == 0) {
            std::vector<size_t> component;
            dfs(
                    trans_graph,
                    order[graph.size() - i  - 1],
                    colors,
                    [&] (const size_t v) {
                        component.push_back(v);
                        v_comp[v] = components.size();
                    },
                    [&] (const size_t v) {}
            );
            components.push_back(component);
        }
    }
    condensed_graph.clear();
    condensed_graph.assign(components.size(), std::vector<size_t> ());
    condense_graph(graph, condensed_graph, v_comp);
}
/* Finds min edges count we need to build to make your graph strongly connected  */
size_t count_roads (
        const std::vector< std::vector<size_t> > &graph,
        std::vector< std::pair<size_t, size_t> > &in_out
) {
    if (graph.size() == 1)
        return 0;
    for (size_t i = 0; i < graph.size(); ++i) {
        in_out[i].first += graph[i].size();
        for (size_t j : graph[i]) {
            in_out[j].second++;
        }
    }
    size_t count1 = 0;
    size_t count2 = 0;
    for (auto & deg : in_out) {
        if (deg.first == 0) {
            count1++;
        }
        if (deg.second == 0) {
            count2++;
        }
    }
    return count1 < count2 ? count2 : count1;
}
/* Get input data */
void get_input (
        size_t &n,
        size_t &m,
        std::vector< std::vector<size_t> > &graph,
        std::vector< std::vector<size_t> > &trans_graph
) {
    std::cin >> n >> m;
    graph.assign(n, std::vector<size_t> ());
    trans_graph.assign(n, std::vector<size_t> ());
    for (size_t i = 0; i < m; ++i) {
        size_t a, b;
        std::cin >> a >> b;
        graph[a - 1].push_back(b - 1);
        trans_graph[b - 1].push_back(a - 1);
    }
}
/* Set output data */
void set_output (size_t count) {
    std::cout << count;
}
/* Main function */
int main () {

    size_t n, m;
    std::vector< std::vector<size_t> > graph;
    std::vector< std::vector<size_t> > trans_graph;

    get_input(n, m, graph, trans_graph);

    std::vector< std::vector<size_t> > condensed_graph;
    make_condensed_graph(graph, trans_graph, condensed_graph);
    std::vector< std::pair<size_t, size_t> > in_out_counts(condensed_graph.size(), std::pair<size_t, size_t>(0, 0));

    set_output(count_roads(condensed_graph, in_out_counts));

    return 0;

}