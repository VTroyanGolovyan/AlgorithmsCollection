#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <cstdint>
#include <functional>

enum {
    in_use = 1,
    not_used = 0
};

const std::string ALPHABET("qazwsxedcrfvtgbyhnujmikolp");

class TreeVertex {
public:

    int64_t link_;
    int64_t parent_;

    int64_t left_;
    int64_t right_;

    std::map<char, int64_t> next_;

    TreeVertex() : left_(-1), right_(-1), parent_(-1), link_(0) {}
    TreeVertex (int64_t left, int64_t right, int64_t parent) : left_(left), right_(right), parent_(parent), link_(0) {}
};

class State {
public:
    int64_t node_;
    int64_t position_;
    State (int64_t node, int64_t pos) : node_(node), position_(pos)  { }
};

class SuffixTree {
public:

    SuffixTree(std::string alphabet, const std::string& source, int64_t first_length)
        :
            first_length_(first_length),
            source_(source),
            size_(2),
            current_position_(0),
            source_length_(source.length()),
            state_(0, 0),
            tree_(source.length() << 2),
            letters_(std::move(alphabet))
    {
        TreeVertex root;
        tree_[0] = root;
        build_();
    }

    int64_t getsize_() const {
        return size_;
    }
    /* DFS with callback */
    void dfs(
        std::vector<int64_t>& re_num,
        const std::function<
                void(const std::vector<TreeVertex>&, int64_t&, int64_t&)
        > &callback
    ) {
        std::vector<char> used(getsize_(), not_used);
        int64_t i = 1;
        dfs_(re_num, used, callback, 0, i);
    }

private:

    void dfs_(
            std::vector<int64_t>& re_num,
            std::vector<char>& used,
            const std::function<
                    void(const std::vector<TreeVertex>&, int64_t&, int64_t&)
            > &callback,
            int64_t vertex_number,
            int64_t &i
    ) {
        used[vertex_number] = in_use;
        if (vertex_number > 0) {
            re_num[vertex_number] = i;
            callback(tree_, vertex_number, first_length_);
            ++i;
        }
        for (auto elem : tree_[vertex_number].next_) {
            if (used[elem.second] == not_used) {
                dfs_(re_num, used, callback, elem.second, i);
            }
        }
    }

    void build_() {

        for (int64_t i = 2; i < tree_.size(); ++i) {
            tree_[i].right_ = source_length_ - 1;
        }

        for (char ch : letters_) {
            tree_[1].next_[ch] = 0;
        }

        tree_[1].next_['$'] = 0;
        tree_[1].next_['#'] = 0;

        tree_[0].link_ = 1;

        for (char& i : source_) {
            insertLetter_(i);
            ++current_position_;
        }
    }

    void insertLetter_(const char& symbol) {

        bool continueFlag;

        do {

            continueFlag = false;
            bool skipFlag = false;

            if (state_.position_ > tree_[state_.node_].right_) {

                if (tree_[state_.node_].next_.find(symbol) == tree_[state_.node_].next_.end()) {
                    tree_[state_.node_].next_[symbol] = size_;
                    tree_[size_].left_ = current_position_;
                    tree_[size_].parent_ = state_.node_;
                    ++size_;
                    state_.node_ = tree_[state_.node_].link_;
                    state_.position_ = tree_[state_.node_].right_ + 1;
                    continueFlag = true;
                    skipFlag = true;
                }
                if (!skipFlag) {
                    state_.node_ = tree_[state_.node_].next_[symbol];
                    state_.position_ = tree_[state_.node_].left_;
                }
            }

            if (!skipFlag) {
                if (state_.position_ == -1 || source_[state_.position_] == symbol) {
                    ++state_.position_;
                } else {

                    tree_[size_] = TreeVertex(tree_[state_.node_].left_, state_.position_ - 1, tree_[state_.node_].parent_);
                    tree_[size_].next_[source_[state_.position_]] = state_.node_;

                    tree_[size_].next_[symbol] = size_ + 1;
                    tree_[size_ + 1].left_ = current_position_;
                    tree_[size_ + 1].parent_ = size_;

                    tree_[state_.node_].left_ = state_.position_;
                    tree_[state_.node_].parent_ = size_;
                    tree_[tree_[size_].parent_].next_[source_[tree_[size_].left_]] = size_;
                    size_ += 2;

                    state_.node_ = tree_[tree_[size_ - 2].parent_].link_;
                    state_.position_ = tree_[size_ - 2].left_;

                    while (tree_[size_ - 2].right_ >= state_.position_) {
                        state_.node_ = tree_[state_.node_].next_[source_[state_.position_]];
                        state_.position_ += 1 + tree_[state_.node_].right_ - tree_[state_.node_].left_;
                    }

                    if (state_.position_ == tree_[size_ - 2].right_ + 1) {
                        tree_[size_ - 2].link_ = state_.node_;
                    } else {
                        tree_[size_ - 2].link_ = size_;
                    }

                    state_.position_ = tree_[state_.node_].right_ - (state_.position_ - tree_[size_ - 2].right_) + 2;
                    continueFlag = true;

                }
            }
        } while (continueFlag);

    }

    std::vector<TreeVertex> tree_;

    int64_t current_position_;
    int64_t source_length_;
    int64_t first_length_;
    int64_t size_;

    std::string source_;

    State state_;
    std::string letters_;

};

void getResultToStream(
    std::ostream& out,
    std::string& firstString,
    std::string& secondString
) {

    SuffixTree suffix_tree(
        ALPHABET,
        firstString + secondString,
        firstString.length()
    );

    std::vector<int64_t> re_num(suffix_tree.getsize_(), 0);

    re_num[0] = 0;

    out << suffix_tree.getsize_() - 1 << "\n";

    suffix_tree.dfs(
        re_num,
        [&] (const std::vector<TreeVertex>& tree, int64_t& vertex_number, int64_t& first_len) {
            out << re_num[tree[vertex_number].parent_] << " ";
            if (tree[vertex_number].left_ < first_len) {
                out << 0 << " ";
                out << tree[vertex_number].left_ << " ";
                if (tree[vertex_number].right_ < first_len ) {
                    out << tree[vertex_number].right_ + 1;
                } else {
                    out << first_len;
                }
                out << "\n";
            }
            if (tree[vertex_number].left_ >= first_len) {
                out << 1 << " ";
                out << tree[vertex_number].left_ - first_len << " ";
                out <<  tree[vertex_number].right_ - first_len + 1;
                out << "\n";
            }
        }
    );

}

void getInput(
    std::istream& in,
    std::string& first_string,
    std::string& second_string
) {
    in >> first_string >> second_string;
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string first_string, second_string;

    getInput(std::cin, first_string, second_string);

    getResultToStream(std::cout, first_string, second_string);

    return 0;
}
