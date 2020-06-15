#ifndef HEAP_H
#define HEAP_H

#include "../inc/myvector.h"

struct heapPair;

class PriorityQueue {
    private:  
        MyVector<heapPair> A;
        int PARENT(int);
        int LEFT(int);
        int RIGHT(int);
        void heapify_down(int);
        void heapify_up(int);

    public:
        unsigned int size();
        bool empty();
        void push(heapPair);
        void pop();
        heapPair top();

};

#endif
