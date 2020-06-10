#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include "../src/workersDirList.cpp"
#include "../inc/functs.h"

#define REC_DELIM " "
using namespace std;

int main(int argc, char **argv)
{
    char givenArg;
    char *inputDir;
    string data;
    int numWorkers, bufferSize, len;
    sprintf("thisText", "./fifo1");
    while ((givenArg = getopt(argc, argv, "w:b:i:")) != -1)
        switch (givenArg)
        {
        //flags for arguments
        case 'w':
            numWorkers = atoi(optarg);
            break;
        case 'b':
            bufferSize= atoi(optarg);
            break;
        case 'i':
            inputDir = basename(optarg);
            break;
        default:
            break;
        }
  if (argc != 7 ) {
        std::cerr << "Please give correct number of arguments" << std::endl;
        return 1;
    }
    
    
    workersDirList** listOfWorkers = new workersDirList* [numWorkers]; 
    for (int i = 0; i < numWorkers; i++) {
        listOfWorkers[i] = new workersDirList();
    }
  
  string countriesArr[30];
  ifstream fp("countriesFile.txt");
  int numCountries= 0;
  if(!fp) 
  {
    cout<<"Error opening output file"<<endl;
    system("pause");
    return -1;
  }
  while(!fp.eof())
  {
    getline(fp,countriesArr[numCountries]);
    cout << countriesArr[numCountries]<< endl;
   numCountries++;
  }
  int countriesPerWorker = numCountries / numWorkers;
    // int lastWorkerCountries = numCountries % numWorkers;

    for (int j = 0; j< numCountries; j++) {
        std::cout << "COUNTRY ARRAYY " << countriesArr[j];
    }
    int start = 0;
    int end = countriesPerWorker;
    while(end < numCountries) {
        // assignToWorker(countries, start, end));
        start+=countriesPerWorker;
        end+=countriesPerWorker;
    }

    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 

    if (pipe(fd1) < 0)
        {
            perror("Failed to allocate first pipe");
            exit(EXIT_FAILURE);
        }

    if (pipe(fd2) < 0)
        {
            perror("Failed to allocate second pipe");
            exit(EXIT_FAILURE);
        }

for (int i = 1; i < numWorkers; i++) {
        // std::cout << "IN FOR " <<  std::endl;
        pid_t p = fork();
        if (p == -1) {
            perror("fork failed");
            exit(1);
        } else if (p > 0) {
            // std::cout << "IN PARENT" <<  std::endl;
            close(fd2[0]);
            close(fd2[1]);
            data = countriesArr[0];
            if (write(fd1[1], &data, sizeof(data)) != sizeof(data))
            {
                perror("Parent: Failed to send data to child ");
                exit(EXIT_FAILURE);
            }

            listOfWorkers[i]->pid = p;
        } else if (p == 0) { // p == 0 (child process)
            close(fd1[0]);
            close(fd1[1]);
            len = read(fd2[0], &data, sizeof(data));
            if (len < 0)
        {
            perror("Child: Failed to read data from pipe");
            exit(EXIT_FAILURE);
        }
        else if (len == 0)
        {
            // not an error, but certainly unexpected
            fprintf(stderr, "Child: Read EOF from pipe");
        }
        else
        {
            // std::cout << "IN CHILD" <<  std::endl;
            std::cout << "Worker " << getpid() << "received" << data << std::endl;
            close(fd2[0]);
            close(fd2[1]);
            return EXIT_SUCCESS;
        }
    }
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

            if(strcmp(token,"/listCountries") == 0) {
                // globalDiseaseStats(diseaseHashTable,viruses);
            }
            else if(strcmp(token,"/diseaseFrequency") == 0) {
                // diseaseFrequency(diseaseHashTable);

            }
            else if(strcmp(token,"/topk-AgeRanges") == 0) {
                // topKCases(countryHashTable,true);         
                }
            else if(strcmp(token,"/topk-Countries") == 0) {
                // topKCases(diseaseHashTable,false);
            }
            else if(strcmp(token,"/searchPatientRecord") == 0) {
                // InsertPatientRecord(records,diseaseHashTable,countryHashTable);
            }
            else if(strcmp(token,"/numPatientAdmissions") == 0) {
                // recordPatientExit(records);    
            }
            else if(strcmp(token,"/numPatientDischarges") == 0) {
                // numCurrentPatients(diseaseHashTable,viruses);
            }
            else if(strcmp(token,"/exit") == 0) {
                printf("Exiting \n");
                if(strtok(NULL,REC_DELIM) != NULL) {
                    std::cerr << "Wrong format of command" <<std::endl;
                    continue;
                }
                //Free all the allocated memory
                free(command);
                // records.clear();
                return 0;
            }
            else {
                std::cerr << "There is no such command" << std::endl;
            }
        }
    }

    for(int i=0;i<numWorkers;i++) // loop will run n times (n=5) 
        wait(NULL); 

        return EXIT_SUCCESS;


}