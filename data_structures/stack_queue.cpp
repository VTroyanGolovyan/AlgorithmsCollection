#include <iostream>

using std::cin;
using std::cout;
using std::freopen;
class Elem {
  private:
    Elem* next;
    int val;
  public:
    /* constructor */
    Elem(int val) {
      this->val = val;
      this->next = nullptr;
	}
	/* check next element */
    bool hasNext() {
	  if (this->next != nullptr) {
		return true;
	  } else {
	    return false;
	  }
	}
	/* set next element */
	void setNext(Elem* next) {
	  this->next = next;
	}
	/* get pointer */
	Elem* getNext() {
	  return this->next;
	}
	/* set value */
	void setVal(int val) {
	  this->val = val;
	}
	/* get value */
	int getVal() {
	  return this->val;
	}
	
};

class Stack{
  private:
    Elem* first; //stack front
  public:
    /* constructor */
    Stack() {
	  this->first = nullptr;
	}
	/* destructor */
	~Stack() {
	  while (!this->is_empty()) {
		  this->pop();
	  }
	}
	/* push new element */
	void push(int val){
	  if (this->first == nullptr) {
		 this->first = new Elem(val);
	  } else {
		 Elem* tmp = this->first;
		 this->first = new Elem(val);
		 this->first->setNext(tmp);
	  }
	}
	/* get and delete front element */
	int pop(){
	  if (!this->is_empty()) {
		  Elem* tmp = this->first;
		  this->first = this->first->getNext(); 
		  int val = tmp->getVal();
		  delete tmp;
		  return val;
	  } else return -1;
	}
    /* check for items */
	bool is_empty() {
	  if (this->first != nullptr) {
		return false;
	  } else {
	    return true;
	  }
	}
};

class Queue {
  private:
    Stack* left; //первый стек
    Stack* right; //второй стек
  public:
    /* constructor */
    Queue() {
	  this->left = new Stack();
	  this->right = new Stack(); 
	}
	/* destructor */
	~Queue() {
	  delete this->left;
	  delete this->right; 
	}
	/* push element in the end */
    void push_back(int val) {
	  left->push(val);
	}
	/* get and delete first element */
	int pop_front() {
	  if (!this->right->is_empty()) {
		  return this->right->pop();
	  } else if (!this->left->is_empty()) {
		  while (!this->left->is_empty()) {
		    this->right->push( this->left->pop() );
		  }
		  return this->right->pop();
	  } else {
	      return -1;
	  }
	}
};

int main() {
  Queue *q = new Queue();
  q->push_back(5);
  cout<<q->pop_front();
  return 0;
}
