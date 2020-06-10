#ifndef MAP_H
#define MAP_H

class Map {
public:
    Map( char * filename);
    ~Map();
    void print();
    bool readFile();
    int getLines();
    char** getLinesArray();    
    char* getFileName();


private:
    char **linesArray; 
    int lines;
    char* fileName;
    int countLines();
    
};
#endif /* MAP_H */