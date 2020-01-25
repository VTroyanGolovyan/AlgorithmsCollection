/* Троян-Головян Владислав Дмитриевич */
/* 2_1. Декартово дерево. Разница глубин */
/*
    Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.

    Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi). Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
    Добавление узла в декартово дерево выполняйте следующим образом: При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом. Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x, а во втором больше или равны x. Получившиеся два дерева сделайте дочерними для нового узла (x, y). Новый узел вставьте на место узла P.

    Построить также наивное дерево поиска по ключам Xi.
    Т.е., при добавлении очередного числа K в наивное дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.

    Вычислить разницу глубин наивного дерева поиска и декартового дерева. Разница может быть отрицательна, необходимо вывести модуль разности.
*/
#include <iostream>

template <typename keyT, typename priorityT>
class Treap{
public:
    keyT x;
    priorityT y;
    Treap<keyT, priorityT>* left;
    Treap<keyT, priorityT>* right;
    Treap<keyT, priorityT>* parent;
    Treap(keyT x, priorityT y, Treap<keyT, priorityT>* left, Treap<keyT, priorityT>* right) {
        this->x = x;
        this->y = y;
        this->left = left;
        this->right = right;
    }
    ~Treap() {
        if (this->left != nullptr) {
            delete this->left;
        }
        if (this->right != nullptr) {
            delete this->right;
        }
    }
    Treap<keyT, priorityT>* Merge(Treap<keyT, priorityT>* firstTree, Treap<keyT, priorityT>* secondTree) {
        if (firstTree == nullptr)
            return secondTree;
        if (secondTree == nullptr)
            return firstTree;
        if (firstTree->y > secondTree->y) {
            firstTree->right = Merge(firstTree->right, secondTree);
            return firstTree;
        } else {
            secondTree->left = Merge(firstTree, secondTree->left);
            return secondTree;
        }
    }
    void Split(keyT x, Treap<keyT, priorityT>*& outLeft, Treap<keyT, priorityT>*& outRight) {
        Treap<keyT, priorityT>* newTreap = nullptr;
        if (this->x <= x) {
            if (this->right == nullptr)
                outRight = nullptr;
            else this->right->Split(x, newTreap, outRight);
            this->right = newTreap;
            outLeft = this;
        } else {
            if (this->left == nullptr)
                outLeft = nullptr;
            else this->left->Split(x, outLeft, newTreap);
            this->left = newTreap;
            outRight = this;
        }
    }
    Treap<keyT, priorityT>* Insert(Treap<keyT, priorityT>* tp, keyT x, priorityT y) {
        if (tp == nullptr)
            return new Treap<keyT, priorityT>(x, y, nullptr, nullptr);
        if (tp->y > y) {
            if (x < tp->x)
                tp->left = Insert(tp->left, x, y);
            else tp->right = Insert(tp->right, x, y);
            return tp;
        } else {
            Treap<keyT, priorityT>* outLeft = nullptr;
            Treap<keyT, priorityT>* outRight = nullptr;
            tp->Split(x, outLeft, outRight);
            return new Treap<keyT, priorityT>(x, y, outLeft, outRight);
        }
    }
    int Depth(Treap<keyT, priorityT>* tp) {
        int lDepth = 0;
        int rDepth = 0;
        if (tp->left != nullptr)
            lDepth = Depth(tp->left);
        if (tp->right != nullptr)
            rDepth = Depth(tp->right);
        return 1 + ( lDepth > rDepth ? lDepth : rDepth);
    }
};

int main() {
    int n;
    std::cin >> n;
    int x, y;
    std::cin >> x >> y;

    Treap <int, int>* tp = new Treap <int, int>(x, y, nullptr, nullptr);

    for (int i = 1; i < n; i++) {
        std::cin >> x >> y;
        tp = tp->Insert(tp, x, y);
    }

    delete tp;
    return 0;
}
