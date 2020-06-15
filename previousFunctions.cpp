#include <iostream>
#include <cstdio>
#include <cstring>
#include "../inc/heapPair.h"
#include "../inc/heap.h"
#include "../inc/myList.h"
#include "../inc/hash.h"
#include "../inc/functs.h"
#include "../inc/record.h"

//  Search for duplicate Records
 
bool isValidRecordId(MyVector<Record*>& records,char* recordId) {
    for(int i=0; i<records.size(); i++) {
        if(strcmp(records[i]->getRecordId(),recordId) == 0) {
            return false;
        }
            
    }
    return true;
}

bool virusFound(MyVector<char*>& viruses,char* virus) {
    for (int i=0; i<viruses.size(); i++) {
        if (strcmp(viruses[i],virus) == 0)
            return true;
    }
    return false;
}

//returns 1 if d1 is after d2 --- return -1 if d1 is before d2 --- return 0 if they are equal

int compareDates(char *d1, char *d2)
{
    int days1,months1,years1;
    int days2,months2,years2;

    sscanf(d1,"%d-%d-%d",&days1,&months1,&years1);
    sscanf(d2,"%d-%d-%d",&days2,&months2,&years2);

    // printf("%d-%d-%d, %d-%d-%d\n",days1,months1,years1,days2,months2,years2);
    if (years1 < years2) { 
        return -1;
    }
    else if (years1 > years2) { 
        return 1;
    }
    else {
        if (months1 < months2) {
            return -1;
        }
        else if (months1 > months2) { 
            return 1;
        }
        else {
            if (days1 < days2) {
                return -1;
            }
            else if (days1 > days2) {
                return 1;
            }
            else { 
                return 0;
            }
        }
    }            
}

void pairSwap(heapPair& x,heapPair& y)
{
     heapPair temp;
     temp=x;
     x=y;
     y=temp;
}


void InsertPatientRecord(MyVector<Record*>& records, Hash& diseaseHashTable, Hash& countryHashTable) {
    MyVector<char*> arg;
    char *argument,*exitDate;

    while ((argument = strtok(NULL,REC_DELIM)) != NULL) {
        arg.add(argument);
    }
    
    if (arg.size() < 6 || arg.size() > 7) {
        std::cerr << "You have given wrong number of arguments";
        return;
    }

    if (!isValidRecordId(records, arg[0])) {
        std::cerr << "There is already a record with this id " << arg[0] << std::endl; 
        return;
    }

    if (arg.size() == 6) {
        exitDate = strdup("-");
    }
    else {
        if (compareDates(arg[5], arg[6]) > 0 && strcmp(exitDate,"-") !=0) {
            std::cerr << "Exit date " << arg[6] << " can not be earlier than entry date " << arg[5] << std::endl;
            return;
        }
        exitDate = strdup(arg[6]);
    }

    Record *rec = new Record(strdup(arg[0]),strdup(arg[1]),strdup(arg[2]),strdup(arg[3]),strdup(arg[4]),strdup(arg[5]),exitDate);
    records.add(rec);

    diseaseHashTable.insertItem(rec->diseaseID,rec);
    countryHashTable.insertItem(rec->country,rec);
    std::cout << "Record added" << std::endl;
}

void recordPatientExit(MyVector<Record*>& records)   {
    MyVector<char*> arg;
    char *argument,*exit, *recordId;
    bool flag = false;

    while ((argument = strtok(NULL,REC_DELIM)) != NULL) {
        arg.add(argument);
    }

    if (arg.size() != 2) {
        std::cerr << "You have given wrong number of arguments" << std::endl;
        return;
    }
    
    recordId = arg[0];
    exit = arg[1];

    for(int i=0; i<records.size(); i++) {
        if(strcmp(records[i]->getRecordId(),recordId) == 0) {
            flag = true;
            if (strcmp(records[i]->exitDate,"-") != 0) {
                std::cerr << "This record already has an exit date" << std::endl;
                return;
            }
            if (compareDates(records[i]->entryDate,exit) > 0) {
                std::cerr << "Exit date can not be earlier than entry date " << std::endl;
                return;
            }
            records[i]->setNewExitDate(strdup(exit));
            std::cout << "Record updated " << std::endl;
        }
    }
    if (!flag) {
        std::cerr << "Not found" << std::endl;
    }
}


void globalDiseaseStats(Hash& diseaseHashTable,MyVector<char*>& viruses) {
    bool flag;
    char *argument,*earliest,*latest;
    MyVector<char*> arg; 
    while ((argument = strtok(NULL,REC_DELIM)) != NULL) {
        arg.add(argument);
    }

    if (arg.size() == 1)
    {
        std::cerr << "You have only given an entry date. Give an exit date too" << std::endl;
        return;
    }
    
    else if (arg.size() > 2){
        std::cerr << "You have given too many arguments." << std::endl;
        return;
    }
    else if (arg.size() == 2){
        flag = false;
        earliest = arg[0];
        latest = arg[1];
    }
    else{
        flag = true;
    }

    for(int i = 0; i< viruses.size(); i++) {

        int count = 0;
        myList<Record*>* lst = diseaseHashTable.getBucketRecordIds(viruses[i]);

        if(lst == NULL) {
            continue;
        }

        Node<Record*> *virusRecord = lst->initList();
        std::cout << virusRecord->data->diseaseID << ":";

        while(virusRecord != NULL) {
            if (!flag) {
                if (compareDates(virusRecord->data->entryDate,earliest) >= 0 &&  compareDates(virusRecord->data->entryDate,latest) <= 0) {
                    count++;
                }
            }
            else {
                count++;
            }
            virusRecord = virusRecord->next;
        }
        std::cout << count << std::endl;
    }
}


void diseaseFrequency(Hash& diseaseHashTable) {
   char *argument;
   int count=0;
   MyVector<char*> arg; 
   while ((argument = strtok(NULL,REC_DELIM)) != NULL) {
        arg.add(argument);
    }


    if (arg.size() < 3 || arg.size() > 4)
    {
        std::cerr << "Wrong number of arguments" << std::endl;
        return;
    }  

    char *virusName = arg[0], *earliest = arg[1], *latest = arg[2] , *country = NULL;
   //std::cout << virusName << std::endl;

    if (arg.size() == 4) country = arg[3]; 

    myList<Record*>* lst = diseaseHashTable.getBucketRecordIds(virusName);
    if(lst == NULL)
    {
        std::cout << "No patients with these virus" << std::endl; // Instead, I could write goto 211;
        return;
    }

    Node<Record*> *virusRecord;
    for(virusRecord = lst->initList(); virusRecord != NULL; virusRecord=virusRecord->next) {
        if (compareDates(virusRecord->data->entryDate,earliest) >= 0 &&  compareDates(virusRecord->data->entryDate,latest) <= 0) {
            if (country == NULL) {
                count++;
            }
            else{
                if (strcmp(virusRecord->data->country, country) == 0)
                    count++;
            }
        }
    }
    std::cout << virusName << " " << count << std::endl;
    return;
} 
    
void numCurrentPatients(Hash& diseaseHashTable, MyVector<char*>& viruses) {
    char *disease = strtok(NULL,REC_DELIM);
    int count;

    if (disease != NULL){ 
        count = 0;
        myList<Record*>* lst = diseaseHashTable.getBucketRecordIds(disease);

        if(lst == NULL) {
            printf("No current patients found with these virus\n");
            return;
        }

        Node<Record*> *virusRecord = lst->initList();
        std::cout << virusRecord->data->diseaseID << ":";
        
        while(virusRecord != NULL) {
            if(strcmp(virusRecord->data->exitDate,"-") == 0) {
                count++;
            }
            virusRecord = virusRecord->next;
        }
        std::cout << count << std::endl;
    }
    else {
        for(int i = 0; i< viruses.size(); i++) {

            count = 0;
            myList<Record*>* lst = diseaseHashTable.getBucketRecordIds(viruses[i]);

            if(lst == NULL) {
                continue;
            }

            Node<Record*> *virusRecord = lst->initList();
            std::cout << virusRecord->data->diseaseID << " ";
            
            while(virusRecord != NULL) {
                if(strcmp(virusRecord->data->exitDate,"-") == 0) {
                    count++;
                }
                virusRecord = virusRecord->next;
            }
            std::cout << count << std::endl;
        }
    }

}

void insertToFrequency(MyVector<heapPair>& frequencyVector,char* key) {
    for (int i=0; i<frequencyVector.size(); i++) {
        if(strcmp(frequencyVector[i].key,key) == 0) {
            frequencyVector[i].count++;
            return;
        }
    }
    heapPair hPair;
    hPair.key = key;
    hPair.count = 1;
    frequencyVector.add(hPair);
}
   
void insertToFrequencyWithDates(MyVector<heapPair>& frequencyVector,char* key) {
    int found = -1;
    for (int i=0; i<frequencyVector.size(); i++) {    
        //std::cout << "Before compare "<< frequencyVector[i].key << " " << frequencyVector[i].count << std::endl;
        if(strcmp(frequencyVector[i].key,key) == 0) {
            found = i;
            break;
        }
    }

    heapPair hPair;
    hPair.key = key;
    hPair.count = 1;

    if(found == -1) {
        frequencyVector.add(hPair);
    }
    else {
        frequencyVector[found].count++;
    }

}

   
void topKCases(Hash& hashTable,bool flag) {
   char *argument; 
   char* earliest = NULL, *latest = NULL;
   MyVector<char*> arg; 
   while ((argument = strtok(NULL,REC_DELIM)) != NULL) {
        arg.add(argument);
    }

    if (arg.size() < 2 || arg.size() > 4)
    {
        std::cerr << "Wrong number of arguments" << std::endl;
        return;
    }  

    else if(arg.size() == 3)
    {
        std::cerr << "Entry date without exit date" << std::endl;
        return;
    }

    MyVector<heapPair> frequencyVector;
    int k = atoi(arg[0]);
    char* key = strdup(arg[1]);
    
    myList<Record*>* lst = hashTable.getBucketRecordIds(key);

    if(lst == NULL) {
        return;
    }

    Node<Record*> *virusRecord = lst->initList();
    while(virusRecord != NULL) {
        if (arg.size() == 2) {
            if (flag) 
                insertToFrequency(frequencyVector, virusRecord->data->diseaseID);
            else
                insertToFrequency(frequencyVector, virusRecord->data->country);
        
        }
        else {
            earliest = arg[2];
            latest = arg[3];
            if (compareDates(earliest,virusRecord->data->entryDate) <= 0 && compareDates(virusRecord->data->entryDate,latest) <= 0) {
                if (flag) 
                    insertToFrequencyWithDates(frequencyVector, virusRecord->data->diseaseID);
                else
                    insertToFrequencyWithDates(frequencyVector, virusRecord->data->country);
            }
        }
        virusRecord = virusRecord->next;
    }

    PriorityQueue pq;

    for(int i=0; i<frequencyVector.size(); i++) {
        pq.push(frequencyVector[i]);
    }

    while(k>0) {
        if (pq.size() == 0)
            return;
        heapPair hPair = pq.top();
        pq.pop();
        std::cout << hPair.key << " "  << hPair.count << std::endl;
        k--;
    }
}


