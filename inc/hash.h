#ifndef HASH_H
#define HASH_H

#include "myList.h"

struct Bucket;
struct Record;
template <class T> class myList;

class Hash {
    private:
        int BUCKET;
        Bucket **table;

    public:
        Hash(int,int);
        myList<Record*>* getBucketRecordIds(char*);
        bool searchRecordId(char*);
        void insertItem(char*,Record*);
        int hash(char*);
        int returnNumOfBuckets();
        ~Hash();
};

#endif


