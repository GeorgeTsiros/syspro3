#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../inc/record.h"

// about dates: https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm

Record::Record(char* recId, char* name,char* surname,char* diseaseID,char* country, char* entryDate, char* exitDate) {
    // recordId = (char*) malloc((strlen(surname)+1) * sizeof(char));
    // strcpy(recordId,surname);

    // senderId = (char*) malloc((strlen(name)+1) * sizeof(char));
    // strcpy(senderId,sendId);

    // receiverId = (char*) malloc((strlen(surname)+1) * sizeof(char));
    // strcpy(receiverId,surname);

    this->recordId = recId;
    this->patientFirstName = name;
    this->patientLastName = surname;
    this->diseaseID = diseaseID;
    this->country = country;
    this->entryDate = entryDate;
    this->exitDate = exitDate;
}

Record::~Record() {
    free(recordId);
    free(patientFirstName);
    free(patientLastName);
    free(diseaseID);
    free(country);
    free(entryDate);
    free(exitDate);

}

char* Record::getRecordId() {
    return recordId;
}


void Record::setNewExitDate(char* exit){
    free(exitDate);
    exitDate = exit;
}