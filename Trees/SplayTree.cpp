#include <iostream>
#include <functional>

template <typename T, typename V>
class SplayTree {
private:

    class Node {
    public:
        Node(const T& key, Node* left, Node* right, Node* parent, const V& value)
            : key(key), left(left), right(right), parent(parent), value(value) {}
        T key;
        V value;
        Node* left;
        Node* right;
        Node* parent;
    };

public:

    SplayTree() {
        root = nullptr;
    }

    ~SplayTree() {
        postOrder(root, [](Node* v) { delete v; });
    }

    void postOrder(Node* v, const std::function<void(Node*)>& callback) {
        if (v == nullptr) {
            return;
        }
        postOrder(v->left, callback);
        postOrder(v->right, callback);
        callback(v);
    }

    std::pair<Node*, Node*> split(Node* v, T key) {
        if (v == nullptr) {
            return std::make_pair(nullptr, nullptr);
        }
        Node* left;
        Node* right;
        if (v->key > key) {
            std::pair<Node*, Node*> t = split(v->left, key);
            v->left = t.second;
            if (t.second != nullptr) {
                p(t.second) = v;
            }
            left = t.first;
            right = v;
        } else {
            std::pair<Node*, Node*> t = split(v->right, key);
            v->right = t.first;
            if (t.first != nullptr) {
                p(t.first) = v;
            }
            right = t.second;
            left = v;
        }
        return std::make_pair(left, right);
    }

    Node* merge(Node* l, Node* r) {
        if (l == nullptr) {
            return r;
        }
        if (r == nullptr) {
            return l;
        }
        Node* m = max(l);
        splay(m);
        m->right = r;
        p(r) = m;
        return m;
    }

    void add(T x, const V& value) {
        if (root == nullptr) {
            root = new Node(x, nullptr, nullptr, nullptr, V());
        } else {
            add(root, x, value);
        }
    }

    void remove(T key) {
        Node* v = find(root, key);
        splay(v);
        if (v->left != nullptr) {
            v->left->parent = nullptr;
        }
        if (v->right != nullptr) {
            v->right->parent = nullptr;
        }
        Node* tree = merge(v->left, v->right);
        delete v;
        root = tree;
        tree->parent = nullptr;
    }

    V& operator [] (T key) {
        Node* v = find(root, key);
        splay(v);
        root = v;
        return v->value;
    }
private:

    void add(Node*, T key, const V& value) {
        auto trees = split(root, key);
        root = merge(merge(trees.first, new Node(key, nullptr, nullptr, nullptr, value)), trees.second);
    }

    Node* max(Node* v) {
        if (v->right == nullptr) {
            return v;
        } else {
            return max(v->right);
        }
    }

    Node* find(Node* v, T key) {
        if (v->key == key) {
            return v;
        }
        if (v->key > key) {
            return find(v->left, key);
        } else {
            return find(v->right, key);
        }
    }

    Node*& p(Node* v) {
        return v->parent;
    }

    Node*& g(Node* v) {
        return p(p(v));
    }

    void rotateLeft(Node* v) {
        Node* p = this->p(v);
        Node* r = v->right;
        if (p != nullptr) {
            (p->left == v ? p->left : p->right) = r;
        }
        Node* tmp = r->left;
        r->left = v;
        v->right = tmp;
        this->p(v) = r;
        this->p(r) = p;
        if (v->right != nullptr) {
            this->p(v->right) = v;
        }
    }

    void rotateRight(Node* v) {
        Node* p = this->p(v);
        Node* l = v->left;
        if (p != nullptr) {
            (p->right == v ? p->right : p->left) = l;
        }
        Node* tmp = l->right;
        l->right = v;
        v->left = tmp;
        this->p(v) = l;
        this->p(l) = p;
        if (v->left != nullptr) {
            this->p(v->left) = v;
        }
    }

    void splay(Node* v) {
        while (p(v) != nullptr) {
            if (v == p(v)->left) {
                if (g(v) == nullptr) {
                    rotateRight(p(v));          // zig
                } else if (p(v) == g(v)->left) {
                    rotateRight(g(v));          // zig-zig
                    rotateRight(p(v));
                } else {
                    rotateRight(p(v));          // zig-zag
                    rotateLeft(p(v));
                }
            } else {
                if (g(v) == nullptr) {
                    rotateLeft(p(v));           // zig
                } else if (p(v) == g(v)->right) {
                    rotateLeft(g(v));           // zig-zig
                    rotateLeft(p(v));
                } else {
                    rotateLeft(p(v));           // zig-zag
                    rotateRight(p(v));
                }
            }
        }
    }

    Node* root;

};
int main() {
    SplayTree<int, std::string> t;
    t.add(5, "hh");
    t.add(7, "hhi");
    t.add(1, "hh");
    t.add(8, "hh");
    t.remove(1);
    std::cout << t[7];
    std::cout << t[7];
    std::cout << t[7];
    std::cout << t[7];

    return 0;
}
