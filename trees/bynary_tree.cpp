#include <iostream>

template <typename keyT>
class Elem{
private:
    keyT x;
    Elem<keyT>* left;
    Elem<keyT>* right;
public:
    Elem(keyT x) {
        this->x = x;
        this->left = nullptr;
        this->right = nullptr;
    }
    ~Elem() {

    }
    int GetVal() {
        return this->x;
    }
    void SetLeft(Elem<keyT>* left) {
        this->left = left;
    }
    void SetRight(Elem<keyT>* right) {
        this->right = right;
    }
    Elem* GetLeft() {
        return this->left;
    }
    Elem* GetRight() {
        return this->right;
    }
};

template <typename keyT>
class BinTree{
private:
    Elem<keyT>* root;
    Elem<keyT>* Add(Elem<keyT> * node, keyT x) {
        if (node == nullptr) {
            return new Elem<keyT>(x);
        } else if (node->GetVal() < x) {
            node->SetRight( Add(node->GetRight(), x) );
        } else {
            node->SetLeft( Add(node->GetLeft(),x) );
        }
        return node;
    }
    void DeleteTree(Elem<keyT>* node) {
        if (node != nullptr) {
            if ( node->GetLeft() != nullptr) {
                DeleteTree(node->GetLeft());
            }
            if ( node->GetRight() != nullptr) {
                DeleteTree(node->GetRight());
            }
            delete node;
        }
    }
public:
    BinTree() {
        this->root = nullptr;
    }
    ~BinTree() {
        DeleteTree(this->root);
    }
    Elem<keyT>* GetRoot() {
        return this->root;
    }
    void Insert(keyT x) {
        this->root = this->Add(this->root, x);
    }
    void InorderTraversal(Elem<keyT>* node) {
        if (node != nullptr) {

            if (node->GetLeft() != nullptr) {
                InorderTraversal(node->GetLeft());
            }
            std::cout << node->GetVal() << " ";
            if (node->GetRight() != nullptr) {
                InorderTraversal(node->GetRight());
            }
        }
    }
    int Depth(Elem<keyT>* t) {
        int lDepth = 0;
        int rDepth = 0;
        if (t->GetLeft() != nullptr)
            lDepth = Depth(t->GetLeft());
        if (t->GetRight() != nullptr)
            rDepth = Depth(t->GetRight());
        return 1 + ( lDepth > rDepth ? lDepth : rDepth);
    }
};

int main() {
    int n;
    std::cin >> n;
    int x;
    std::cin >> x;
    BinTree <int>* bt = new BinTree<int>();
    bt->Insert(x);

    for (int i = 1; i < n; i++) {
        std::cin >> x;
        bt->Insert(x);
    }

    delete bt;
    return 0;
}
