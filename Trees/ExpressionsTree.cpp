#include <iostream>
#include <functional>

class Tree {
public:
    Tree(Tree* left, Tree*right) : left(left), right(right) {}
    Tree* left;
    Tree* right;

    void inOrder(
        const std::function<void(Tree*)> &callback
    ) {
        if (left != nullptr) {
            left->inOrder(callback);
        }
        callback(this);
        if (right != nullptr) {
            right->inOrder(callback);
        }
    }

    void preOrder (
        const std::function<void(Tree*)> &callback
    ) {
        callback(this);
        if (left != nullptr) {
            left->preOrder(callback);
        }
        if (right != nullptr) {
            right->preOrder(callback);
        }
    }

    void postOrder(
        const std::function<void(Tree*)> &callback
    ) {
        if (left != nullptr) {
            left->postOrder(callback);
        }

        if (right != nullptr) {
            right->postOrder(callback);
        }
        callback(this);
    }

    int countLevel(
        int current_level,
        int find_level
    ) const {
        if (current_level == find_level) {
            return 1;
        }
        int result = 0;
        if (left != nullptr) {
            result += left->countLevel(current_level + 1, find_level);
        }

        if (right != nullptr) {
            result += right->countLevel(current_level + 1, find_level);
        }
        return result;
    }

    virtual void view(std::ostream &out) = 0;
    virtual int calculate() = 0;
    virtual ~Tree() = default;
};

class MultiplyTree : public Tree {
public:
    MultiplyTree(Tree* left, Tree*right) : Tree(left, right) {}
    int calculate() override {
        return left->calculate() * right->calculate();
    }
    void view(std::ostream &out) override {
        out << '*';
    }
    ~MultiplyTree() override = default;
};

class SumTree : public Tree {
public:
    SumTree(Tree* left, Tree*right) : Tree(left, right) {}
    int calculate() override {
        return left->calculate() + right->calculate();
    }
    void view(std::ostream &out) override {
        out << '+';
    }
    ~SumTree() override = default;
};

class SubtractionTree : public Tree {
public:
    SubtractionTree(Tree* left, Tree*right) : Tree(left, right) {}
    int calculate() override {
        return left->calculate() - right->calculate();
    }
    void view(std::ostream& out) override {
        out << '-';
    }
    ~SubtractionTree() override = default;
};

class NumberTree : public Tree {
public:
    NumberTree(Tree* left, Tree*right, int number) : Tree(left, right), number(number) {}
    int calculate() override {
        return number;
    }
    void view(std::ostream& out) override {
        out << number;
    }
    ~NumberTree() override = default;
private:
    int number;
};

bool sign(char ch) {
    if (ch == '+') {
        return true;
    }
    if (ch == '-') {
        return true;
    }
    if (ch == '*') {
        return true;
    }
    return false;
}


Tree* operand(const std::string& expression, size_t &i) {
    int number = 0;
    while (i < expression.length() && !sign(expression[i])) {
        number *= 10;
        number += expression[i] - 48;
        ++i;
    }
    return new NumberTree(nullptr, nullptr, number);
}

Tree* term(const std::string& expression, size_t &i) {
    Tree* tree = operand(expression, i);
    while (i < expression.length() && expression[i] == '*') {
        ++i;
        tree = new MultiplyTree(tree, operand(expression, i));
    }
    return tree;
}

Tree* buildTree(const std::string& expression) {
    size_t i = 0;
    Tree* tree = term(expression, i);
    while (i < expression.length()) {
        if (sign(expression[i])) {
            if (expression[i] == '+') {
                ++i;
                tree = new SumTree(tree, term(expression, i));
            } else if (expression[i] == '-') {
                ++i;
                tree = new SubtractionTree(tree, term(expression, i));
            }
        }
    }
    return tree;
}

void prefixForm(std::ostream& out, Tree* tree) {
    tree->preOrder(
        [&](Tree* node) {
            node->view(out);
            std::cout << " ";
        }
    );
    out << std::endl;
}

void removeTree(Tree* tree) {
    tree->postOrder(
        [&](Tree* node) {
            delete node;
        }
    );
}

int calculateCount(Tree* tree, int level) {
    return tree->countLevel(0, level);
}

int main() {
    std::string expression;
    std::cin >> expression;
    Tree* tree = buildTree(expression);
    int level;
    std::cin >> level;
    std::cout << "Count elements in level " << level << ":" << std::endl;
    std::cout << calculateCount(tree, level) << std::endl;
    std::cout << "Prefix form:" << std::endl;
    prefixForm(std::cout, tree);
    std::cout << "Left calculate:" << std::endl;
    if (tree != nullptr && tree->left != nullptr) {
        std::cout << tree->left->calculate() << std::endl;
    } else {
        std::cout << "tree left or tree is empty :(" << std::endl;
    }
    removeTree(tree);
    return 0;
}
