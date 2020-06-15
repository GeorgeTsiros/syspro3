#include <dirent.h> 
#include <stdio.h> 
#include <cstring>
#include <iostream>
#include "../inc/functs.h"
#include "../inc/map.h"
#include "../inc/worker.h"
#include "../inc/workersDirList.h"
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int getCountries() {
  DIR *d;
  struct dirent *dir;
  int countries = 0;
  d = opendir("/mnt/c/Users/User/Documents/syspro3/dataset");
  if (d) {
    while ((dir = readdir(d)) != NULL ) {
      if(strcmp(dir->d_name, ".")==0 || strcmp(dir->d_name, "..")==0)
                continue;
      countries++;
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  // printf("%d\n", countries);
  return(countries);
  
}
// 5 χωρες - 3 workers: 5/3 = 2, το υπολοιπο ο λαστ

void assignCountriesToWorkers(int numWorkers, int numCountries){
  if (numWorkers > numCountries)
    {
      //mia ston kathena
    }

  int countriesPerWorker = numCountries / numWorkers;
  int lastWorkerCountries = numCountries % numWorkers;
  DIR *d;
  struct dirent *dir;
  int currentWorkerCountries = 0;
  int currentWorker = 0;
  while ((dir = readdir(d)) != NULL ) { 
    currentWorkerCountries++;
    //assign dir->d_name to this worker
    if (currentWorkerCountries == countriesPerWorker) {
      currentWorkerCountries = 0;
      currentWorker++;
    }
  
  }
  
}

void sendData(int fd, char * data) {
    int header = strlen(data);
    int header_length = sizeof (header);

    char * payload = data;
    int payload_length = strlen(data);
}
char* receiveData(int fd) {
    int header;                     
    int header_length = sizeof (header);                

    char* payload;
    int payload_length;

    if (read(fd, (void*) &(header), header_length) <= 0) {
        return NULL;
    }

    payload_length = header;

    payload = new char [header + 1]();
    if (read(fd, (void*) payload, payload_length) <= 0) {
        return NULL;
    }
    return payload;
}

void initList(workersDirList** listOfWorkers, int numWorkers) {
    int x;
    Map* map; 
    for (int i = 0; i < numWorkers; i++) {
        //dimioyrgia named pipes
       
        listOfWorkers[i]->parToChildName = new char[strlen("exec_to_worker") + 10];
        listOfWorkers[i]->childToParName = new char[strlen("worker_to_exec") + 10];

        sprintf(listOfWorkers[i]->parToChildName, "exec_to_worker_%d.fifo", i);
        sprintf(listOfWorkers[i]->childToParName, "worker_to_exec_%d.fifo", i);

        char * buf = listOfWorkers[i]->parToChildName;

        x = mkfifo(buf, 0666);
        if (x == -1 && errno != EEXIST) {
            perror("mkfifo failed");
            exit(-1);
        }
        if (x == -1 && errno == EEXIST) {
            unlink(buf);
            x = mkfifo(buf, 0666);
        }

        buf = listOfWorkers[i]->childToParName;
        x = mkfifo(buf, 0666);

        if (x == -1 && errno != EEXIST) {
            perror("mkfifo failed");
            exit(-1);
        }
        if (x == -1 && errno == EEXIST) {
            unlink(buf);
            x = mkfifo(buf, 0666);
        }
    }
    for (int i = 0; i < map->getLines(); i++) { 
        //kathe country (dld kathe grammi toy country file) anatithetai tuxaia (i % numWorkers) se kapoion worker
        listOfWorkers[i % numWorkers]->assign(map->getLinesArray()[i]);
    }
}

void assignPaths(workersDirList** listOfWorkers, int numWorkers) {

    for (int i = 0; i < numWorkers; i++) {
        char * payload = listOfWorkers[i]->merge();
        sendData(listOfWorkers[i]->parToChildFD, payload);
        delete [] payload;
    }
}

void createWorkers(workersDirList** listOfWorkers, int numWorkers) { //dimiourgia twn worker 
    for (int i = 0; i < numWorkers; i++) {
        pid_t p = fork();
        if (p == -1) {
            perror("fork failed");
            exit(1);
        } else if (p > 0) {
            listOfWorkers[i]->pid = p;
        } else if (p == 0) { // p == 0 (child process)
             std::cout << "Worker " << getpid() << std::endl;

            int e2wfd = open(listOfWorkers[i]->parToChildName, O_RDONLY);
            int w2efd = open(listOfWorkers[i]->childToParName, O_WRONLY);

            Worker* worker = new Worker(e2wfd, w2efd, i);
            // if (!worker->receivePaths()) {
            //     delete worker;
            //     exit(1);
            // }
            // if (!worker->importData()) {
            //     delete worker;
            //     exit(1);
            // }
            worker->start();        //seg sto start tou last worker
            delete worker;
            exit(1);
        }
    }

     std::cout << "O Executor perimenei tous workers na suxronistoun " << std::endl;
    for (int i = 0; i < numWorkers; i++) {
        listOfWorkers[i]->parToChildFD = open(listOfWorkers[i]->parToChildName, O_WRONLY);
     std::cout << "Worker " << i << " synched with " << listOfWorkers[i]->parToChildName << " pipe " << std::endl;
    }
    for (int i = 0; i < numWorkers; i++) {
        listOfWorkers[i]->childToParFD = open(listOfWorkers[i]->childToParName, O_RDONLY);
         std::cout << "Worker " << i << " synched with " << listOfWorkers[i]->childToParName << " << pipe " << std::endl;
    }
    // std::cout << "O Executor exei suxronistei me olous tous workers " << std::endl;
}