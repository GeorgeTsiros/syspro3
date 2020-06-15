#ifndef BUCKET_H
#define BUCKET_H

#include "myList.h"

struct Record;
template <class T> class myList;

struct bucketRecord {
    char *recordId;
    myList<Record*> *recordsList;
    bucketRecord(char *);
    ~bucketRecord();
};

struct Bucket {
    int bucketSize;
    Bucket *next;
    bucketRecord** records;
    int numOfRecords;  

    Bucket(int);
    myList<Record*>* getBucketRecords(int);
    myList<Record*>* getBucketRecordIds(char*);
    bool searchRecord(char*);
    void insertRecord(char*,Record*);
    ~Bucket();
};

#endif