#include <iostream>
#include <string>
#include <vector>
#include <numeric>

std::vector<int64_t> calculateLCP(
    const std::string &s,
    const std::vector<int64_t> &inverse_suffix_array,
    const std::vector<int64_t> &suffix_array
) {

    std::vector<int64_t> lcp(s.size(), 0);

    size_t n = s.size();
    int64_t current_lcp = 0;

    bool skip_iteration;
    for (int64_t i = 0; i < n; ++i) {
        skip_iteration = false;
        if (inverse_suffix_array[i] == n - 1) {
            skip_iteration = true;
        }
        if (!skip_iteration) {
            int64_t next = suffix_array[inverse_suffix_array[i] + 1];
            while (
                current_lcp + (i > next ? i : next) < n &&
                s[i + current_lcp] == s[next + current_lcp]
            ) {
                current_lcp++;
            }
            lcp[inverse_suffix_array[i]] = current_lcp;
            current_lcp = (current_lcp - 1 > 0l ? current_lcp - 1 : 0);
        }
    }

    return lcp;

}


std::vector<int64_t> buildSuffixArray(
    std::string s
) {

    s += "#";

    int64_t n = s.size();
    int64_t equivalence_classes_count = 0;
    int64_t sort_counter = 0;

    std::vector< std::vector <int64_t> > class_helper(256, std::vector <int64_t>());

    std::accumulate(
        s.begin(),
        s.end(),
        0,
        [&] (int64_t index, char el) -> int64_t {
            class_helper[el].emplace_back(index);
            return ++index;
        }
    );

    std::vector<int64_t> suffix_array(n);

    /* zero step */
    std::vector<int64_t> classes = std::accumulate(
        class_helper.begin(),
        class_helper.end(),
        std::vector<int64_t>(n),
        [&] (std::vector<int64_t>& classes, auto &element) -> std::vector<int64_t>& {
            for (int64_t u : element) {
                classes[u] = equivalence_classes_count;
                suffix_array[sort_counter++] = u;
            }
            if (element.size() != 0) {
                ++equivalence_classes_count;
            }
            return classes;
        }
    );

    // get unique suffixes
    for (size_t l = 1; equivalence_classes_count < n; ++l) {

        std::vector< std::vector<int64_t> > a(equivalence_classes_count);  // counting sort array

        std::vector<int64_t> new_classes(n, 0);
        int64_t new_classes_count = 0;

        sort_counter = 0;
        int64_t d = 1ull << (l - 1);

        int64_t k;
        for (size_t i = 0; i < n; ++i) {
            k = (suffix_array[i] - d + n) % n;
            a[classes[k]].emplace_back(k);
        }

        for (size_t i = 0; i < equivalence_classes_count; ++i) {
            for (size_t j = 0; j < a[i].size(); ++j) {
                // new class
                if (
                    j == 0 ||
                    classes[(a[i][j - 1] + d) % n] != classes[(a[i][j] + d) % n]
                ) {
                    ++new_classes_count;
                }
                new_classes[a[i][j]] = new_classes_count - 1;
                suffix_array[sort_counter++] = a[i][j];
            }
        }

        equivalence_classes_count = new_classes_count;
        classes = new_classes;

    }

    return std::vector<int64_t>(
        suffix_array.begin() + 1,
        suffix_array.end()
    );

}

std::vector<int64_t> inverseSuffixArray(
        std::vector<int64_t>& suffix_array
) {
    std::vector<int64_t> inverse_suffix_array(suffix_array.size() + 1);
    std::accumulate(
        suffix_array.begin(),
        suffix_array.end(),
        static_cast<uint64_t>(0),
        [&] (int64_t index, int64_t element) -> uint64_t {
            inverse_suffix_array[element] = index;
            return ++index;
        }
    );
    return inverse_suffix_array;
}

std::string getKStatString(
    const std::string& first_string,
    const std::string& second_string,
    const int64_t& k
) {

    std::string s = first_string + '$' + second_string;

    std::vector<int64_t> suffix_array = buildSuffixArray(s);
    std::vector<int64_t> inverse_suffix_array = inverseSuffixArray(suffix_array);

    std::vector<int64_t>  lcp = calculateLCP(
        s,
        inverse_suffix_array,
        suffix_array
    );

    std::string result;
    int64_t counter = 0;
    int64_t prev_lcp = 0;
    int64_t prev_counter = 0;
    for (int64_t i = 0; i < suffix_array.size(); ++i) {
        if (lcp[i] > 0) {
            int64_t first_suff_size = suffix_array.size() - suffix_array[i];
            int64_t second_suff_size = suffix_array.size() - suffix_array[i + 1];
            if (
                    (first_suff_size > second_string.size() + 1 &&
                     second_suff_size <= second_string.size()) ||
                    (second_suff_size > second_string.size() + 1 &&
                     first_suff_size <= second_string.size())
                    ) {
                counter += lcp[i];
                counter -= std::min(prev_lcp, lcp[i]);
                if (counter >= k) {
                    result = s.substr(suffix_array[i], k - prev_counter + prev_lcp);
                    break;
                }
                prev_lcp = lcp[i];
                prev_counter = counter;
            } else {
                prev_lcp = std::min(lcp[i], prev_lcp);
            }
        } else {
            prev_lcp = 0;
        }
    }

    return result;
}

void getInput(
    std::istream& in,
    std::string& first_string,
    std::string& second_string,
    int64_t& k
) {
    in >> first_string >> second_string >> k;
}

void setOutput(
    std::ostream& out,
    std::string& result
) {
    if (result.empty()) {
        out << -1;
    } else {
        out << result;
    }

}

int main() {

    /* in&out optimization */
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string first_string;
    std::string second_string;
    int64_t k;
    getInput(std::cin, first_string, second_string, k);

    std::string result = getKStatString(first_string, second_string, k);
    setOutput(std::cout, result);

    return 0;
}
