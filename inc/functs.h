#ifndef FUNCTS_H
#define FUNCTS_H

class workersDirList;

void sendData(int fd, char * data);
char* receiveData(int fd);
int getCountries();
void initList(workersDirList** listOfWorkers, int numWorkers);
void assignPaths(workersDirList** listOfWorkers, int numWorkers);
void createWorkers(workersDirList** listOfWorkers, int numWorkers);
#endif