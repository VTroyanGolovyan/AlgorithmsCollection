#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

class SparseTable {
public:

    explicit SparseTable (const std::vector<long long> &v) {
        v_ = v;
        for (size_t i = 0; i < v_.size() + 2; ++i) {
            log_.push_back(ceil(log2(i)));
        }
        table_.assign(
            ceil(log2(v_.size())),
            std::vector< std::pair <long long, long long> > (v_.size(), std::make_pair(0, 0))
        );

        for (size_t i = 0; i < v_.size(); ++i) {
            table_[0][i] = std::make_pair(v_[i], v_[i]);
        }

        for (size_t i = 1; i < table_.size(); ++i) {
            for (size_t j = 0; j < table_[i].size() - pow2(i) + 1; ++j) {
                table_[i][j] = min(table_[i - 1][j], table_[i - 1][j + pow2(i - 1)]);
            }
        }
    }

    ~SparseTable() = default;

    long long query (size_t l, size_t r) {
        size_t k = log_[r - l + 1] - 1;
        return min(table_[k][l], table_[k][r - pow2(k) + 1]).second;
    }

private:

    static std::pair <long long, long long> min (
        const std::pair <long long, long long> &a,
        const std::pair <long long, long long> &b
    ) {
        std::vector <long long> v = { a.first, a.second, b.first, b.second };
        std::sort(v.begin(), v.end());
        long long first = v[0];
        for (long long x : v) {
            if (x != first) {
                return std::make_pair(first, x);
            }
        }
        return std::make_pair(first, first);

    }

    static size_t pow2 (size_t k) {
        return 1 << k;
    }

    typedef std::vector < std::vector< std::pair <long long, long long> > > Table;
    std::vector <long long> v_;
    Table table_;
    std::vector <size_t> log_;

};

void getInput (
    std::istream &in,
    std::vector<long long> &v,
    std::vector< std::pair <size_t, size_t> > &queries
) {
    size_t n, m;
    in >> n >> m;
    long long x;
    for (size_t i = 0; i < n; ++i) {
        in >> x;
        v.push_back(x);
    }
    size_t l, r;
    for (size_t i = 0; i < m; ++i) {
        in >> l >> r;
        queries.emplace_back(l, r);
    }
}

void makeQueries (
    const std::vector<long long> &v,
    const std::vector< std::pair <size_t, size_t> > &queries,
    std::vector<long long> &results
) {
   SparseTable st(v);
   for (auto q : queries) {
       results.push_back(st.query(q.first - 1, q.second - 1));
   }
}

void setOutput (
    std::ostream &out,
    std::vector <long long> &result
) {
    for (size_t res : result) {
        out << res << std::endl;
    }
}

int main() {
    std::vector<long long> v;
    std::vector< std::pair <size_t, size_t> > queries;
    getInput(std::cin, v, queries);
    std::vector <long long> results;
    makeQueries(v, queries, results);
    setOutput(std::cout, results);
    return 0;
}