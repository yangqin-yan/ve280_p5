//
// Created by Thinkpad on 2020/7/25.
//
#include <iostream>
#include <cassert>
#include "dlist.h"
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

template <class T>
class Stack{

    public:

    void push(T * object);
    // EFFECTS: put the object at the top of the stack.
    // MODIFIES: this.

    void pop();
    // EFFECTS: remove the object of the top of the stack. Throw emptyList if there is no element
    //          in the stack.
    // MODIFIES: this.


    T* top();
    // EFFECTS: return the top of the stack. Throw emptyList if there is no element.

    bool empty() const;
    // EFFECTS: return true if the stack is empty, return false otherwise.


    private:

    Dlist<T> stack;
    // last == top;

};

template <class T>
void Stack<T>::push(T * object) {
    stack.insertBack(object);
}
// EFFECTS: put the object at the top of the stack.
// MODIFIES: this.

template <class T>
void Stack<T>::pop() {
    stack.removeBack();
}
// EFFECTS: remove the object of the top of the stack. Throw emptyList if there is no element
//          in the stack.
// MODIFIES: this.

template <class T>
T* Stack<T>::top() {
    T* temp = stack.removeBack();
    stack.insertBack(temp);
    return temp;
}
// EFFECTS: return the top of the stack. Throw emptyList if there is no element.

template <class T>
bool Stack<T>::empty() const {
    return stack.isEmpty();
}
// EFFECTS: return true if the stack is empty, return false otherwise.



int main(int argc, char* argv[]){
    /*Stack<int> a;

    int *n = new int(1);
    int *l = new int(2);
    int *k = new int(3);
    int *s = new int(4);
    a.push(n);
    assert(*a.top() == 1);
    a.push(l);
    assert(*a.top() == 2);
    a.push(k);
    assert(*a.top() == 3);
    a.push(s);
    assert(*a.top() == 4);
    a.pop();
    assert(*a.top() == 3);
    a.push(s);
    assert(*a.top() == 4);
    a.pop();
    a.pop();
    assert(*a.top() == 2);


    delete n;
    delete l;
    delete k;
    delete s;*/

    



    return 0;
}