#include <iostream>
#include <vector>
#include <queue>
/* BFS */
void bfs (const std::vector <std::vector <size_t> > &graph, const size_t &v, const size_t &f, std::vector<ssize_t> &ways) {
    std::queue<size_t> q;
    q.push(v);
    ways[v] = 0;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (size_t u : graph[current]) {
            if (ways[u] == -1) {
                q.push(u);
                ways[u] = ways[current] + 1;
            }
        }
    }
}
/* Find ways count */
size_t find_way_count (
    const std::vector<ssize_t> &matilda_ways,
    const std::vector<ssize_t> &leon_ways,
    const std::vector<ssize_t> &milk_ways
) {
    size_t min = matilda_ways[0] + milk_ways[0] + leon_ways[0];
    for (size_t i = 0; i < matilda_ways.size(); i++) {
       if (matilda_ways[i] + milk_ways[i] + leon_ways[i] < min) {
           min = matilda_ways[i] + milk_ways[i] + leon_ways[i];
       }
    }
    return min;
}
/* Get input */
void get_input (
    std::vector <std::vector <size_t> > &graph,
    size_t &n,
    size_t &m,
    size_t &matilda,
    size_t &leon,
    size_t &milk
) {
    std::cin >> n >>  m >> leon >> matilda >> milk;
    graph.assign(n, std::vector<size_t> ());
    int a, b;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b;
        graph[a - 1].push_back(b - 1);
        graph[b - 1].push_back(a - 1);
    }
}
/* Set output */
void set_output(size_t count) {
    std::cout << count;
}
/* Main function */
int main() {
    size_t n, m, matilda, leon, milk;
    std::vector <std::vector <size_t> > graph;
    get_input(graph, n, m, matilda, leon, milk);

    std::vector<ssize_t> matilda_ways(graph.size(), -1);
    bfs(graph, matilda - 1, milk - 1, matilda_ways);
    std::vector<ssize_t> leon_ways(graph.size(), -1);
    bfs(graph, leon - 1, milk - 1, leon_ways);
    std::vector<ssize_t> milk_ways(graph.size(), -1);
    bfs(graph, milk - 1, milk - 1, milk_ways);

    set_output(find_way_count(matilda_ways, leon_ways, milk_ways));

    return 0;
}