#ifndef DATABASE_H
#define DATABASE_H

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
    void CreateTable()
    {
        OpenDb();
        int rc;
        const char *sql;
        char *zErrMsg = 0;
        sql = "CREATE TABLE PERSONDATA(" \
              "ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
              "NAME CHAR(20) NOT NULL,"\
              "LAST_NAME CHAR(30) NOT NULL,"\
              "OFFICE_NAME CHAR(100) NOT NULL,"\
              "POSITION CHAR(100) NOT NULL"");";
        rc = sqlite3_exec(db,sql,callback,0,&zErrMsg);
        if(rc != SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else
        {
            cout << "Table created successfully\n";
        }
        sqlite3_close(db);
    }

    void CreateImageTable()
    {

        OpenDb();
        int rc;
        const char *sql;
        char *zErrMsg = 0;
        sql = "CREATE TABLE IMAGES(" \
              "ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
              "PERSON_ID INTEGER NOT NULL, "\
              "IMAGE_PATH CHAR(255) NOT NULL,"\
              "FOREIGN KEY(PERSON_ID) REFERENCES PERSONDATA(ID)"");";
        rc = sqlite3_exec(db,sql,callback,0,&zErrMsg);
        if(rc != SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else
        {
            cout << "Table created successfully\n";
        }
        sqlite3_close(db);
    }

    void InsertData(string NameValue, string LastNameValue, string OfficeNameVlue, string PositionValue, vector<string> ImagePath)
    {
        int rc;
        int isInserted;
        OpenDb();
        string sql;
        string insertImagePath;
        char *zErrMsg = 0;

            sql = "INSERT INTO PERSONDATA (NAME,LAST_NAME,OFFICE_NAME,POSITION)"\
                  "VALUES ('"+NameValue+"','"+LastNameValue+"','"+OfficeNameVlue+"','"+PositionValue+"');";
            rc = sqlite3_exec(db,sql.c_str(),callback,0, &zErrMsg);
            string  pid = to_string( sqlite3_last_insert_rowid(db));
            for(int i = 0; i < ImagePath.size(); i++)
            {
                insertImagePath = "INSERT INTO IMAGES(PERSON_ID,IMAGE_PATH) VALUES('"+pid+"','"+ImagePath.at(i)+"');";
                isInserted = sqlite3_exec(db,insertImagePath.c_str(),callback,0,&zErrMsg);
            }

            if(rc != SQLITE_OK || isInserted  != SQLITE_OK)
            {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

            else
            {
                cout << "Records created Successfully\n";
            }
            sqlite3_close(db);
        }

    Info SelectInfo(string name)
    {
        OpenDb();
        string sql,sql2;
        int worked,worked2;

        char *zErrMsg = 0;
        Info info;
        sql = "SELECT * FROM PERSONDATA WHERE NAME = '"+name+"'";
        worked = sqlite3_exec(db,sql.c_str(),selectCallback,&info,&zErrMsg);

        sql2 = "SELECT IMAGE_PATH FROM IMAGES WHERE PERSON_ID = "+to_string(info.id)+"";
        worked2 = sqlite3_exec(db,sql2.c_str(),selectImageCallback,&info,&zErrMsg);

        if(worked != SQLITE_OK && worked2 != SQLITE_OK)
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        sqlite3_close(db);
        return info;
    }

private:
    void OpenDb()
    {
        int rc;
        rc = sqlite3_open("/home/anahit/DB/FaceRecognition.db", &db);
        if(rc)
        {
            cout << "Error opening database\n";
        }
    }

    sqlite3 *db;
};
#endif // DATABASE_H
