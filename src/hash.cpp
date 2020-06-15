#include "../inc/myList.h"
#include "../inc/bucket.h"
#include "../inc/record.h"
#include "../inc/hash.h"
#include <iostream>
#include <cstring>


/*
 * HashTable functionality insert,search and other functions
 * All hashtable functions call the respective ones from the struct bucket
 */

Hash::Hash(int bck,int bucketSize) {
    BUCKET = bck;
    table = new Bucket*[BUCKET];
    for(int i=0; i<BUCKET; i++) {
        table[i] = new Bucket(bucketSize);
    }
}

void Hash::insertItem(char* id,Record* tr) {
    int index = hash(id);
    table[index]->insertRecord(id,tr);
}

bool Hash::searchRecordId(char* recordId) {
    for(int i=0; i<BUCKET; i++) {
        if(table[i]->searchRecord(recordId))
            return true;
    }
    return false;
}

myList<Record*>* Hash::getBucketRecordIds(char* id) {
    int index = hash(id);
    myList<Record*> *lst = table[index]->getBucketRecordIds(id);
    return lst;
}

/* As a hash function, I am taking the sum of the characters of the 'string' 
 * and take the remainder from the division with 'BUCKET'
 */
int Hash::hash(char* s) {
    long int hashval=0;
    for (int i=0; i<strlen(s); i++)  
        hashval += (int)s[i];
    return hashval % BUCKET;
}

int Hash::returnNumOfBuckets() {
    return BUCKET;
}

Hash::~Hash() {
    for(int i=0; i<BUCKET; i++) {
        delete table[i];
    }
    delete []table;
}
