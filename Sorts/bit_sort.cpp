#include <vector>
#include <iostream>
#include <fstream>
#include <string>

template <size_t size>
class BitSet {
public:

    BitSet () {
        bits.assign(size, 0);
    }

    void set (size_t i) {
        size_t element_size = sizeof(size_t) * 8;
        size_t number_index = i / element_size;
        size_t position =  i % element_size;
        bits[number_index] |= 1ull << position;
    }

    bool test (size_t i) {
        size_t element_size = sizeof(size_t) * 8;
        size_t number_index = i / element_size;
        size_t position =  i % element_size;
        return (bits[number_index] & (1ull << position)) == (1ull << position);
    }

private:

    std::vector<size_t> bits;

};

void sort_bit (std::vector<size_t> &array) {
    BitSet <10000000> b;
    for (size_t x : array) {
        b.set(x);
    }
    array.clear();
    for (size_t i = 0; i < 1000000; ++i) {
        if (b.test(i)) {
            array.push_back(i);
        }
    }
}

void read_file_to_vector (const std::string &file, std::vector<size_t> &numbers) {
    std::ifstream fin(file);
    while (!fin.eof()) {
        size_t x;
        fin >> x;
        numbers.push_back(x);
    }
    fin.close();
}

void write_vector_to_file (const std::string &file, std::vector<size_t> &numbers) {
    std::ofstream out(file);
    for (size_t x : numbers) {
        out << x << " ";
    }
    out.close();
}

void optimum_sort_bit (const std::string &file1, const std::string &file2) {

    BitSet <10000000> b;
    std::ifstream fin(file1);

    while (!fin.eof()) {
        size_t x;
        fin >> x;
        b.set(x);
    }

    std::ofstream out(file2);

    for (size_t i = 0; i < 1000000; ++i) {
        if (b.test(i)) {
            out << i << " ";
        }
    }
}

int main () {
    std::vector<size_t> array;
    read_file_to_vector("/home/vh/CLionProjects/sort_bits/nums.txt",array);
    sort_bit(array);
    write_vector_to_file("/home/vh/CLionProjects/sort_bits/result1.txt", array);
    optimum_sort_bit("/home/vh/CLionProjects/sort_bits/nums.txt", "/home/vh/CLionProjects/sort_bits/result2.txt");
    return 0;
}
