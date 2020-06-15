#include <cstdlib>
#include <cstring>
#include "../inc/bucket.h"
#include "../inc/myList.h"
#include "../inc/record.h"

#define BUCKETRECORDSIZE sizeof(bucketRecord)

/*
 * Here we have all the hashtable's bucket functionality,
 * which is implemented like singly linked list with blocks
 */


bucketRecord::bucketRecord(char* key) {
    this->recordId = (char*)malloc((strlen(key)+1) * sizeof(char));
    strcpy(recordId,key);
    recordsList = new myList<Record*>;
}

bucketRecord::~bucketRecord() {
    free(recordId);
    delete recordsList;
}


Bucket::Bucket(int size) {
    this->bucketSize = size;
    this->numOfRecords = 0;
    this->next = NULL;
    this->records = new bucketRecord*[bucketSize/BUCKETRECORDSIZE];
}


/*
 * Searching if record exists already
 */
bool Bucket::searchRecord(char *key) {
    Bucket *temp = this;
    while(temp != NULL) {
        for(int i=0; i<temp->numOfRecords; i++) {
            Node<Record*> *rec = temp->records[i]->recordsList->initList();
            while(rec != NULL) {
                if(strcmp(rec->data->recordId,key) == 0) {
                    return true;
                }
                rec = rec->next;
            }
        }
        temp = temp->next;
    }
    return false;
}

/*
 * Here we search for the transactions of a specific user
 */

myList<Record*>* Bucket::getBucketRecordIds(char* key) {
    Bucket *temp = this;
    while(temp != NULL) {
        for(int i=0; i<temp->numOfRecords; i++) {
            if(strcmp(temp->records[i]->recordId,key) == 0) {
                return temp->records[i]->recordsList;
            }
        }
        temp = temp->next;
    }
    return NULL;
}




/*
 * Here we insert a transaction into the bucket
 * If user exists already then we push the transaction into the list of transactions,
 * otherwise we make a new Record and if necessary a new Bucket first
 */
void Bucket::insertRecord(char* key,Record *rec) {
    Bucket *last,*temp=this;
    while(temp!=NULL) {
        last = temp;
        for(int i=0; i<temp->numOfRecords; i++) {
            if(strcmp(temp->records[i]->recordId,key) == 0) {
                temp->records[i]->recordsList->insert(rec);
                return ;
            }
        }

        temp = temp->next;
    }

    if(last->numOfRecords == last->bucketSize/BUCKETRECORDSIZE) {
        last->next = new Bucket(bucketSize);
        last = last->next;
    }
    last->records[last->numOfRecords] = new bucketRecord(key);
    last->records[last->numOfRecords++]->recordsList->insert(rec);
}

//Here we just free the memory
Bucket::~Bucket() {
    if(this != NULL) {
        for(int i=0; i<this->numOfRecords; i++) {
            delete this->records[i];
        }
        delete []this->records;
        delete this->next;    
    }
}
