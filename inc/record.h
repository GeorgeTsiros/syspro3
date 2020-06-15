#ifndef RECORD_H
#define RECORD_H



struct Record {
    char* recordId;
    char* patientFirstName;
    char* patientLastName;
    char* diseaseID;
    char* country;
    char* entryDate;
    char* exitDate;

    Record(char*,char*,char*,char*,char*,char*,char*);
    ~Record();
    char* getRecordId();
    void setNewExitDate(char*);
};

#endif