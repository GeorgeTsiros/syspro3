#include <sys/wait.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/poll.h>
#include "../inc/workersDirList.h"
#include "../inc/functs.h"
#include "../inc/map.h"
#include "../inc/worker.h"

//named pipe functions
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

void createWorkers(workersDirList** listOfWorkers, int numWorkers) { //dimiourgia twn worker 
    // fd_set
    for (int i = 0; i < numWorkers; i++) {
        pid_t p = fork();
        if (p == -1) {
            perror("fork failed");
            exit(1);
        } else if (p > 0) {
            listOfWorkers[i]->pid = p;
        } else if (p == 0) { // p == 0 (child process)
            std::cout << "Worker " << getpid() << std::endl;

            int e2wfd = open(listOfWorkers[i]->parToChildName, O_WRONLY);
            int w2efd = open(listOfWorkers[i]->childToParName, O_WRONLY);

            Worker* worker = new Worker(e2wfd, w2efd, i);
            if (!worker->receivePaths()) {
                delete worker;
                exit(1);
            }
            if (!worker->importData()) {
                delete worker;
                exit(1);
            }
            worker->start();
            delete worker;
            exit(1);
        }
    }

    for (int i = 0; i < numWorkers; i++) {
        listOfWorkers[i]->parToChildFD = open(listOfWorkers[i]->parToChildName, O_WRONLY);
        std::cout << "Worker " << i << " works with " << listOfWorkers[i]->parToChildName << " pipe " << std::endl;
    }
    for (int i = 0; i < numWorkers; i++) {
        listOfWorkers[i]->childToParFD = open(listOfWorkers[i]->childToParName, O_RDONLY);
        std::cout << "Worker " << i << " works with " << listOfWorkers[i]->childToParName << " << pipe " << std::endl;
    }
    std::cout << "All workers have work to do    " << std::endl;
}