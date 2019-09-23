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

int main() {
  Stack* st= new Stack();
  st->push(5);
  cout<<st->pop();
  return 0;
}
