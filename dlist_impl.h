//
// Created by Thinkpad on 2020/7/23.
//

#ifndef P5_DLIST_IMPL_H
#define P5_DLIST_IMPL_H

#include "dlist.h"

template <class T>
Dlist<T>::Dlist():
first(0), last(0)
{}

template <class T>
bool Dlist<T>::isEmpty() const {
    if(!first && !last){
        return true;
    }
    return false;
}
// EFFECTS: returns true if list is empty, false otherwise

template <class T>
void Dlist<T>::insertFront(T *op) {
    node *n = new node;
    n->op = op;
    n->prev = nullptr;
    n->next = first;
    if(first){
        first->prev = n;
    }
    first = n;
    // special case when dlist is empty.
    if(!last){
        last = n;
    }
}
// MODIFIES this
// EFFECTS inserts o at the front of the list

template <class T>
void Dlist<T>::insertBack(T *op) {
    node *n = new node;
    n->op = op;
    n->prev = last;
    n->next = nullptr;
    if(last){
        last->next = n;
    }
    last = n;
    // special case when dlist is empty.
    if(!first){
        first = n;
    }
}
// MODIFIES this
// EFFECTS inserts o at the back of the list

template <class T>
T *Dlist<T>::removeFront() {
    // empty case
    if(isEmpty()){
        emptyList e;
        throw e;
    }

    // only one element
    if(first == last){
        T* result = first->op;
        delete first;
        first = last = nullptr;
        return result;
    }

    // normal case
    T* result = first->op;
    first = first->next;
    delete first->prev;
    return result;
}
// MODIFIES: this
// EFFECTS: removes and returns first object from non-empty list
//         throws an instance of emptyList if empty

template <class T>
T *Dlist<T>::removeBack() {
    // empty case
    if(isEmpty()){
        emptyList e;
        throw e;
    }

    // only one element
    T* result = last->op;
    if(first == last){
        delete last;
        first = last = nullptr;
        return result;
    }

    // normal case
    last = last->prev;
    delete last->next;
    return result;

}
// MODIFIES this
// EFFECTS removes and returns last object from non-empty list
//         throws an instance of emptyList if empty

template <class T>
void Dlist<T>::removeAll() {
    while(!isEmpty()){
        removeFront();
    }
}
// EFFECT: called by destructor/operator= to remove and destroy
//         all list elements

template <class T>
void Dlist<T>::copyAll(const Dlist<T> &l) {
    removeAll();
    if(!l.isEmpty()){
        node * it = l.first;
        while(it->next){
            insertBack(it->op);
            it = it->next;
        }
        insertBack(it->op);
    }
}
// EFFECT: called by copy constructor/operator= to copy elements
//         from a source instance l to this instance

template <class T>
T *Dlist<T>::remove(bool (*cmp)(const T*, const T*), T* ref) {
    node * it = first;

    // if the first element satisfies.
    if(cmp(it->op, ref)){
        return removeFront();
    }

    // if the first element does not satisfy.
    while(it){
        if(cmp(it->op, ref)){
            // if cmp is true.
            T* result = it->op;

            // remove this element
            // on the first of the list
            if(it == first){
                return removeFront();
            }
            else if(it == last){
                // on the last of the list
                return removeBack();
            }
            // in somewhere of the list
            it->prev->next = it->next;
            it->next->prev = it->prev;
            delete it;
            return result;
        }
        it = it->next;
    }

    // no elements satisfy.
    return nullptr;
}
// MODIFIES this
// REQUIRES there is only one or zero node in the list satisfying cmp(op, ref) == true
// EFFECTS traverses through the whole list
//         if `op` in a node satisfies cmp(op, ref) == true,
//         removes and returns this object from the list
//         returns NULL pointer if no such node exists

template <class T>
Dlist<T>::Dlist(const Dlist &l):
first(0), last(0)
{
    copyAll(l);
}

template <class T>
Dlist<T> &Dlist<T>::operator=(const Dlist<T> &l) {
    removeAll();
    copyAll(l);
    return *this;
}

template <class T>
Dlist<T>::~Dlist() {
    removeAll();
}

#endif //P5_DLIST_IMPL_H
