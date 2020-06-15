#include "../inc/functs.h"
#include "../inc/map.h"
#include "../inc/worker.h"
#include "../inc/workersDirList.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>


using namespace std;

workersDirList::workersDirList() {
    WorkerNodeHeadPtr = NULL;
    countries = 0;
    pathLength = 0;
}

workersDirList::~workersDirList() {
    FilePathNode* temp = this->WorkerNodeHeadPtr;
    while (temp != NULL) {
        FilePathNode* t = temp;
        temp = temp->next;
        delete t;
    }
}

//add new country to worker
void workersDirList::assign(char * filePath) {
    if (WorkerNodeHeadPtr == NULL) {
        FilePathNode* node = new FilePathNode();
        node->filePath = filePath;
        countries++;
        pathLength += strlen(filePath);
        node->next = NULL;
        WorkerNodeHeadPtr = node;
    } else {
        FilePathNode* node = new FilePathNode();
        node->filePath = filePath;
        countries++;
        pathLength += strlen(filePath);
        node->next = WorkerNodeHeadPtr;
        WorkerNodeHeadPtr = node;
    }
}

char * workersDirList::merge() {
    char* totalCountries = new char [pathLength + countries + 1]();
    FilePathNode* filePathNode = WorkerNodeHeadPtr;
    while (filePathNode) {
        strcat(totalCountries, filePathNode->filePath);
        if (filePathNode->next != NULL) {
            strcat(totalCountries, " ");
        }
        filePathNode = filePathNode->next;
    }
    cout << "Merge " << countries << endl;
    return totalCountries;
}