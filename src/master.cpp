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
#include "../inc/functs.h"
#include "../inc/map.h"
#include "../inc/worker.h"
#include "../inc/workersDirList.h"
#include <fcntl.h>
#include <errno.h>
using namespace std;

int main(int argc, char **argv)
{

    char givenArg;
    char *inputDir;
    string data,serverIP;
    int numWorkers, bufferSize, len,serverPort;
    while ((givenArg = getopt(argc, argv, "w:b:s:p:i:")) != -1)
        switch (givenArg)
        {
        //flags for arguments
        case 'w':
            numWorkers = atoi(optarg);
            break;
        case 'b':
            bufferSize= atoi(optarg);
            break;
        case 's':
            serverIP = (optarg);
            break;
        case 'p':
            serverPort = atoi(optarg);
            break;
        case 'i':
            inputDir = basename(optarg);
            break;
        default:
            break;
        }
//   if (argc != 10 ) {
//         std::cerr << "Please give correct number of arguments" << std::endl;
//         return 1;
//     }    
    int countries = getCountries();
    workersDirList** listOfWorkers = new workersDirList* [numWorkers]; 
    createWorkers(listOfWorkers, numWorkers);
    for (int i = 0; i < numWorkers; i++) {
        listOfWorkers[i] = new workersDirList();
    }
    //pinakas me listes , kathe lista gia enan worker   

    // JobExecutor* jobExecutor = new JobExecutor(fileName, W);
    initList(listOfWorkers, numWorkers);     
    // assignPaths(listOfWorkers, numWorkers);
    cout << countries << endl;    
}