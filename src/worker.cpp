#include "../inc/worker.h"
#include "../inc/map.h"
#include "../inc/functs.h"
#include "../inc/workersDirList.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h> 
using namespace std;

ofstream ofs;
Map ** mapArray;

Worker::Worker(int e2wfd, int w2efd, int workerNum) : e2wfd(e2wfd), w2efd(w2efd) {
    totalFiles = 0;
    successfulSearches=0;
    struct stat st = {0};                   //https://stackoverflow.com/questions/7430248/creating-a-new-directory-in-c

    char* charWorkerNum;
    charWorkerNum = (char*) malloc(sizeof workerNum + 1);
    sprintf(charWorkerNum, "%d", workerNum);

    char charPid[100];
    sprintf(charPid, "%d", getpid());

    logDir = new char [strlen(" log/worker/ .txt  ") + strlen(charWorkerNum) + strlen(charPid) + 1]();

    if (stat("log", &st) == -1) {
        mkdir("log", 0700);
    }

    strcat(logDir, "log/");
    strcat(logDir, "worker");


    strcat(logDir, charWorkerNum);

    if (stat(logDir, &st) == -1) {
        mkdir(logDir, 0700);
    }

    strcat(logDir, "/");

    //    char pid[1024];
    //    sprintf(pid, "%d", getpid());

    strcat(logDir, charPid);
    strcat(logDir, ".txt");

    ofstream ofs(logDir);
    free(charWorkerNum);
    //  ofs << logDir << endl;
}

Worker::~Worker() {
    close(e2wfd);
    close(w2efd);

    if (ofs.is_open()) {
        ofs.close();
    }

    for (int i = 0; i < totalFiles; i++) {
        delete mapArray[i];
    }
    delete[] mapArray;

    delete[] logDir;
    // delete trie;
}

bool Worker::receivePaths() {           //lamvanei ta paths
    char * payload = receiveData(e2wfd);
   // cout << "Worker " << getpid() << " received payload " << payload << endl;
    char* str = new char [strlen(payload) + 1];
    strcpy(str, payload);

    char * pch;
    pch = strtok(str, " \n");

    totalFiles += countFiles(pch);         //metrisi gia to posa arxeia exei na analavei

    while (pch != NULL) {
        pch = strtok(NULL, " \n");
        if (pch != NULL) {
            totalFiles += countFiles(pch);
        }
    }

    if (totalFiles == 0) {
        cout << "Wrong directory or empty" << endl;
        delete[] str;
        return false;
    }

    //dimioyrgia enos pinaka apo maps
    mapArray = new Map*[totalFiles]; //!!!! //https://stackoverflow.com/questions/20303820/creation-of-dynamic-array-of-dynamic-objects-in-c
    

    int i = 0;
    char* fil;
    fil = strtok(payload, " \n");
    i += importFilesToMaps(fil, i);

    while (i < totalFiles && fil != NULL) {
        fil = strtok(NULL, " \n");
        if (fil != NULL) {
            i = importFilesToMaps(fil, i);          //dimioyrgia twn maps, ena map gia kathe arxeio
        }
    }
    delete [] payload;
    delete [] str;

    return true;
}

int Worker::importFilesToMaps(char* pch, int i) {
    DIR *dir_ptr; //mema pdf
    struct dirent *direntp;
    if ((dir_ptr = opendir(pch)) == NULL)
        fprintf(stderr, "cannot open %s \n", pch);

    else {
        while ((direntp = readdir(dir_ptr)) != NULL) {
            //printf("%s\n", direntp->d_name);
            if (strcmp(direntp->d_name, ".") != 0 && strcmp(direntp->d_name, "..") != 0) {
                char *path = new char [strlen(direntp->d_name) + strlen(pch) + 2]();
                strcat(path, pch);
                strcat(path, "/");
                strcat(path, direntp->d_name);
                mapArray[i] = new Map(path);
                i++;
                delete [] path;
            }
        }
        closedir(dir_ptr);
    }
    return i;
}

int Worker::countFiles(char* pch) {        
    int counter = 0;
    DIR *dir_ptr; 
    struct dirent *direntp;
    if ((dir_ptr = opendir(pch)) == NULL)
        fprintf(stderr, "cannot open %s \n", pch);

    else {
        while ((direntp = readdir(dir_ptr)) != NULL) {
            if (strcmp(direntp->d_name, ".") != 0 && strcmp(direntp->d_name, "..") != 0) {
                counter++;
            }
        }
        closedir(dir_ptr);
    }
    return counter;
}



void Worker::start() {
    cout << "worker " << getpid() << " starts " << endl;

    while (1) {
        char * payload = receiveData(e2wfd);

        char* str = new char [strlen(payload) + 1];
        strcpy(str, payload);

        char * pch;
        pch = strtok(str, " \n");
        if (!ofs.is_open()) {
            ofs.open(logDir, std::ofstream::out | std::ofstream::app);
        }

        ofs << payload << endl;


        delete [] str;
        delete [] payload;

        if (payload == NULL) {
            exit(0);
        }
    }

}


Map** Worker::getMapArray() {
    return mapArray;
}