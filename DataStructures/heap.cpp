#include <iostream>
#include <limits>

using std::cin;
using std::cout;
using std::endl;
using std::numeric_limits;
/* Класс элемента бинарной кучи */
class Node{
   private:
     int index;
     int val;
   public:
     Node(int index, int val) {
		 this->index = index;
		 this->val = val;
	 }
	 int getVal() {
		 return this->val;
	 }
	 int getIndex() {
		 return this->index;
	 }
};
/* Класс бинарной кучи */
class BinHeap {
	private:
	  int size;
	  Node **a;
	  /* функция для обмена местами */
	  void swap(int i, int j){
	    Node* t = a[i];
	    a[i] = a[j];
	    a[j] = t;
	  }
	public:
	  /* Кунструктор кучи, принимает параметр - максимальный размер кучи */
	  BinHeap(int max) {
		  this->size = 0;
		  this->a = new Node*[max];
	  }
	  /* Деструктор */
	  ~BinHeap() {
		  for (int i = 0; i < size; i++) {
			delete a[i];
		  }
		  delete []a;
	  }
	  /* Добавление элемента в кучу */
	  void add(int index, int val) {
		  a[this->size++] = new Node(index, val); //создали элемент
		  int i = size -1;
		  int parent = (i - 1) / 2; //индекс родителя
		  while (i > 0 && a[parent]->getVal() < a[i]->getVal()) { //поднимаем элемент пока он больше родителя, чтобы не потерять свойтво кучи
			  this->swap(parent, i);
			  i = parent;
			  parent = (i - 1) / 2;
		  }
	  }
	  /* востановление свойтв для вершины i */
	  void heapify(int i) {
		  int leftChild;  
          int rightChild;
          int bigestChild;
          bool f = true; //переменная, для хранения информации нужен ли нам еще цикл
          do {
			  leftChild = 2 * i + 1; 
			  rightChild = 2 * i + 2;
              bigestChild = i;
			  if (leftChild < this->size && a[leftChild]->getVal() > a[bigestChild]->getVal()) {
				  bigestChild = leftChild;
			  }
			  if (rightChild < this->size && a[rightChild]->getVal() > a[bigestChild]->getVal()) {
				  bigestChild = rightChild;
			  }
			  
			  this->swap(i, bigestChild);
			  if (i == bigestChild) //свойство выполняется, нам нужно закончить цикл
			    f = false;  
			  i = bigestChild;
		  } while (f);
	  }
	  /* извлечение корня */
	  Node* getRoot() {
		  Node* res = a[0];
		  a[0] = a[this->size - 1];
		  this->size--;
		  if (this->size > 0)
		    this->heapify(0); //востановить свойства кучи, если в ней есть элементы
		  return res;
	  }
	  /* получить размер кучи */
	  int getSize() {
		  return this->size; 
	  }
};

int main() {
    
    int n, k;
    cin >> n;
    BinHeap* bh = new BinHeap(n+10);
	  delete bh;
	  return 0;
}
