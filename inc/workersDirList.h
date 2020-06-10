#ifndef WORKERSDIRLIST_H
#define WORKERSDIRLIST_H
#include <unistd.h>

class workersDirList {
private:
    class FilePathNode {
    public:
        char* filePath;
        FilePathNode * next;
    };

public:
    workersDirList();
    ~workersDirList();     //  name,fd0  ---------------------------- name,fd1
    // named pipes              |exec|   ---------------------------- |worker|
    char* parToChildName; 
    int parToChildFD;     
    char* childToParName;
    int childToParFD;
    pid_t pid;
    int countries;
    int paths;
    int pathLength;
    void assign(char * filePath);
    char* merge();
    bool hasCompletedCmdCommunication;
        FilePathNode* WorkerNodeHeadPtr;
};

#endif