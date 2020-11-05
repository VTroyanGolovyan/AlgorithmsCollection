#include <iostream>
#include <vector>

template <typename T>
class Node {
public:
    T key;
    Node* prev;
    Node(T key) {
        this->key = key;
        this->prev = nullptr;
    }
    Node(T key, Node<T>* prev) {
        this->key = key;
        this->prev = prev;
    }
};

template <typename T>
class PersistentStack {
private:
   std::vector <Node<T>*> states;
public:
   PersistentStack() {
       states.push_back(nullptr);
   }
   ~PersistentStack() {
       for (Node<T>*& node : states) {
           delete node;
       }
   }
   int push(int state, T key) {
       states.push_back(new Node<T>(key, states[state]));
       return states.size() - 1;
   }
   int pop(int state) {
       Node<T>* current = states[state];
       Node<T>* newTop = nullptr;
       if (current->prev == nullptr)
           newTop = nullptr;
       else newTop = new Node<T>(current->prev->key, current->prev->prev);
       states.push_back(newTop);
       return states.size() - 1;
   }
   T top(int state) {
       return states[state]->key;
   }
};
int main() {
    PersistentStack<int> ps;
    int state1 = ps.push(0,5);
    state1 = ps.push(state1, 6);
    state1 = ps.push(state1, 8);
    std::cout << ps.top(state1) << std::endl;
    int state2 = ps.push(0,15);
    state2 = ps.push(state2, 56);
    state2 = ps.push(state2, 87);
    std::cout << ps.top(state2) << std::endl;
    int state3 = ps.pop(state2);
    std::cout << ps.top(state2) << std::endl;
    std::cout << ps.top(state3) << std::endl;
    return 0;
}
