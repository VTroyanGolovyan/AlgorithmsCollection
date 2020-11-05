#include <iostream>
#include <string>
#include <vector>
#include <fstream>

template <typename ValType>
class HashTable {
private:

    class TableCell {
    public:

        bool isset;
        std::string key;
        ValType value;

        TableCell() {
            isset = false;
            key = "";
            value = ValType();
        }

    };

    std::vector<TableCell> hash_table;

public:

    explicit HashTable () {
        hash_table.assign(8, TableCell());
        insertions = 0;
    }

    bool contains(
        const std::string &key
    ) {
        size_t real_index = getRealIndex(hash_table, key, hash_table.size());
        return hash_table[real_index].isset;
    }

    ValType& get(
        const std::string &key
    ) {
        size_t real_index = getRealIndex(hash_table, key, hash_table.size());
        return hash_table[real_index].value;
    }

    void remove(
        const std::string& key
    ) {
        size_t real_index = getRealIndex(hash_table, key, hash_table.size());
        hash_table[real_index].isset = false;
    }

    void insert(
        const std::string& key,
        const ValType& value
    ) {
        if (((double)insertions)/hash_table.size() >= 0.7) {
            rehash();
        }
        size_t real_index = getRealIndex(hash_table, key, hash_table.size());
        hash_table[real_index].key = key;
        hash_table[real_index].value = value;
        if (!hash_table[real_index].isset) {
            ++insertions;
        }
        hash_table[real_index].isset = true;
    }

    bool erase(const std::string& key) {
        return false;
    }

    ValType& operator [] (const std::string &key) {
        if (!contains(key)) {
            insert(key, ValType());
        }
        return get(key);
    }

    const ValType& operator [] (const std::string &key) const {
        return get(key);
    }

private:

    size_t insertions;

    size_t getRealIndex (
        const std::vector<TableCell> &table,
        const std::string& key,
        size_t size
    ) {
        size_t i = hash(key, size);
        size_t h2 = hash2(key, size);
        size_t k = 0;
        while (table[i].key != key && table[(i + h2 * k) % size].isset) {
            ++k;
        }
        return (i + h2 * k) % size;
    }

    size_t hash(const std::string &s, size_t size) {
        size_t result = 0;
        int p = 3;
        for (char i : s) {
            result += (size_t)i;
            result *= p;
            result %= size;
        }
        return result;
    }

    size_t hash2(const std::string &s, size_t size) {
        return (s.length() + (size_t)(s.length() % 2 == 0)) % size;
    }

    void rehash() {
        size_t new_size = 2 * hash_table.size();
        std::vector<TableCell> new_table(new_size);
        size_t new_insertions = 0;
        for (auto& element : hash_table) {
            if (element.isset) {
                size_t real_index = getRealIndex(new_table, element.key, new_size);
                new_table[real_index].key = element.key;
                new_table[real_index].value = element.value;
                new_table[real_index].isset = true;
                ++new_insertions;
            }
        }
        hash_table = new_table;
        insertions = new_insertions;
    }

};

void makeTable(std::istream &in, HashTable<std::string> &ht) {
    char line[512];
    while (in.getline(line, 512)) {
        size_t i = 0;
        std::string number;
        std::string data;
        while (line[i] != ',') {
            number += line[i];
            ++i;
        }
        ++i;
        while (line[i] == ' ') {
            ++i;
        }
        while (line[i] != '\0') {
            data += line[i];
            ++i;
        }
        ht[number] = data;
    }
}

int main() {
    HashTable<std::string> ht;
    std::ifstream fs("/home/vh/CLionProjects/DoubleHashF/data.txt");
    makeTable(fs, ht);
    std::cout << ht["788GGG7"] << std::endl;
    std::cout << ht.get("AI9896D") << std::endl;
    std::cout << ht["AC57577"] << std::endl;
    std::cout << ht.contains("AJ89899") << std::endl;
    ht.remove("AJ89899");
    std::cout << ht.contains("AJ89899") << std::endl;
    return 0;
}
