#include <iostream>
#include <vector>
#include <string>

std::vector<size_t> findPositions(
    const std::string &pattern,
    const std::string &text
) {

    std::string s = pattern + "#" + text;
    size_t n = s.length();
    size_t p = pattern.length();
    std::vector<size_t> z (p, 0);
    std::vector<size_t> positions;
    size_t left = 0;
    size_t right = 0;

    for (size_t i = 1; i < n; ++i) {
        size_t z_val = 0;
        if (i <= right) {
            z_val = std::min(right - i + 1, z[i - left]);
        }
        while (
            i + z_val < n &&
            s[z_val] == s[i + z_val]
        ) {
            ++z_val;
        }
        if (i + z_val - 1 > right) {
            left = i;
            right = i + z_val - 1;
        }
        if (i < pattern.length()) {
            z[i] = z_val;           //save only prefix
        }
        if (i > pattern.length() && z_val == pattern.length()) {
            positions.push_back(i - pattern.length() - 1);
        }
    }

    return positions;

}

void getInput(
    std::istream &in,
    std::string &find_template,
    std::string &text
) {
    in >> find_template;
    in >> text;
}

void setOutput(
    std::ostream &out,
    std::vector<size_t> &positions
) {
    for (auto& pos : positions) {
        out << pos << " ";
    }
}

int main() {

    /* cin&cout optimization */
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string pattern, text;
    getInput(std::cin, pattern, text);

    std::vector <size_t> result = findPositions(pattern, text);

    setOutput(std::cout, result);

    return 0;

}