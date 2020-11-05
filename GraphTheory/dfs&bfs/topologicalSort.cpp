
CLion
/* Troyan-Golovyan Vladislav*/
/* Contest 1, Task B*/
#include <iostream>
#include <vector>
#include <functional>
/* DFS */
void dfs (
    const std::vector< std::vector<size_t> > &graph,
    std::vector <char> &colors,
    size_t v,
    const std::function < void(size_t) > &in_callback,
    const std::function < void(size_t) > &out_callback,
    const std::function < bool(size_t) > &call_dfs_checker
) {
    colors[v] = 1;
    in_callback(v);
    for (size_t i = 0; i < graph[v].size(); ++i) {
        if (call_dfs_checker(graph[v][i])) {
            dfs(graph, colors, graph[v][i], in_callback, out_callback, call_dfs_checker);
        }
    }
    out_callback(v);
    colors[v] = 2;
}
/* Topological sort */
bool top_sort (const std::vector<std::vector<size_t>> &graph, std::vector<size_t> &sort_result) {
    std::vector<char> colors(graph.size(), 0);
    sort_result.clear();
    bool res = true;
    for (size_t i = 0; i < graph.size() && res; ++i) {
        if (colors[i] == 0) {
            dfs(
                 graph,
                 colors,
                 i,
                 [&] (size_t v) {

                 },
                 [&] (size_t v) {
                      sort_result.push_back(v);
                 },
                 [&] (size_t v) -> bool {
                     if (colors[v] == 1) {
                         res = false;
                     }
                     return colors[v] == 0;
                 }

            );
        }
    }
    return res;
}
/* Get input */
void get_input (
        size_t &n,
        size_t &m,
        std::vector< std::vector<size_t> > &graph
) {
    std::cin >> n >> m;
    graph.assign(n, std::vector <size_t> ());
    for (size_t i = 0; i < m; ++i) {
        size_t a, b;
        std::cin >> a >> b;
        graph[a].push_back(b);
    }
}
/* Set output */
void set_output (
    const bool res,
    std::vector <size_t> sort_result
) {
    if (res) {
        std::cout << "YES" << std::endl;
        for (size_t i = 0; i < sort_result.size(); ++i) {
            std::cout << sort_result[sort_result.size() - i - 1] << " ";
        }
    } else {
        std::cout << "NO";
    }
}
/* Main function */
int main() {

    size_t n, m;
    std::vector< std::vector<size_t> > graph;

    get_input(n, m, graph);

    std::vector <size_t> sort_result;

    bool res = top_sort(graph, sort_result);

    set_output(res, sort_result);

    return 0;

}