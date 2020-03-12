#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;

class Element {
public:
    string key;
    int value;
    bool deleted;
    explicit Element(const string key, int value) {
        this->key = key;
        this->value = value;
        this->deleted = false;
    }
    ~Element() {

    }
};

class HashTable{
private:
    Element** v;
    int size;
    int n;
    /* хэш-функция для строки по схеме Горнера */
    int HashFunction(const string &key, int &size, int p) {
        long long result = 0;
        for (int i = 0; i < key.length(); i++)
            result = (result * p + (int)key[i] + 1) % size;
        return result % size;
    }
    /* поиск */
    int Find(string &key) {
        int hash = HashFunction(key, size, 3); //считаем хеш функцию
        int i = 1;
        int index = hash;
        while (v[index] != nullptr && v[index]->key != key) { //квадратичное пробирование
            index = (index + i) % size;
            i++;
        }
        if (v[index] == nullptr || v[index]->deleted)
            return -1;
        else return index;
    }
    /* вставка, перегруженный метод */
    bool Insert(Element** v, int size, string key, int value) {
        int hash = HashFunction(key, size, 3); //считаем хеш функцию
        int i = 1;
        int index = hash;
        while (v[index] != nullptr && v[index]->key != key) { //квадратичное пробирование
            index = (index + i) % size;
            i++;
        }
        if (v[index] == nullptr || v[index]->deleted) {
            if (v[index] != nullptr)
                delete v[index];
            v[index] = new Element(key, value);
            return true;
        } else {
            return false;
        }
    }
    /* перехеширование */
    bool ReHash() {
        Element** arr = new Element*[size * 2];
        InitNull(arr, size * 2);
        int newn = 0;
        for (int i = 0; i < size; i++) {
            if (v[i] != nullptr && !v[i]->deleted) {
                Insert(arr, this->size * 2, v[i]->key, v[i]->value);
                newn++;
            }
            if (v[i] != nullptr)
                delete v[i];
        }
        delete []v;
        this->v = arr;
        this->size *= 2;
        this->n = newn;
    }
    /* инициализация nullptr */
    void InitNull(Element** v, int size) {
        for (int i = 0; i < size; i++)
            v[i] = nullptr;
    }
public:
    /* конструктор */
    explicit HashTable() {
        this->size = 8;
        this->n = 0;
        v = new Element * [this->size];
        InitNull(v, size);
    }
    /* деструктор */
    ~HashTable() {
        for (int i = 0; i < size; i++) {
            if (v[i] != nullptr)
                delete v[i];
        }
        delete []v;
    }
    /* вставка, перегруженный метод */
    bool Insert(string key, int value) {
        if (((double)this->n) / ((double)this->size) >= 0.75)
            ReHash();
        if (Insert(this->v, this->size, key, value)) {
            n++;
            return true;
        } else return false;

    }
    /* Удаление */
    bool Delete(string key) {
        int index = Find(key);
        if (index != -1) {
            v[index]->deleted = true;
            return true;
        } else return false;
    }
    /* Проверка на наличие */
    bool Has(string key) {
        int index = Find(key);
        if (index != -1)
            return true;
        else return false;
    }
};
int main() {
    HashTable ht;
    string cmd, str;
    while ( !std::cin.fail() ) {
        std::cin >> cmd >> str;
        if (std::cin.fail())
            break;
        if (cmd == "+"){
            if (ht.Insert(str, 0))
                std::cout << "OK" << std::endl;
            else  std::cout << "FAIL" << std::endl;
        } else if (cmd == "-") {
            if (ht.Delete(str))
                std::cout << "OK" << std::endl;
            else  std::cout << "FAIL" << std::endl;
        } else if (cmd == "?") {
            if (ht.Has(str))
                std::cout << "OK" << std::endl;
            else  std::cout << "FAIL" << std::endl;
        }
    }
    return 0;
}
