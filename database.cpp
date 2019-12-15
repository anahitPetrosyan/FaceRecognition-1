#include <database.h>


std::vector<PersonInfo> pvec;

static int selectPersonInfoCallback(void *Used, int argc, char **argv, char **azColName)
{
    PersonInfo *personInfo = (PersonInfo*)Used;
    int i;
    for( i = 0; i < argc; i++)
    {
        if(!strcmp(azColName[i],"ID"))
        {
            personInfo->id = atoi(argv[0]);
        }
        else if(!strcmp(azColName[i],"NAME"))
        {
            personInfo->name = argv[1];
        }
        else if(!strcmp(azColName[i],"LAST_NAME"))
        {
            personInfo->lastName = argv[2];
        }
        else if(!strcmp(azColName[i],"OFFICE_NAME"))
        {
            personInfo->officeName = argv[3];
        }
        else
        {
            personInfo->position = argv[4];
        }
        //        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    pvec.push_back(*personInfo);
    return 0;

}


sqlite3 *db;

void DataBase::DeleteById(string id){

    int rc,rc2;
    int isInserted;
    OpenDb();
    string sql,sql2;
    string insertImagePath;
    char *zErrMsg = 0;

    sql = "DELETE FROM PERSONDATA WHERE ID="+(id)+";";
    sql2 = "DELETE FROM IMAGES WHERE PERSON_ID="+(id)+";";
    rc = sqlite3_exec(db,sql.c_str(),callback,0,&zErrMsg);
    rc2 = sqlite3_exec(db,sql2.c_str(),callback,0,&zErrMsg);
    if (rc != SQLITE_OK || rc2 != SQLITE_OK) {
        cerr << "Error with sql\n";
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

void DataBase::CreateTable()
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

void DataBase::CreateImageTable()
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

void DataBase::UpdateById(string id,string name,string lastName,string office,string position){
    int rc;
    OpenDb();
    string sql;
    string insertImagePath;
    char *zErrMsg = 0;
    sql = "UPDATE PERSONDATA SET NAME = '"+name+"', LAST_NAME = '"+lastName+"', OFFICE_NAME = '"+office+"', POSITION = '"+position+"' WHERE ID="+(id)+";";
    rc = sqlite3_exec(db,sql.c_str(),callback,(void*)"d\n",&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Update error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    return;
}

void DataBase::InsertData(string NameValue, string LastNameValue, string OfficeNameVlue, string PositionValue, vector<string> ImagePath)
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
        fprintf(stderr, "SQL Insert error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    else
    {
        cout << "Records created Successfully\n";
    }
    sqlite3_close(db);
}

Info DataBase::SelectInfo(string name)
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
        fprintf(stderr, "SQL Select info error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    return info;
}

PersonInfo DataBase::SelectInfoByID(string id){
    OpenDb();
    string sql;
    int worked;
    char *zErrMsg = 0;
    PersonInfo info;
    sql = "SELECT * FROM PERSONDATA WHERE ID = '"+id+"';";
    worked = sqlite3_exec(db,sql.c_str(),selectCallback,&info,&zErrMsg);

    if(worked != SQLITE_OK)
    {
        fprintf(stderr, "SQL select info by id error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    return info;
}

std::vector<Info> DataBase::SelectVecInfo(){
    OpenDb();
    string sql = "select * from PERSONDATA";
    char *zErrMsg = 0;
    Info info;
    int worked = sqlite3_exec(db,sql.c_str(),selectCallback,&info,&zErrMsg);
}

vector<PersonInfo> DataBase::SelectPersonInfo()    {
    OpenDb();
    string sql2;
    int worked;
    char *zErrMsg = 0;
    PersonInfo personInfo;
    //    sql = "SELECT COUNT(ID) FROM PERSONDATA";
    //    worked2 = sqlite3_exec(db,sql.c_str(),selectCountCallback,&count,&zErrMsg);
    pvec.clear();
    sql2 = "SELECT * FROM PERSONDATA";
    worked = sqlite3_exec(db,sql2.c_str(),selectPersonInfoCallback,&personInfo,&zErrMsg);

    if(worked != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    return pvec;
}
