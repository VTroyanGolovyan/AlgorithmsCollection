#include <iostream>
#include <string>
#include <vector>

enum {
    white = 0,
    grey = 1,
    black = 2
};

bool dfs (
    std::vector < std::vector <size_t> > &graph,
    std::vector <char> &colors,
    std::vector <long long> &matching,
    size_t v
) {
    if (colors[v] != white) {
        return false;
    }
    colors[v] = grey;
    for (size_t i = 0; i < graph[v].size(); ++i) {
        if (matching[graph[v][i]] == -1 || dfs(graph, colors, matching, matching[graph[v][i]])) {
            matching[graph[v][i]] = v;
            return true;
        }
    }
    colors[v] = black;
    return false;
}

size_t kun (
    std::vector < std::vector <size_t> > &graph,
    std::vector <long long> &matching
) {
    matching.assign(graph.size(), -1);
    std::vector <char> colors;
    size_t size = 0;
    for (size_t i = 0; i < graph.size(); ++i) {
        colors.assign(graph.size(), white);
        if (dfs(graph, colors, matching, i)) {
            ++size;
        }
    }
    return size;
}

void addVertixes (
    std::vector <std::string> &str_graph,
    std::vector < std::vector <size_t> > &graph,
    long long x,
    long long y,
    size_t n,
    size_t m
) {
    for (long long i = x - 1; i <= x + 1; ++i) {
        if (i < 0 || i >= n) {
            continue;
        }
        if (i == x) {
            continue;
        }
        if (str_graph[i][y] == '*') {
            graph[x * m + y].push_back(i * m + y);
        }
    }
    for (long long j = y - 1; j <= y + 1; ++j) {
        if (j < 0 || j >= m) {
            continue;
        }
        if (j == y) {
            continue;
        }
        if (str_graph[x][j] == '*') {
            graph[x * m + y].push_back(x * m + j);
        }
    }
}
void getInput (
    std::istream &in,
    std::vector < std::vector <size_t> > &graph,
    size_t &n,
    size_t &m,
    long long &a,
    long long &b,
    size_t &k
) {
    in >> n >> m >> a >> b;
    k = 0;
    std::vector <std::string> str_graph;
    graph.assign(n * m, std::vector <size_t>());
    for (size_t i = 0; i < n; ++i) {
        std::string tmp;
        std::cin >> tmp;
        str_graph.push_back(tmp);
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (str_graph[i][j] == '*') {
                ++k;
                addVertixes(str_graph, graph, i, j, n, m);
            }
        }
    }
}

long long getTime (
    std::vector < std::vector <size_t> > &graph,
    long long a,
    long long b,
    size_t k
) {
    if (a >= 2 * b) {
        return k * b;
    } else {
        std::vector <long long> matching;
        size_t size = kun(graph, matching);
        return size / 2 * a + (k - size) * b;
    }
}

void setOutput (
    std::ostream &out,
    long long time
) {
    out << time;
}

int main() {
    size_t n, m, k;
    long long a, b;
    std::vector < std::vector <size_t> > graph;
    getInput(std::cin, graph, n, m, a, b, k);
    setOutput(std::cout, getTime(graph, a, b, k));
    return 0;
}