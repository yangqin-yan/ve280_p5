//
// Created by Thinkpad on 2020/7/31.
//
#include <iostream>
#include <cassert>
#include "dlist.h"
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

class AddressOutofBound{};
// exception class: address out of bound

class LRUCache{
private:
    int mem_size; // size of memory
    int *memory;
    struct block{
        int address; // its address in memory
        int data;
    };
    Dlist<block> cache;
    int cur_cache_size; // current length of `cache`
    int max_cache_size; // maximum length of `cache`
    static bool compare(const block *a, const block *b);
// EFFECTS: returns true if two blocks have the same address
public:
    LRUCache(int cache_size, int memory_size); // constructor
// Initialize `cur_cache_size`, `max_cache_size`, `memory`
// Initialize all elements in `memory` to 0
    ~LRUCache(); // destructor
    int read(int address);
// EFFECTS: returns data corresponding to address,
// 0 <= address < mem_size;
// if address is out of bound, throws an exception
//
// if hit,
// removes this block and insert it to the front;
// returns `data`;
// if miss,
// if `cur_cache_size` equals to `max_cache_size`,
// removes the last (least recently used) block
// in the `cache`;
// writes data in the last block
// to the corresponding address in `memory`;
// if `cur_cache_size` < `max_cache_size`,
// increment `cur_cache_size` by 1;
// reads `data` of `address` from `memory`,
// inserts the block with `address` and `data`
// to the front of `cache`;
// returns `data`
    void write(int address, int data);
// EFFECTS: writes data to address, 0 <= address < mem_size
// if address is out of bound, throws an exception
//
// if hit,
// removes this block from list,
// writes `data` to this block,
// and inserts this block to the front;
// if miss,
// if `cur_cache_size` equals to `max_cache_size`,
// removes the last (least recently used) block
// in the `cache`;
// writes data in the last block
// to the corresponding address in `memory`;
// if `cur_cache_size` < `max_cache_size`,
// increment `cur_cache_size` by 1;
// inserts the block with `address` and `data`
// to the front of `cache`
    void printCache();
// EFFECTS: prints the cache in given format
    void printMem();
// EFFECTS: prints the memory in given format
};

bool LRUCache::compare(const block *a, const block *b){
    return a->address == b->address;
}
// EFFECTS: returns true if two blocks have the same address

LRUCache::LRUCache(int cache_size, int memory_size):
mem_size(memory_size), memory(new int[mem_size]), cur_cache_size(0), max_cache_size(cache_size)
{
    for(int i = 0; i < mem_size; i++){
        memory[i] = 0;
    }
}
// EFFECTS: constructor
// Initialize `cur_cache_size`, `max_cache_size`, `memory`
// Initialize all elements in `memory` to 0

LRUCache::~LRUCache(){
    delete[] memory;
    while(!cache.isEmpty()){
        block* victim = cache.removeFront();
        delete victim;
    }
} // destructor

int LRUCache::read(int address){
    if(address < 0 || address >= mem_size){
        ///exception: out of bound
        AddressOutofBound e;
        cout << "ERROR: Address out of bound" << endl;
        throw e;
    }
    block temp;
    temp.data = 0;
    temp.address = address;
    block *op = cache.remove(compare, &temp);
    if(op){
        // insert it to the front
        cache.insertFront(op);
        return op->data;
    }
    //else{
        // do nothing
      //  cache.insertBack(op);
    //}

    // if miss
    if(cur_cache_size == max_cache_size){
        block * victim = cache.removeBack();
        // write data in the last block to the memory.
        memory[victim->address] = victim->data;
        delete victim;
        cur_cache_size--;
    }
    if(cur_cache_size < max_cache_size){
        cur_cache_size++; // expand the current size of cache.
        block * new_block = new block;
        new_block->data = memory[address];
        new_block->address = address;
        cache.insertFront(new_block);
        return new_block->data;
    }
    return -1;
}
// EFFECTS: returns data corresponding to address,
// 0 <= address < mem_size;
// if address is out of bound, throws an exception
//
// if hit,
// removes this block and insert it to the front;
// returns `data`;
// if miss,
// if `cur_cache_size` equals to `max_cache_size`,
// removes the last (least recently used) block
// in the `cache`;
// writes data in the last block
// to the corresponding address in `memory`;
// if `cur_cache_size` < `max_cache_size`,
// increment `cur_cache_size` by 1;
// reads `data` of `address` from `memory`,
// inserts the block with `address` and `data`
// to the front of `cache`;
// returns `data`

void LRUCache::write(int address, int data){
    if(address < 0 || address >= mem_size){
        /// exception
        AddressOutofBound e;
        cout << "ERROR: Address out of bound" << endl;
        throw e;
    }
    // if hit
    block temp;
    temp.data = 0;
    temp.address = address;
    block * op = cache.remove(compare, &temp);
    if(op){
        op->data = data;
        cache.insertFront(op);
    }
    else{
        // if miss
        if(cur_cache_size == max_cache_size){
            block * victim = cache.removeBack();
            memory[victim->address] = victim->data;
            delete victim;
            cur_cache_size--;
        }
        if(cur_cache_size < max_cache_size){
            cur_cache_size++;
            block * new_block = new block;
            new_block->address = address;
            new_block->data = data;
            cache.insertFront(new_block);
        }
    }
}
// EFFECTS: writes data to address.
// REQUIRES: 0 <= address < mem_size
// if address is out of bound, throws an exception
//
// if hit,
// removes this block from list,
// writes `data` to this block,
// and inserts this block to the front;
// if miss,
// if `cur_cache_size` equals to `max_cache_size`,
// removes the last (least recently used) block
// in the `cache`;
// writes data in the last block
// to the corresponding address in `memory`;
// if `cur_cache_size` < `max_cache_size`,
// increment `cur_cache_size` by 1;
// inserts the block with `address` and `data`
// to the front of `cache`

void LRUCache::printMem() {
    for(int i = 0; i < mem_size; i++){
        cout << memory[i] << " ";
    }
    cout << endl;
}
// EFFECTS: prints the memory in given format

void LRUCache::printCache() {
    block * it[3];
    for(int i = 0; i < cur_cache_size; i++){
        it[i] = cache.removeFront();
        cout << it[i]->address << " " << it[i]->data << endl;
    }
    for(int i = cur_cache_size - 1; i >= 0; i--){
        cache.insertFront(it[i]);
    }
}
// EFFECTS: prints the cache in given format

void cache_operation(int & cache_size, int & memory_size){
    LRUCache sys(cache_size, memory_size);
    string cmd;
    while(getline(cin, cmd)){
        istringstream iStream(cmd);
        string str[4];
        iStream >> str[0] >> str[1] >> str[2] >> str[3];
        if(str[0] == "READ"){
            // READ command
            // str[1] is the address
            if(str[1] == ""){
                /// exception: not enough operands
                cout << "ERROR: Not enough operands" << endl;
                continue;
            }
            if(str[2] != ""){
                /// exception: too many operands.
                cout << "ERROR: Too many operands" << endl;
                continue;
            }
            int address = atoi(str[1].c_str());
            try{
                cout << sys.read(address) << endl;
            }
            catch(AddressOutofBound &e){
                continue;
            }
        }
        else if(str[0] == "WRITE"){
            // WRITE command
            // str[1] is the address
            // str[2] is the data
            if(str[2] == ""){
                /// exception: not enough operands
                cout << "ERROR: Not enough operands" << endl;
                continue;
            }
            if(str[3] != ""){
                /// exception: too many operands.
                cout << "ERROR: Too many operands" << endl;
                continue;
            }
            int address = atoi(str[1].c_str());
            int data = atoi(str[2].c_str());
            try{
                sys.write(address, data);
            }
            catch(AddressOutofBound &e){
                continue;
            }
        }
        else if(str[0] == "PRINTCACHE"){
            // PRINTCACHE command
            if(str[1] != ""){
                /// exception: too many operands.
                cout << "ERROR: Too many operands" << endl;
                continue;
            }
            sys.printCache();
        }
        else if(str[0] == "PRINTMEM"){
            // PRINTMEM command
            if(str[1] != ""){
                /// exception: too many operands.
                cout << "ERROR: Too many operands" << endl;
                continue;
            }
            sys.printMem();
        }
        else if(str[0] == "EXIT"){
            if(str[1] != ""){
                /// exception: too many operands.
                cout << "ERROR: Too many operands" << endl;
                continue;
            }
            // EXIT command
            break;
        }
            /// exception : unknown instruction.
        else{
            cout << "ERROR: Unknown instruction" << endl;
        }
    }
}
// EFFECTS: execute the simulation of cache and memory.


int main(){
    int cache_size, memory_size;
    string input;
    getline(cin, input);
    istringstream inputstream(input);
    inputstream >> cache_size >> memory_size;
    cache_operation(cache_size, memory_size);
    return 0;
}