#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "../inc/record.h"
#include "../inc/hash.h"
#include "../inc/functs.h"
#include "../inc/myList.h"
#include "../inc/myvector.h"


int main(int argc, char **argv)
{

    char givenArg;
    size_t diseaseHashtableSize, countryHashtableSize, bucketSize;
    char *patientRecordsFl;
    
    
    while ((givenArg = getopt(argc, argv, "p:d:c:b:")) != -1)
        switch (givenArg)
        {
        //flags for arguments
        case 'p':
            patientRecordsFl = optarg;
            break;
        case 'd':
            diseaseHashtableSize = atol(optarg);
            break;
        case 'c':
            countryHashtableSize = atol(optarg);
            break;
        case 'b':
            bucketSize = atol(optarg);
            break;
        default:
            break;
        }

    //Condition for wrong arguments
    if (argv[optind] == NULL && optind < argc) {
        std::cerr << "Please give correct number of arguments" << std::endl;
        return 1;
    }
    FILE *patientRecordsFile;


    if((patientRecordsFile = fopen(patientRecordsFl,"r")) == NULL) {
        perror("Something wrong happened with the patient records file");
        return 1;
    }
    
    Hash diseaseHashTable(diseaseHashtableSize, bucketSize);
    Hash countryHashTable(countryHashtableSize, bucketSize);
    MyVector<Record*> records;
    MyVector<char*> viruses;

    char *recID,*firstName,*surname,*diseaseID,*country,*entryDate,*exitDate;
    
    while(fscanf(patientRecordsFile,"%ms %ms %ms %ms %ms %ms %ms",&recID,&firstName,&surname,&diseaseID,&country,&entryDate,&exitDate) != EOF) {
        //check for duplicate records

        if (!isValidRecordId(records,recID)) {
            std::cerr << "There is already a record with this id " << recID << std::endl;
            fclose(patientRecordsFile);
            records.clear();
            return 0;
        }

        if (strcmp(exitDate,"-") != 0) {
            if (compareDates(entryDate, exitDate) > 0) {
                std::cerr << "Exit date " << exitDate << " can not be earlier than entry date " << entryDate << std::endl;
                continue;
            }
        }

        if(!virusFound(viruses,diseaseID)) {
            viruses.add(diseaseID);
        }
                
        Record *rec = new Record(recID,firstName,surname,diseaseID,country,entryDate,exitDate);
        records.add(rec);

        diseaseHashTable.insertItem(rec->diseaseID,rec);
        countryHashTable.insertItem(rec->country,rec);
    }
    
    fclose(patientRecordsFile);

    char *command = NULL;
    size_t read; 
    size_t len = 0;


    while(true) {
        std::cout<< "Give a command: ";
        if((read = getline(&command, &len, stdin)) != -1) {
            char* whole_command = strtok(command,"\n");
            char* token = strtok(whole_command,REC_DELIM);

            if(token == NULL) {
                 std::cerr << "You didn't give any input" << std::endl;
                 continue;
            }

            if(strcmp(token,"/globalDiseaseStats") == 0) {
                globalDiseaseStats(diseaseHashTable,viruses);
            }
            else if(strcmp(token,"/diseaseFrequency") == 0) {
                diseaseFrequency(diseaseHashTable);

            }
            else if(strcmp(token,"/topk-Diseases") == 0) {
                topKCases(countryHashTable,true);         }
            else if(strcmp(token,"/topk-Countries") == 0) {
                topKCases(diseaseHashTable,false);
            }
            else if(strcmp(token,"/insertPatientRecord") == 0) {
                InsertPatientRecord(records,diseaseHashTable,countryHashTable);
            }
            else if(strcmp(token,"/recordPatientExit") == 0) {
                recordPatientExit(records);    
            }
            else if(strcmp(token,"/numCurrentPatients") == 0) {
                numCurrentPatients(diseaseHashTable,viruses);
            }
            else if(strcmp(token,"/exit") == 0) {
                printf("Exiting \n");
                if(strtok(NULL,REC_DELIM) != NULL) {
                    std::cerr << "Wrong format of command" <<std::endl;
                    continue;
                }
                //Free all the allocated memory
                free(command);
                records.clear();
                return 0;
            }
            else {
                std::cerr << "There is no such command" << std::endl;
            }
        }
    }
}
