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

/* Exception */
enum Error_t {
    PARENTHESIS_MISMATCH,
    NOT_ENOUGH_OPERANDS,
    DIVIDE_BY_ZERO,
    TOO_MANY_OPERANDS
};

struct Exception_t: public exception{
    Error_t error;
    string error_info;

    Exception_t(Error_t err, const string& info){
        this->error = err;
        this->error_info = info;
    }
};

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
    T* victim = stack.removeBack();
    delete victim;
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

bool greaterPrecedence(const string & str1, const string & str2){
    if(str1 == "(" || str1 == ")"){
        if(str2 != "(" && str2 != ")"){
            return true;
        }
        return false;
    }
    if(str1 == "*" || str1 == "/"){
        if(str2 == "+" || str2 == "-"){
            return true;
        }
        return false;
    }
    return false;
}
// EFFECTS: return true if operator str1 has greater precedence than str2, return false otherwise.
// REQUIRES: str1 and str2 both be operators.

bool equalPrecedence(const string str1, const string str2){
    if((str1 == "+" || str1 == "-") && (str2 == "+" || str2 == "-")){
        return true;
    }
    if((str1 == "*" || str1 == "/") && (str2 == "*" || str2 == "/")){
        return true;
    }
    if((str1 == "(" || str1 == ")") && (str2 == "(" || str2 == ")")){
        return true;
    }
    return false;
}
// EFFECTS: return true if operator str1 and str2 has equal precedence, return false otherwise.
// REQUIRES: str1 and str2 both be operators.


void getRPN(string * str, string * out, Stack<string> & s){

    // token is a number
    if(str[0] != "+" && str[0] != "-" && str[0] != "*" && str[0] != "/"
    && str[0] != "(" && str[0] != ")"){
        *out = *out + (*str) + " ";
    }
    else if((*str) == "+" || (*str) == "-" || (*str) == "*" || (*str) == "/"){
        // operator
        // string *op = s.top();
        while((!s.empty() && *s.top() != "(" && *s.top() != ")")
        && ((greaterPrecedence(*s.top(), (*str))) || (equalPrecedence(*s.top(), (*str))))
        && (*s.top() != "(")){
            *out = *out + *s.top() + " ";
            s.pop();
        }
        s.push(str);
    }
    else if((*str) == "("){
        s.push(str);
    }
    else if((*str) == ")"){
        while(!s.empty() && *s.top() != "("){
            // pop the operator from the stack onto the output string
            *out = *out + *s.top() + " ";
            s.pop();
        }
        // If the stack runs out without finding a left parenthesis, then mismatched.
        if(s.empty()){
            /// exception
        }
        if(*s.top() == "("){
            s.pop();
        }
    }
}

int stack_plus(Stack<int> & s){
    int value = *s.top();
    s.pop();
    value += *s.top();
    s.pop();
    return value;
}
// EFFECTS: take plus operation of 2 integers on the top of the int stack.
//          Return the sum of 2 integers on the top of the stack.
// REQUIRES: stack s is not empty and has at least 2 operands.
// MODIFIES: int stack s.

int stack_minus(Stack<int> & s){
    int value = -*s.top();
    s.pop();
    value += *s.top();
    s.pop();
    return value;
}
// EFFECTS: take minus operation of 2 integers on the top of the int stack.
//          Return the difference of 2 integers using minus on the top of the stack.
// REQUIRES: stack s is not empty and has at least 2 operands.
// MODIFIES: int stack s.

int stack_multi(Stack<int> & s){
    int value = *s.top();
    s.pop();
    value *= *s.top();
    s.pop();
    return value;
}
// EFFECTS: take multiplication operation of 2 integers on the top of the int stack.
//          Return the product of 2 integers on the top of the stack.
// REQUIRES: stack s is not empty and has at least 2 operands.
// MODIFIES: int stack s.

int stack_divide(Stack<int> & s){
    int value = *s.top();
    s.pop();
    if(value == 0){
        ///exception: divide by 0.
    }
    value = *s.top() / value;
    s.pop();
    return value;
}

void evaluate(string * out){
    Stack<int> istack;
    string op;

    istringstream istream;
    istream.str(*out);
    while(istream >> op){
        if(op[0] >= 48 && op[0] <= 57){
            // operands
            int *val = new int(atoi(op.c_str()));
            istack.push(val);
        }
        else{
            /// exception: too many operands or not enough operands.
            //plus
            int *val = new int;
            if(op == "+"){
                *val = stack_plus(istack);
            }
            // minus
            if(op == "-"){
                *val = stack_minus(istack);
            }
            // multiply
            if(op == "*"){
                *val = stack_multi(istack);
            }
            /// divide: there is an exception.
            if(op == "/"){
                *val = stack_divide(istack);
            }
            istack.push(val);
        }
    }
    cout << *istack.top() << endl;
    istack.pop();
}


int main(){
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

    Stack<string> s;

    string *out = new string("");
    string input, temp;

    // input
    getline(cin, input);
    istringstream iStream(input);

    while(iStream >> temp){
        // read a token.
        string *str = new string("");
        *str = temp;
        // cout << *str << endl;
        getRPN(str, out, s);
    }

    while(!s.empty()){
        // mismatch if there exists parenthesis
        try{
            if(*s.top() == "(" || *s.top() == ")"){
                ///exception: parentheses mismatch
                ostringstream oStream;
                oStream << "ERROR: Parenthesis mismatch" << endl;
                throw Exception_t(PARENTHESIS_MISMATCH, oStream.str());
            }
        }
        catch(Exception_t exception){
            cout << exception.error_info;
        }
        // pop all the operators to output string
        string t = *s.top();
        *out = *out + t + " ";
        s.pop();
    }

    cout << *out << endl;
    evaluate(out);
    delete out;
    return 0;
}