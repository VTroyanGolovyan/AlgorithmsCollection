#include <bitset>
#include <iostream>
#include <limits>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <functional>
#include <thread>
#include <queue>
#include <chrono>
#include <mutex>

const int32_t ST_MAX = std::numeric_limits<int32_t>::max(); // max int32_t
const int32_t RESERVE_TEXT_SIZE = 2000005; // reserve text size for optimization buffer
const int32_t RESERVE_PATTERN_SIZE = 5000; // reserve pattern size for optimization buffer

std::mutex protect_queue_mutex;

/* Wrapper class for result processing */
template <typename T>
class ResultProcessWrapper {
public:

    explicit ResultProcessWrapper (
        T& container,
        const std::function<void(T&, size_t)> processor
    ) :
            container(container), processor(processor) {}

    void process(size_t pos) {
        processor(container, pos);
    }

private:
    T& container;
    std::function<void(T&, size_t)> processor;
};

/* Trie class for Aho-Korasik algorithm */
template <typename T>
class Trie {
private:
    /* Trie vertex class with getters and setters */
    class TrieVertex {
    public:

        TrieVertex(int32_t parent, char symbol, size_t alphabet_size);

        const int32_t& getLetterChild(const char& symbol) const;

        void setNextLetterChild(const char& symbol, const int32_t& vertex);

        const int32_t& getSuffix() const;

        void setSuffix(const int32_t &suffix);

        const int32_t& getGoodSuffix() const;

        void setGoodSuffix(const int32_t& suffix);

        const int32_t& getParent() const;

        void setParent(const int32_t& parent);

        const int32_t& getAutomateMove(const char& symbol) const;

        void setAutomateMove(const char& symbol, int32_t vertex);

        const char& getSymbol() const;

        void setTrueFlag();

        const bool& getFlag() const;

        const int32_t& getPatternNumber() const;

        void setPatternNumber(const int32_t& number);

    private:

        std::vector<int32_t> children_;
        std::vector<int32_t> automate_move_;
        int32_t suffix_link_;
        int32_t good_suffix_link_;
        bool flag_;
        char symbol_;
        int32_t parent_;
        int32_t pattern_number_;

    };
public:

    explicit Trie (size_t alphabet_size = 256);

    const int32_t& next(const int32_t& vertex, const char& );

    void addVertex(const int32_t& parent, const char& );

    void addString(const std::string &s);

    const int32_t& getSuffixLink(const int32_t& vertex);

    const int32_t& getGoodSuffixLink(const int32_t& vertex);

    const int32_t& getAutomateMove(const int32_t& vertex, const char& i);

    void check(
        ResultProcessWrapper<T> &out,
        const std::string &s,
        const int32_t& v,
        const int32_t& i,
        std::vector<std::vector<int32_t>> &patterns_positions,
        std::vector<int32_t> &patterns_pos_vector_indexes,
        std::vector<int32_t> &result,
        const int32_t& pattern_len
    );

    void findPositions(
        ResultProcessWrapper<T> &out,
        const std::string& s,
        std::vector<std::vector<int32_t>> &patterns_positions,
        std::vector<int32_t> &patterns_pos_vector_indexes,
        std::vector<int32_t> &result,
        int32_t pattern_len
    );

private:

    std::vector<TrieVertex> Trie_;
    std::vector<std::string> patterns_;
    size_t alphabet_size_;

};

template <typename T>
Trie<T>::Trie(size_t alphabet_size) : alphabet_size_(alphabet_size) {
    Trie_.emplace_back(0, 120, alphabet_size);
}

/* create vertex */
template <typename T>
void Trie<T>::addVertex(const int32_t& parent, const char& i) {
    Trie_.emplace_back(parent, i, alphabet_size_);
    Trie_[parent].setNextLetterChild(i, Trie_.size() - 1);
}

/* next vertex getter */
template <typename T>
const int32_t& Trie<T>::next(const int32_t& vertex, const char& i) {
    return Trie_[vertex].getLetterChild(i);
}

/* add string to Trie */
template <typename T>
void Trie<T>::addString(const std::string &s) {
    int32_t num = 0;
    for (int32_t i = 0; i < s.length(); ++i) {
        const char& ch = s[i];
        char index = static_cast<char>(ch - 'a');
        if (next(num, index) == ST_MAX) {
            addVertex(num, index);
        }
        num = next(num, index);
    }

    patterns_.push_back(s);
    Trie_[num].setTrueFlag();
    Trie_[num].setPatternNumber(patterns_.size() - 1);
}

/* suffix link getter */
template <typename T>
const int32_t& Trie<T>::getSuffixLink(const int32_t& vertex) {
    if (Trie_[vertex].getSuffix() == ST_MAX) {
        if (vertex == 0 || Trie_[vertex].getParent() == 0) {
            Trie_[vertex].setSuffix(0);
        } else {
            Trie_[vertex].setSuffix (
                    getAutomateMove (
                        getSuffixLink(Trie_[vertex].getParent()),
                        Trie_[vertex].getSymbol()
                    )
            );
        }
    }
    return Trie_[vertex].getSuffix();
}

/* compressed suffix link getter */
template <typename T>
const int32_t& Trie<T>::getGoodSuffixLink(const int32_t& vertex) {
    if (Trie_[vertex].getGoodSuffix() == ST_MAX) {
        int32_t u = getSuffixLink(vertex);
        if (u == 0) {
            Trie_[vertex].setGoodSuffix(0);
        } else {
            Trie_[vertex].setGoodSuffix(
                Trie_[u].getFlag() ? u : getGoodSuffixLink(u)
            );
        }
    }
    return Trie_[vertex].getGoodSuffix();
}

/* automate step */
template <typename T>
const int32_t& Trie<T>::getAutomateMove(const int32_t& vertex, const char& i) {
    if (Trie_[vertex].getAutomateMove(i) == ST_MAX) {
        if (Trie_[vertex].getLetterChild(i) != ST_MAX) {
            Trie_[vertex].setAutomateMove (
                i,
                Trie_[vertex].getLetterChild(i)
            );
        } else if (vertex == 0) {
            Trie_[vertex].setAutomateMove(i, 0);
        } else {
            Trie_[vertex].setAutomateMove(
                i,
                getAutomateMove(getSuffixLink(vertex), i)
            );
        }
    }
    return Trie_[vertex].getAutomateMove(i);
}

/* check positions and write to stream results */
template <typename T>
void Trie<T>::check(
    ResultProcessWrapper<T> &out,
    const std::string &s,
    const int32_t& v,
    const int32_t& i,
    std::vector<std::vector<int32_t>> &patterns_positions,
    std::vector<int32_t> &patterns_pos_vector_indexes,
    std::vector<int32_t> &result,
    const int32_t& pattern_len
) {
    int32_t u = v;
    int32_t patterns_count = patterns_.size();
    int32_t patterns_last = patterns_count - 1;
    int32_t s_len = s.length();
    while (u != 0) {
        if (Trie_[u].getFlag()) {
            const std::string& pat = patterns_[Trie_[u].getPatternNumber()];
            int32_t pos = i - pat.length() + 1;
            const std::vector<int32_t>& pat_pos = patterns_positions[patterns_pos_vector_indexes[Trie_[u].getPatternNumber()]];
            for (int32_t it = 0; it < pat_pos.size(); ++it) {
                const int32_t& my_p = pat_pos[it];
                if (pos >= my_p) {
                    int32_t index =  pos - my_p;
                    ++result[index];
                    if (
                            Trie_[u].getPatternNumber() == patterns_last &&
                            result[index] == patterns_count &&
                            index <= s_len - pattern_len
                    ) {
                        out.process(index);
                        result[index] = 0;
                    }
                }
            }
        }
        u = getGoodSuffixLink(u);
    }
    if (patterns_.empty() && i <= s_len - pattern_len) {
        out.process(i);
    }
}

template <typename T>
void Trie<T>::findPositions(
    ResultProcessWrapper<T> &out,
    const std::string &s,
    std::vector<std::vector<int32_t>> &patterns_positions,
    std::vector<int32_t> &patterns_pos_vector_indexes,
    std::vector<int32_t> &result,
    int32_t pattern_len
) {
    int32_t now = 0;
    int32_t s_len = s.length();
    for (int32_t i = 0; i < s_len; ++i) {
        now = getAutomateMove(now, static_cast<char>(s[i] - 'a'));
        check(out, s, now, i, patterns_positions, patterns_pos_vector_indexes, result, pattern_len);
    }
}

template <typename T>
Trie<T>::TrieVertex::TrieVertex(
    int32_t parent,
    char symbol,
    size_t alphabet_size
) :
    parent_(parent),
    symbol_(symbol),
    suffix_link_(ST_MAX),
    good_suffix_link_(ST_MAX),
    pattern_number_(ST_MAX),
    flag_(false) {

    children_.assign(alphabet_size, ST_MAX);
    automate_move_.assign(alphabet_size, ST_MAX);

}

template <typename T>
const int32_t& Trie<T>::TrieVertex::getLetterChild(const char& symbol) const {
    return children_[symbol];
}

template <typename T>
void Trie<T>::TrieVertex::setNextLetterChild(const char& symbol, const int32_t& vertex) {
    children_[symbol] = vertex;
}

template <typename T>
const int32_t& Trie<T>::TrieVertex::getSuffix() const {
    return suffix_link_;
}

template <typename T>
void Trie<T>::TrieVertex::setSuffix(const int32_t &suffix) {
    suffix_link_ = suffix;
}

template <typename T>
const int32_t& Trie<T>::TrieVertex::getGoodSuffix() const {
    return good_suffix_link_;
}

template <typename T>
void Trie<T>::TrieVertex::setGoodSuffix(const int32_t &suffix) {
    good_suffix_link_ = suffix;
}

template <typename T>
const int32_t& Trie<T>::TrieVertex::getParent() const {
    return parent_;
}

template <typename T>
void Trie<T>::TrieVertex::setParent(const int32_t &parent) {
    parent_ = parent;
}

template <typename T>
const int32_t& Trie<T>::TrieVertex::getPatternNumber() const {
    return pattern_number_;
}

template <typename T>
void Trie<T>::TrieVertex::setPatternNumber(const int32_t &num) {
    pattern_number_ = num;
}

template <typename T>
const int32_t& Trie<T>::TrieVertex::getAutomateMove(const char& symbol) const {
    return automate_move_[symbol];
}

template <typename T>
void Trie<T>::TrieVertex::setAutomateMove(const char& symbol, int32_t vertex) {
    automate_move_[symbol] = vertex;
}

template <typename T>
void Trie<T>::TrieVertex::setTrueFlag() {
    flag_ = true;
}

template <typename T>
const bool& Trie<T>::TrieVertex::getFlag() const {
    return flag_;
}

template <typename T>
const char& Trie<T>::TrieVertex::getSymbol() const {
    return symbol_;
}

template <typename T>
void fillTrieWithPattern(
    Trie<T>& Trie,
    std::string &pattern,
    std::vector<std::vector<int32_t>> &positions,
    std::vector<int32_t> &patterns_pos_vector_indexes,
    bool &has_fictive_symbol
) {
    std::string current;
    int32_t pos = 0;

    if (pattern[pattern.length() - 1] != '?') {
        pattern += '?';
        has_fictive_symbol = true;
    }

    std::unordered_map<std::string, int32_t> patterns_positions;

    for (int32_t i = 0; i < pattern.size(); ++i) {
        const char &ch = pattern[i];
        if (ch != '?') {
            current += ch;
        } else if (!current.empty()){
            Trie.addString(current);
            if (patterns_positions.find(current) != patterns_positions.end()) {
                positions[patterns_positions[current]].push_back(pos - current.length());
            } else {
                patterns_positions[current] = positions.size();
                positions.emplace_back(1, pos - current.length());
            }
            patterns_pos_vector_indexes.push_back(patterns_positions[current]);

            current.clear();
        }
        ++pos;
    }

    if (!current.empty()) {
        Trie.addString(current);
        --pos;
        if (patterns_positions.find(current) != patterns_positions.end()) {
            positions[patterns_positions[current]].push_back(pos - current.length());
        } else {
            patterns_positions[current] = positions.size();
            positions.emplace_back(1, pos - current.length());
        }
        patterns_pos_vector_indexes.push_back(patterns_positions[current]);
        current.clear();
    }
}

template <typename T>
void search(
    ResultProcessWrapper<T> out,
    std::string &pattern,
    const std::string &s
) {

    Trie<T> Trie;
    std::vector<std::vector<int32_t>> patterns_positions;
    std::vector<int32_t> patterns_pos_vector_indexes;
    bool has_fictive_symbol = false; // true if we have fictive '?' in the end

    fillTrieWithPattern(Trie, pattern, patterns_positions, patterns_pos_vector_indexes, has_fictive_symbol);

    std::vector<int32_t> result(s.length(), 0);
    Trie.findPositions(
        out,
        s,
        patterns_positions,
        patterns_pos_vector_indexes,
        result,
        pattern.length() - static_cast<int32_t>(has_fictive_symbol)
    );

}

void getInput(
        std::istream &in,
        std::string &pattern,
        std::string &s
) {
    in >> pattern >> s;
}

void threadOut(std::queue<size_t>& queue, std::ostream& out, bool& finished) {
    while (!finished || !queue.empty()) {
        protect_queue_mutex.lock();
        while (!queue.empty()) {
            out << queue.front() << " ";
            queue.pop();
        }
        protect_queue_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {

    std::string pattern;
    std::string s;
    /* reserve space to increase performance (we needn't reallocate space) */
    s.reserve(RESERVE_TEXT_SIZE);
    pattern.reserve(RESERVE_PATTERN_SIZE);
    /* cin&cout optimization */
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    getInput(std::cin, pattern, s);
    std::queue<size_t> res_queue{};

    bool finished = false;
    std::thread th(threadOut, std::ref(res_queue), std::ref(std::cout), std::ref(finished));

    search(
        ResultProcessWrapper<std::queue<size_t>>(
            res_queue,
            [](std::queue<size_t>& out, size_t pos){
                protect_queue_mutex.lock();
                out.push(pos);
                protect_queue_mutex.unlock();
            }
        ),
        pattern,
        s
    );

    finished = true;
    th.join();

    return 0;

}
