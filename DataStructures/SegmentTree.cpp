#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>

class SegmentTree {
public:

    explicit SegmentTree (const std::vector <size_t> &tickets, size_t size) {
        size_t n = 2 * (1 << (size_t)ceil(log2((double)size)));
        tree_.assign(n - 1, std::make_pair(0, 0));
        size_ = n / 2;
        for (size_t i = 0; i < tickets.size(); ++i) {
            tree_[tree_.size() / 2 + i].first = tickets[i];
        }

        for (size_t i = tree_.size() / 2; i >= 1; --i) {
            tree_[i - 1].first = std::max(
                tree_[2 * (i - 1) + 1].first,
                tree_[2 * (i - 1) + 2].first
            );
        }
    }

    size_t size () {
        return size_;
    }
    size_t getMax (size_t node, size_t tl, size_t tr, size_t l, size_t r) {
        if (tl > tr || l > r) {
            return 0;
        }
        if (l == tl && r == tr) {
            return tree_[node].first + tree_[node].second;
        }
        size_t p = tl + (tr - tl) / 2;
        size_t result = std::max (
                getMax(2 * node + 1, tl, p, l, std::min(r, p)),
                getMax(2 * node + 2, p + 1, tr, std::max(l, p + 1), r)
        ) + tree_[node].second;
        return result;
    }

    void update (size_t node, size_t tl, size_t tr, size_t l, size_t r, size_t change) {
        if (tl > tr) {
            return;
        }
        if (l > r) {
            return;
        }
        if (l == tl && tr == r) {
            tree_[node].second += change;
        } else {

            size_t p = tl + (tr - tl) / 2;

            update(2 * node + 1, tl, p, l, std::min(r, p), change);
            update(2 * node + 2, p + 1, tr, std::max(l, p + 1), r, change);

            tree_[node].first = std::max(
                    tree_[node * 2 + 1].first + tree_[2 * node + 1].second,
                    tree_[node * 2 + 2].first + tree_[2 * node + 2].second
            );
        }
    }

private:

    std::vector < std::pair<size_t, size_t> > tree_;
    size_t size_;

};
void getInput (
    std::istream &in,
    std::vector <size_t> &tickets,
    size_t &volume,
    std::vector < std::tuple <size_t, size_t, size_t> > &queries
) {
    size_t n;
    in >> n;
    tickets.assign(n - 1, 0);
    for (size_t i = 0; i < n - 1; ++i) {
        in >> tickets[i];
    }
    in >> volume;
    size_t m;
    in >> m;
    size_t x, y, count;
    for (size_t i = 0; i < m; ++i) {
        in >> x >> y >> count;
        queries.emplace_back(x, y, count);
    }
}

void setOutput (
    std::ostream &out,
    std::vector <size_t> &result
) {
    for (size_t x : result) {
        out << x << " ";
    }
}

void makeQueries (
    std::vector <size_t> &tickets,
    std::vector < std::tuple <size_t, size_t, size_t> > &queries,
    size_t volume,
    std::vector <size_t> &result
) {
    SegmentTree tree(tickets, tickets.size());
    size_t i = 0;
    for (auto& q : queries) {
        if (tree.getMax(0, 0, tree.size() - 1, std::get<0>(q), std::get<1>(q) - 1) + std::get<2>(q) > volume) {
            result.push_back(i);
        } else {
            tree.update(0, 0, tree.size() - 1, std::get<0>(q), std::get<1>(q) - 1 , std::get<2>(q));
        }
        ++i;
    }
    std::cout << std::endl;
}

int main() {
    std::vector <size_t> tickets;
    std::vector < std::tuple <size_t, size_t, size_t> > queries;
    size_t volume;
    getInput(std::cin, tickets, volume, queries);
    std::vector <size_t> result;
    makeQueries(tickets, queries, volume, result);
    setOutput(std::cout, result);
    return 0;
}