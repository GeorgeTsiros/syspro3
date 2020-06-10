#include <iostream>
#include "../inc/map.h"
#include "../inc/functs.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>

using namespace std;

Map::Map( char * name) {
   fileName = new char [strlen(name) + 1]();
   strcpy(fileName, name);
}

int Map::countLines() {
    FILE * currentFile = NULL; 
    currentFile = fopen(fileName, "r");
    if (currentFile != NULL) {
        char * result = NULL;
        size_t len = 0;
        int currentLine;
        lines = 0;

        while ((currentLine = getline(&result, &len, currentFile)) != -1) {

            lines++;
            free(result);
            result = NULL;
        }
        if (result != NULL) {
            free(result);
        }
        fclose(currentFile);
    } else {
        cout <<fileName <<" Τhis file does not exist " << endl;
        return 0;
    }
    //cout << "Line counter " << lines << endl; //for debug 
    return lines;
}

bool Map::readFile() {
    if (countLines() == 0) { 
        return false;
    }
    //keep lines - countries in an Array
    linesArray = new char * [lines];  

    FILE * currentFile; 
    currentFile = fopen(fileName, "r");
    if (currentFile != NULL) {
        char* result = NULL;
        size_t len = 0;
        int currentLine, thisLine = 0;

        while ((currentLine = getline(&result, &len, currentFile)) != -1) { //diavasma tou arxeioy ana grammi
            linesArray[thisLine] = new char [len + 1];
            strcpy(linesArray[thisLine], result); //perasma kathe keimenou stin antistixi thesi sto pinaka 
            free(result);
            result = NULL;
            thisLine++;
        }
        if (result != NULL) {
            free(result);
        }
        fclose(currentFile);
    } else {
        cout << "cant open the file" << endl;
        return false;
    }
    return true;
}

Map::~Map() {
    if (linesArray != NULL) {
        for (int i = 0; i < lines; i++) {
            delete [] linesArray[i];
        }
        delete [] linesArray;
    }
    delete [] fileName;
}

void Map::print() {
        cout <<"File "<< fileName << endl;

    cout <<"Total lines "<< lines << endl;
    for (int i = 0; i < lines; i++) {
        cout <<"line "<< i << endl;
        cout << linesArray[i] << endl;
    }
}

char** Map::getLinesArray() {
    return linesArray;
}

int Map::getLines() {
    return lines;
}

 char* Map::getFileName() {
    return fileName;
}