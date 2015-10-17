//
// Created by darkphoton on 17/10/15.
//

#ifndef BDT_GENERATOR_MANAGED_PTR_H
#define BDT_GENERATOR_MANAGED_PTR_H

#include <iostream>

using namespace std;

template <typename T>
class managed_ptr {
    T* _ptr;
    unsigned int* _counter;

    unsigned int* getCounter() const {
        return _counter;
    }

    void addPointer(const managed_ptr& obj){
        _ptr = obj.getPtr();
        _counter = obj.getCounter();
        *_counter += 1;
    }
    void removePointer(){
        *_counter -= 1;
        if (*_counter == 0){
            delete _ptr;
            delete _counter;
            --COUNTER;
        }
    }

public:
    static int COUNTER;
    static int CC;
    static int DC;

    managed_ptr(T* ptr) : _ptr(ptr){
        _counter = new unsigned int(1);
        ++CC;
        ++COUNTER;
        cout << "[" << to_string(CC) << "]-[" << to_string(DC) << "]\n";
    }

    managed_ptr(const managed_ptr& source){
        addPointer(source);
    }

    ~managed_ptr(){
        removePointer();
        ++DC;
        cout << "[" << to_string(CC) << "]-[" << to_string(DC) << "]\n";
    }

    T* getPtr() const {
        return _ptr;
    }

    T* operator->(){
        return _ptr;
    }

    managed_ptr& operator=(const managed_ptr& obj){
        removePointer();
        addPointer(obj);
        return *this;
    }
};

template <typename T>
int managed_ptr<T>::COUNTER = 0;

template <typename T>
int managed_ptr<T>::CC = 0;

template <typename T>
int managed_ptr<T>::DC = 0;

#endif //BDT_GENERATOR_MANAGED_PTR_H
