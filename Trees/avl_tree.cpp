#include <iostream>

template < typename T >
class AVLTree{
public:
    T x;
    short height;
    int children;
    AVLTree<T>* left;
    AVLTree<T>* right;
    /* Конструктор */
    AVLTree(T x) {
        this->x = x;
        this->left = nullptr;
        this->right = nullptr;
        this->height = 1;
        this->children = 1;
    }
    /* Деструктор */
    ~AVLTree() {
    }
    /* Метод, возвращающий высоту дерева */
    short GetHeight(AVLTree<T>* p) {
        return p == nullptr ? 0 : p->height;
    }
    /* Метод, возвращающий количество детей */
    int CountChildren(AVLTree<T>* p) {
        return  (p != nullptr ? p->children : 0);
    }
    /* Метод, считающий баланс-фактор дерева */
    short BalanceFactor(AVLTree<T>* p) {
        if (p == nullptr)
            return 0;
        return GetHeight(p->right) - GetHeight(p->left);
    }
    /* Метод для исправления высоты при поворотах*/
    void FixHeight(AVLTree<T>* p) {
        short LeftHeight = GetHeight(p->left);
        short RightHeight = GetHeight(p->right);
        p->height = LeftHeight > RightHeight ? LeftHeight + 1 : RightHeight + 1;
    }
    /* Метод для пересчета детей при поворотах*/
    void FixChildren(AVLTree<T>* p) {
        short LeftCountChildren = CountChildren(p->left);
        short RightCountChildren = CountChildren(p->right);
        p->children = LeftCountChildren + RightCountChildren + 1;
    }
    /* Метод для поворота вправо, вокруг вершины */
    AVLTree<T>* RotateRight(AVLTree<T>* p) {
        AVLTree<T>* q = p->left;
        p->left = q->right;
        q->right = p;
        FixHeight(p);
        FixChildren(p);
        FixHeight(q);
        FixChildren(q);
        return q;
    }
    /* Метод для поворота влево, вокруг вершины */
    AVLTree<T>* RotateLeft(AVLTree<T>* q) {
        AVLTree<T>* p = q->right;
        q->right = p->left;
        p->left = q;
        FixHeight(q);
        FixChildren(q);
        FixHeight(p);
        FixChildren(p);
        return p;
    }
    /* Метод для балансировки дерева */
    AVLTree<T>* BalanceAVLTree(AVLTree<T>* p) {
        FixHeight(p);
        FixChildren(p);
        if (BalanceFactor(p) == 2) {
            if (BalanceFactor(p->right) < 0)
                p->right = RotateRight(p->right);
            return RotateLeft(p);
        }
        if (BalanceFactor(p) == -2) {
            if (BalanceFactor(p->left) > 0)
                p->left = RotateLeft(p->left);
            return RotateRight(p);
        }
        return p;
    }
    /* Метод для вставки элемента*/
    AVLTree<T>* Insert(AVLTree<T>* p, T x, int& result) {
        if (p == nullptr)
            return new AVLTree(x);
        p->children++;
        if (x < p->x) {
            result += CountChildren(p->right) + 1;
            p->left = Insert(p->left, x, result);
        } else {
            p->right = Insert(p->right, x, result);
        }
        return BalanceAVLTree(p);
    }
    /* Поиск минимума в дереве */
    AVLTree<T>* FindMin(AVLTree<T>* p) {
        if (p->left != nullptr)
            return FindMin(p->left);
        else return p;
    }
    /* Удаление минимума в дереве*/
    AVLTree<T>* RemoveMin(AVLTree<T>* p) {
        if( p->left == nullptr )
            return p->right;
        p->left = RemoveMin(p->left);
        return BalanceAVLTree(p);
    }
    /* Удаление по ключу */
    AVLTree<T>* Remove(AVLTree<T>* p, T x) {
        if (p == nullptr)
            return nullptr;
        p->children--;
        if(x < p->x)
            p->left = Remove(p->left, x);
        else if(x > p->x)
            p->right = Remove(p->right, x);
        else {
            AVLTree<T>* l = p->left;
            AVLTree<T>* r = p->right;
            delete p;
            if (r == nullptr)
                return l;
            AVLTree<T>* min = FindMin(r);
            min->right = RemoveMin(r);
            min->left = l;
            return BalanceAVLTree(min);
        }
        return p;
    }
    /* Метод для удаления дерева */
    void DeleteTree(AVLTree<T>* p) {
        if (p->left != nullptr)
            DeleteTree(p->left);
        if (p->right != nullptr)
            DeleteTree(p->right);
        delete p;
    }
    /* Поиск ключа по номеру ребенка в порядке убывания */
    int FindKeyByNumber(AVLTree<T>* p, int num) {
        if (p == nullptr)
            return 0;
        if (CountChildren(p->right) == num)
            return p->x;
        if (CountChildren(p->right) > num)
            return FindKeyByNumber(p->right, num);
        else return FindKeyByNumber(p->left, num - CountChildren(p->right) - 1);
    }
};

int main() {
    int n;
    std::cin >> n;

    int command, height;
    AVLTree<int> *avl = nullptr;
    int res;

    for (int i = 0; i < n; i++) {
        std::cin >> command >> height;
        if (command == 1) {
            res = 0;
            if (avl == nullptr) {
                avl = new AVLTree<int> (height);
                std::cout << 0 << std::endl;
            } else {
                avl = avl->Insert(avl, height, res);
                std::cout << res << std::endl;
            }
        } else {
           if (avl != nullptr)
             avl = avl->Remove(avl, avl->FindKeyByNumber(avl, height));
        }
    }

    if (avl != nullptr)
      avl->DeleteTree(avl);
    return 0;
}
