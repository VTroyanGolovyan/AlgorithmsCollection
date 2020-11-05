#include <bitset>
#include <iostream>
#include <vector>

void getInput (
    std::vector < std::bitset < 1000 > > &graph,
) {
    int n;
    std::cin >> n;
    std::string data;
    for (int i = 0; i < n; ++i) {
        std::cin >> data;
        graph.emplace_back(data);
    }
}

void floydAlgorithm (
    std::vector < std::bitset < 1000 > > &graph
) {
    int n = graph.size();
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (graph[i].test(k)) {
                graph[i] |= graph[n - 1 - k];
            }
        }
    }
}

void setOutput (
    std::vector < std::bitset < 1000 > > &graph
) {
    int n = graph.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << graph[i][n - j - 1];
        }
        std::cout << std::endl;
    }
}

int main() {
    std::vector < std::bitset <1000> > graph;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    getInput(graph, n);
    floydAlgorithm(graph);
    setOutput(graph);
    return 0;
}