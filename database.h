#ifndef DATABASE_H
#define DATABASE_H
#pragma once
#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <QMessageBox>

using namespace std;


struct Info
{
    int id;
    string name,lastName,office,position;
    vector<string> imagesPath;
};

struct PersonInfo
{
    int id;
    string name,lastName,officeName,position;
};


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

static int selectCallback(void *Used, int argc, char **argv, char **azColName)
{
    Info *structInfo = (Info *)Used;
    int i;

    for(i = 0; i<argc; i++) {
        if(strcmp(azColName[i],"ID") == 0)
        {
            structInfo->id = atoi(argv[0]);
        }
        if(strcmp(azColName[i],"NAME") == 0)
        {
            structInfo->name = argv[1];
        }
        else if(strcmp(azColName[i],"LAST_NAME") == 0)
        {
            structInfo->lastName = argv[2];
        }
        else if(strcmp(azColName[i],"OFFICE_NAME") == 0)
        {
            structInfo->office = argv[3];
        }
        else if(strcmp(azColName[i],"POSITION") == 0)
        {
            structInfo->position = argv[4];
        }

    }
    return 0;
}

static int selectPersonInfoCallback(void *Used, int argc, char **argv, char **azColName);

static int selectImageCallback(void *Used, int argc, char **argv, char **azColName)
{
    Info *structInfo = (Info *)Used;
    int i;
    for(i = 0; i<argc; i++)
    {
        if(strcmp(azColName[i],"IMAGE_PATH") == 0)
        {
            structInfo->imagesPath.push_back(argv[0]);
        }
    }
    return 0;
}

class DataBase
{

public:
    void CreateTable();

    void UpdateById(string id,string name,string lastName,string office,string position);

    void CreateImageTable();

    void InsertData(string NameValue, string LastNameValue, string OfficeNameVlue, string PositionValue, vector<string> ImagePath);

    void DeleteById(string id);

    Info SelectInfo(string name);

    PersonInfo SelectInfoByID(string id);

    std::vector<Info> SelectVecInfo();

    vector<PersonInfo> SelectPersonInfo();
private:
    void OpenDb()
    {
        int rc;
        rc = sqlite3_open("C:\\Users\\Gevorg\\Desktop\\Gevorg\\programming\\FaceRecognitionProject\\dataBases\\Face_recognition.db", &db);
        if(rc)
        {
            cout << "Error opening database\n";
        }
    }

    sqlite3 *db;
};
#endif // DATABASE_H
