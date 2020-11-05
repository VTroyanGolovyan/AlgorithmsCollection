
CLion
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

class ComponentVertix {
public:
    /* Default constructor */
    ComponentVertix() {
        v_ = -1;
        type_ = false;
    }
    /* Constructor */
    ComponentVertix (size_t v, bool type) {
        v_ = v;
        type_ = type;
    }
    /* = operator */
    ComponentVertix& operator =(const ComponentVertix &right) {
        if (this == &right) {
            return *this;
        }
        v_ = right.v_;
        type_ = right.type_;
        return *this;
    }
    /* Get Vertex Number */
    size_t GetNumber () const {
        return v_;
    }
    /* Check Point is Extreme */
    bool IsExtreme () const {
        return type_;
    }
private:
    size_t v_;
    bool type_;
};
/* Component class */
class Component {
public:
    /* Constructor */
    Component () = default;
    /* Constructor */
    Component (size_t u, size_t v) {
        vertix_.emplace_back(u, true);
        vertix_.emplace_back(v, true);
    }
    /* Get component size */
    size_t Size() {
        return vertix_.size();
    }
    /* Get component vertix */
    const ComponentVertix& GetVertix(size_t i) {
        return vertix_[i];
    }
    void AddVertix(size_t v, bool type) {
        vertix_.emplace_back(v, type);
    }
    /* Checks if vertix lie in component */
    bool HasVertix(size_t v) {
        for (ComponentVertix &u : vertix_) {
            if (v == u.GetNumber()) {
                return true;
            }
        }
        return false;
    }
private:
    std::vector<ComponentVertix> vertix_;
};
/* Face class */
class Face {
public:
    /* Default constructor */
    Face () {
        external_ = false;
    }
    /* Face constructor */
    Face (std::vector <size_t> &vertix, bool external) {
        vertix_ = vertix;
        external_ = external;
    }
    /* Split Face with chain*/
    std::pair <Face, Face> Split (std::vector <size_t> &chain) {

        size_t start = chain[0];
        size_t end =  chain[chain.size() - 1];
        size_t start_it = 0;

        while (vertix_[start_it] != start) {
            ++start_it;
        }

        size_t end_it = 0;
        while (vertix_[end_it] != end) {
            ++end_it;
        }

        std::vector<size_t > new_face(0);

        for (size_t i = start_it; i != end_it; i = (i + 1) % vertix_.size()) {
            new_face.push_back(vertix_[i]);
        }
        new_face.push_back(vertix_[end_it]);

        for (ssize_t i = 1; i < chain.size() - 1; ++i) {
            new_face.push_back(chain[chain.size() - i - 1]);
        }

        Face first = Face(new_face, external_);

        new_face.clear();
        for (ssize_t i = start_it; i != end_it; i = i < 0 ? vertix_.size() - 1 : i - 1) {
            new_face.push_back(vertix_[i]);
        }

        new_face.push_back(vertix_[end_it]);
        for (ssize_t i = 1; i < chain.size() - 1; ++i) {
            new_face.push_back(chain[chain.size() - i - 1]);
        }

        Face second = Face(new_face, false);

        return std::pair <Face, Face> (first, second);

    }
    /* Check if Face contains components */
    bool ContainsComponent (Component &component) {
        for (size_t i = 0; i < component.Size(); ++i) {
            if (component.GetVertix(i).IsExtreme()) {
                bool res = false;
                for (size_t v : vertix_) {
                    if (v == component.GetVertix(i).GetNumber()) {
                        res = true;
                    }
                }
                if (!res) {
                    return false;
                }
            }
        }
        return true;
    }
private:
    std::vector <size_t> vertix_; //Face vertex
    bool external_;
};

class Graph {
public:
    /* Default constructor */
    Graph () = default;
    /* Constructor */
    explicit Graph (size_t n) {
        matrix_.assign(n, std::vector<bool> (n, false));
    }
    /* Constructor */
    Graph (Graph &g, Component &comp) {
        matrix_.assign(g.Size(), std::vector<bool> (g.Size(), false));
        for (size_t i = 0; i < comp.Size(); ++i) {
            for (size_t j = 0; j < g.Size(); ++j) {
                if (g.HasEdge(comp.GetVertix(i).GetNumber(), j) && comp.HasVertix(j)) {
                    matrix_[comp.GetVertix(i).GetNumber()][j] = true;
                }
            }
        }

        for (size_t i = 0; i < matrix_.size(); ++i) {
            bool test = false;
            for (size_t j = 0; j < matrix_[i].size(); ++j) {
                test |= matrix_[i][j];
            }
            if (!test) {
                matrix_.erase(matrix_.begin() + i);
                --i;
            }
        }

        for (size_t j = 0; j < matrix_[comp.Size() - 1].size(); ++j) {
            bool test = false;
            for (size_t i = 0; i < comp.Size(); ++i) {
                test |= matrix_[i][j];
            }
            if (!test) {
                for (size_t i = 0; i < comp.Size(); ++i) {
                    matrix_[i].erase(matrix_[i].begin() + j);
                }
                j--;
            }
        }

    }
    /* Reassign graph with new size */
    void Reassign (size_t n) {
        matrix_.assign(n, std::vector<bool> (n, false));
    }
    /* Get Size */
    size_t Size () {
        return matrix_.size();
    }
    /* Add Edge to Graph */
    void AddEdge (size_t u, size_t v) {
        matrix_[u][v] = true;
        matrix_[v][u] = true;
    }
    /* Has Edge */
    bool HasEdge(size_t i, size_t j) const {
        return matrix_[i][j];
    }
    /* Check and returns true if Graph planary via Gamma Algorithm */
    bool IsPlanar() {

        if (IsForest()) {
            return true;
        }

        RemoveCutpoints();
        RemoveBridges();

        std::vector <Component> components;
        GetComponents(components);

        for (Component &comp: components) {
            if (comp.Size() > 1) {
                Graph g(*this, comp);
                if (!g.GammaAlgorithm()) {
                    return false;
                }
            }
        }
        return true;

    }
    /* Check and returns true if Graph planary via Gamma Algorithm */
    bool GammaAlgorithm () {

        if (matrix_.empty())
            return true;
        if (IsForest())
            return true;

        Graph laid_graph (matrix_.size());
        std::vector <bool> laid_vertix (matrix_.size(), false);
        /* #1 Find cycle */
        std::vector <size_t> cycle;
        GetRandomCycle(cycle);
        cycle.push_back(cycle[0]);
        PutChain(laid_graph, laid_vertix, cycle);
        cycle.resize(cycle.size() - 1);
        /* Add First Faces */
        std::vector <Face> faces;
        faces.emplace_back(cycle, true);
        faces.emplace_back(cycle, false);

        std::vector<Component> components;

        do {
            components.clear();
            GetGammaComponents(laid_graph, laid_vertix, components);
            std::vector < std::pair <size_t, size_t> > contains(components.size(), std::pair <size_t, size_t> (0, 0));
            GetContainsCount(faces, components, contains );  //Get faces count contains every components
            if (!components.empty()) {

                if (contains[0].second == 0) {
                    return false;
                }

                std::vector <size_t> chain;
                GetChain(laid_graph, laid_vertix, components[contains[0].first], chain); // Extract chain from component

                size_t i = 0;
                while (!faces[i].ContainsComponent(components[contains[0].first])) { //Find face that contain component
                    i++;
                }

                PutChain(laid_graph, laid_vertix, chain);  // Make chai
                std::pair <Face, Face> tmp = faces[i].Split(chain);
                faces[i] =tmp.first;
                faces.push_back(tmp.second);
            }
        } while (!components.empty());

        return true;
    }
    /* Check if graph is Forest */
    bool IsForest () {
        std::vector<char> colors(matrix_.size(), 0);
        for (size_t i = 0; i < matrix_.size(); ++i) {
            if (colors[i] == 0) {
                bool res = true;
                DFS(
                        colors,
                        i,
                        -1,
                        [&] (size_t v) { },
                        [&] (size_t v) { },
                        [&] (size_t v) -> bool {
                            if (colors[v] == 1) {
                                res = false;
                            }
                            return colors[v] == 0;
                        }
                );
                if (!res) {
                    return res;
                }
            }
        }
        return true;
    }
private:
    /* DFS */
    void DFS (
            std::vector <char> &colors,
            size_t v,
            size_t p,
            const std::function < void(size_t) > &in_callback,
            const std::function < void(size_t) > &out_callback,
            const std::function < bool(size_t) > &call_dfs_checker
    ) {
        colors[v] = 1;
        in_callback(v);
        for (size_t i = 0; i < matrix_[v].size(); ++i) {
            if ( i != p && matrix_[v][i] && call_dfs_checker(i) ) {
                DFS(colors, i, v, in_callback, out_callback, call_dfs_checker);
            }
        }
        out_callback(v);
        colors[v] = 2;
    }
    /* Overloaded DFS */
    void DFS (
            std::vector <char> &colors,
            size_t v,
            size_t p,
            const std::function < void(size_t, size_t) > &in_callback,
            const std::function < void(size_t, size_t) > &out_callback,
            const std::function < void(size_t, size_t) > &fail_dfs_callback,
            const std::function < bool(size_t, size_t) > &call_dfs_checker,
            const std::function < void(size_t, size_t) > &after_dfs_callback
    ) {
        colors[v] = 1;
        in_callback(v, p);
        for (size_t i = 0; i < matrix_[v].size(); ++i) {
            if ( i != p && matrix_[v][i]) {
                if (call_dfs_checker(i, v)) {
                    DFS(colors, i, v, in_callback, out_callback, fail_dfs_callback, call_dfs_checker, after_dfs_callback);
                    after_dfs_callback(i, v);
                } else {
                    fail_dfs_callback(i, v);
                }
            }
        }
        out_callback(v, p);
        colors[v] = 2;
    }
    /* Overloaded DFS2 */
    void DFS (
            std::vector <char> &colors,
            size_t v,
            size_t p,
            const std::function < void(size_t, size_t, size_t* &) > &in_callback,
            const std::function < void(size_t, size_t, size_t* &) > &out_callback,
            const std::function < void(size_t, size_t, size_t* &) > &fail_dfs_callback,
            const std::function < bool(size_t, size_t, size_t* &) > &call_dfs_checker,
            const std::function < void(size_t, size_t, size_t* &) > &after_dfs_callback
    ) {
        colors[v] = 1;
        size_t* buffer = nullptr;
        in_callback(v, p, buffer);
        for (size_t i = 0; i < matrix_[v].size(); ++i) {
            if (i != p && matrix_[v][i]) {
                if (call_dfs_checker(i, v, buffer)) {
                    DFS(colors, i, v, in_callback, out_callback, fail_dfs_callback, call_dfs_checker, after_dfs_callback);
                    after_dfs_callback(i, v, buffer);
                } else {
                    fail_dfs_callback(i, v, buffer);
                }
            }
        }
        out_callback(v, p, buffer);
        colors[v] = 2;
    }
    /* Get Random Cycle */
    void GetRandomCycle (std::vector <size_t> &cycle) {
        std::vector <char> colors (matrix_.size(), 0);
        bool cycle_found;
        size_t cycle_start = -1;
        DFS(
                colors,
                0,
                -1,
                [&] (size_t v) {

                },
                [&] (size_t v) {
                    if (v == cycle_start) {
                        cycle.push_back(v);
                        cycle_start = -1;
                    }
                    if (cycle_start != -1) {
                        cycle.push_back(v);
                    }
                },
                [&] (size_t child) -> bool {
                    if (colors[child] == 1) {
                        cycle_start = child;
                    }
                    return cycle.empty() && cycle_start == -1 && colors[child] == 0;
                }
        );
    }
    /* Get not laid components */
    void GetGammaComponents (
            const Graph &laid_graph,
            const std::vector <bool> &laid_vertix,
            std::vector <Component>& components
    ) {
        /* Get one edge segments */
        for (size_t i = 0; i < matrix_.size(); ++i) {
            for (size_t j = i + 1; j < matrix_.size(); ++j) {
                if (i != j && matrix_[i][j] && !laid_graph.HasEdge(i, j) && laid_vertix[i] && laid_vertix[j]) {
                    components.emplace_back(i, j);
                }
            }
        }
        /* Get Other Components */
        std::vector<char> colors(matrix_.size(), 0);
        for (size_t i = 0; i < matrix_.size(); ++i) {
            if (!laid_vertix[i] && colors[i] == 0) {
                Component comp;
                DFS(
                        colors,
                        i,
                        -1,
                        [&] (size_t v) {
                            comp.AddVertix(v, laid_vertix[v]);
                        },
                        [&] (size_t v) {},
                        [&] (size_t v) -> bool {
                            if (colors[v] == 0 && laid_vertix[v]) {
                                comp.AddVertix(v, true);
                                return false;
                            }
                            return colors[v] == 0;
                        }
                );
                if (comp.Size() != 1) {
                    components.push_back(comp);
                }
            }
        }
    }
    /* Get Contains Count */
    static void GetContainsCount (
            std::vector <Face> &faces,
            std::vector <Component> &components,
            std::vector < std::pair <size_t, size_t> > &contains
    ) {
        for (size_t i = 0; i < components.size(); ++i) {
            contains[i].first = i;
            for (auto & face : faces) {
                if (face.ContainsComponent(components[i])) {
                    contains[i].second++;
                }
            }
        }
        std::sort(
                contains.begin(),
                contains.end(),
                [] (std::pair <size_t, size_t> a, std::pair <size_t, size_t> b) -> bool {
                    return a.second < b.second;
                }
        );
    }
    /*Get chain method */
    void GetChain (
            Graph &laid_graph,
            std::vector <bool>  &laid_vertex,
            Component &component,
            std::vector <size_t> &chain
    ) {
        ssize_t start = -1;
        for (size_t i = 0; i < component.Size() && start == -1; ++i) {
            if (component.GetVertix(i).IsExtreme()) {
                start = component.GetVertix(i).GetNumber();
            }
        }
        ssize_t end = -1;
        std::vector <char> colors(matrix_.size(), 0);
        colors[start] = 1;
        for (size_t i = 0; i < matrix_[start].size(); ++i) {
            if (colors[i] == 0 && matrix_[start][i] && !laid_graph.HasEdge(start, i) && component.HasVertix(i)) {
                DFS(
                        colors,
                        i,
                        component.GetVertix(start).GetNumber(),
                        [&] (size_t v) {
                            if (laid_vertex[v]) {
                                chain.push_back(v);
                                end = v;
                            }
                        },
                        [&] (size_t v) {
                            if (end != -1) {
                                chain.push_back(v);
                            }
                        },
                        [&] (size_t v) -> bool {
                            if (!component.HasVertix(v)) {
                                return false;
                            }
                            if (v == start) {
                                return false;
                            }
                            if (end != -1) {
                                return false;
                            }
                            return colors[v] == 0;
                        }
                );
                break;
            }
        }
        chain.push_back(start);
        std::reverse(chain.begin(), chain.end());
        colors[start] = 2;
    }
    /*Put chain method */
    static void PutChain (
            Graph &laid_graph,
            std::vector <bool>  &laid_vertex,
            std::vector <size_t> &chain,
            bool f = true
    ) {
        for (size_t i = 0; i < chain.size() - 1; ++i) {
            laid_graph.AddEdge(chain[i], chain[i + 1]);
            laid_vertex[chain[i]] = true;
        }
        laid_vertex[chain[chain.size() - 1]] = true;
    }
    /* Get graph components */
    void GetComponents (std::vector <Component>& components) {
        std::vector<char> colors(matrix_.size(), 0);
        for (size_t i = 0; i < matrix_.size(); ++i) {
            if (colors[i] == 0) {
                Component comp;
                DFS(
                        colors,
                        i,
                        -1,
                        [&] (size_t v) {
                            comp.AddVertix(v,false);
                        },
                        [&] (size_t v) {},
                        [&] (size_t v) -> bool {
                            return colors[v] == 0;
                        }
                );
                components.push_back(comp);
            }
        }
    };
    /* Delete bridges */
    void RemoveBridges () {
        size_t timer = 0;
        std::vector <char> colors(matrix_.size(), 0);
        std::vector <size_t> enter(matrix_.size(), std::numeric_limits<size_t>::max());
        std::vector <size_t> ret(matrix_.size(), std::numeric_limits<size_t>::max());
        std::vector < std::pair <size_t, size_t> > bridges;
        for (size_t i = 0; i < matrix_.size(); ++i) {
            if (colors[i] == 0) {
                DFS(
                        colors,
                        i,
                        -1,
                        [&] (size_t v, size_t p) {
                            enter[v] = timer;
                            ret[v] = timer;
                            timer++;
                        },
                        [&] (size_t v, size_t p) { },
                        [&] (size_t v, size_t p) {
                            ret[p] = std::min(ret[p], enter[v]);
                        },
                        [&] (size_t v, size_t p) -> bool {
                            return colors[v] == 0;
                        },
                        [&] (size_t v, size_t p) {
                            ret[p] = std::min(ret[p], ret[v]);
                            if (ret[v] > enter[p]) {
                                bridges.emplace_back(v, p);
                            }
                        }
                );
            }
        }
        for (auto & bridge : bridges) {
            matrix_[bridge.first][ bridge.second] = false;
            matrix_[bridge.second][ bridge.first] = false;
        }
    }
    /* Remove bridge */
    void RemoveCutpoints () {
        size_t timer = 0;
        std::vector <char> colors(matrix_.size(), 0);
        std::vector <size_t> enter(matrix_.size(), std::numeric_limits<size_t>::max());
        std::vector <size_t> ret(matrix_.size(), std::numeric_limits<size_t>::max());
        std::vector < size_t > points;
        for (size_t i = 0; i < matrix_.size(); ++i) {
            if (colors[i] == 0) {
                DFS(
                        colors,
                        i,
                        -1,
                        [&] (size_t v, size_t p, size_t* &buffer) {
                            enter[v] = timer;
                            ret[v] = timer;
                            timer++;
                            buffer = new size_t[2];
                            buffer[0] = 0;
                            buffer[1] = p;
                        },
                        [&] (size_t v, size_t p, size_t* &buffer) {
                            if (p == -1 && buffer[0] > 1) {
                                points.push_back(v);
                            }
                            delete []buffer;
                        },
                        [&] (size_t v, size_t p, size_t* &buffer) {
                            ret[p] = std::min(ret[p], enter[v]);
                        },
                        [&] (size_t v, size_t p, size_t* &buffer) -> bool {
                            return colors[v] == 0;
                        },
                        [&] (size_t v, size_t p, size_t* &buffer) {
                            ret[p] = std::min(ret[p], ret[v]);
                            if (ret[v] >= enter[p] && buffer[1] != -1) {
                                points.push_back(p);
                            }
                            buffer[0]++;
                        }
                );
            }
        }

        for (size_t point : points) {
            TransformPoint(point);
        }
    }
    /* Transform points */
    void TransformPoint (size_t v) {
        std::vector < std::vector <size_t> > children_groups;
        std::vector <char> colors(matrix_.size(), 0);
        for (size_t i = 0; i < matrix_[v].size(); ++i) {
            if (matrix_[v][i] && i != v && colors[i] == 0) {
                size_t start_point = v;
                std::vector <size_t> group;
                DFS(
                        colors,
                        i,
                        v,
                        [&] (size_t v, size_t p) {
                            if (p == start_point) {
                                group.push_back(v);
                            }
                        },
                        [&] (size_t v, size_t p) { },
                        [&] (size_t v, size_t p) { },
                        [&] (size_t v, size_t p) -> bool {
                            if (v == start_point) {
                                group.push_back(p);
                                return false;
                            }
                            return colors[v] == 0;
                        },
                        [&] (size_t v, size_t p) { }
                );
                children_groups.push_back(group);
            }
        }
        for (size_t i = 0; i < matrix_.size(); ++i) {
            matrix_[v][i] = false;
        }

        for (size_t i = 0; i < children_groups[0].size(); ++i) {
            matrix_[v][children_groups[0][i]] = true;
        }
        for (size_t i = 1; i < children_groups.size(); ++i) {
            matrix_.emplace_back(matrix_.size(), false);
            for (size_t j = 0; j < matrix_.size(); ++j) {
                matrix_[j].push_back(false);
            }
            for (size_t j = 0; j < children_groups[i].size(); ++j) {
                matrix_[matrix_.size() - 1][children_groups[i][j]] = true;
                matrix_[children_groups[i][j]][matrix_.size() - 1] = true;
            }
        }
    }
    /* Graph matrix */
    std::vector < std::vector<bool> > matrix_;
};

/* Get input data */
void GetInput (size_t &n, size_t &m, Graph &g) {
    std::cin >> n >> m;
    g.Reassign(n);
    size_t u, v;
    for (size_t i = 0; i < m; ++i) {
        std::cin >> u >> v;
        if (u != v) {
            g.AddEdge(u, v);
        }
    }
}
/* Set output */
void SetOutput (const bool &res) {
    std::cout << (res ? "YES" : "NO");
}
/* Main function */
int main() {
    size_t n, m;

    Graph g;
    GetInput(n, m, g);
    SetOutput(g.IsPlanar());

    return 0;
}