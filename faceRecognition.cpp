#include "faceRecognition.h"
#include <UpdateInfo.h>
#include <iostream>
#include <QPushButton>
#include <QThread>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QEventLoop>
#include <QString>
#include <QJsonArray>
#include <QTableView>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QFrame>
#include <QGroupBox>
#include <QFileDialog>
#include <QTextEdit>
#include <database.h>
#include "takeimage.h"
#include "database.h"
using namespace std;
static int iter = 0;
faceRecognition::faceRecognition(QWidget *parent): QWidget(parent)
{
    this->setMinimumSize(1500,500);
//    this->setFixedSize(900,500);
    if (iter == 1) {

        model->clear();
        createMembers();
        setMembers();
        setupLayouts();
        connection();
    }
    iter++;
    //   mPersonDataStruct->deserialize(mJsonpath);
    //    PersonDataStruct *pobj =  new PersonDataStruct;
}

string ID;
std::vector<PersonInfo> p;

faceRecognition::~faceRecognition()
{

}
void faceRecognition::update_table(){
    DataBase db1;
    this->repaint();

    std::vector<PersonInfo> p1 = db1.SelectPersonInfo();
    cout << p1.size();
    for (int i = 0; i < p1.size(); i++){
        verticalHeader.append(QString::fromUtf8(to_string(i + 1).c_str()));
         item = new QStandardItem(QString::fromUtf8(to_string(p1[i].id).c_str()));
         model->setItem(i, 0, item);

         item = new QStandardItem(QString::fromUtf8(p1[i].name.c_str()));
         model->setItem(i, 1, item);

         item = new QStandardItem(QString::fromUtf8(p1[i].lastName.c_str()));
         model->setItem(i, 2, item);

         item = new QStandardItem(QString::fromUtf8(p1[i].officeName.c_str()));
         model->setItem(i, 3, item);

         item = new QStandardItem(QString::fromUtf8(p1[i].position.c_str()));
         model->setItem(i, 4, item);
    }
}
void faceRecognition::createMembers()
{
    m_addPerson = new QPushButton;
    m_setFirstName = new QLineEdit;
    m_setLastName = new QLineEdit;
    m_setOfficeName = new QLineEdit;
    m_setJob = new QLineEdit;

    m_name_label = new QLabel;
    m_last_name_label = new QLabel;
    m_office_name_label = new QLabel;
    m_job_label = new QLabel;

    m_id = new QLineEdit;

    result = new QTableView;

    m_delete = new QPushButton;
    VLayDel = new QVBoxLayout;
    DelGroupBox = new QGroupBox;
    item = new QStandardItem;
    m_update = new QPushButton;
//    horizontalHeader = new QStringList;
//    verticalHeader = new QStringList;
    //    mPersonDataStruct = new Utility::PersonDataStruct;
    //    QDir dir = QDir::current();
    //    mJsonpath = dir.absolutePath() + "/" + Utility::jsonPathForPersonDate;
    //    m_upload_label = new QLabel;
    //    m_upload_text_edit = new QTextEdit;
    //    m_open_folder = new QPushButton;
    VLay  = new QVBoxLayout;
    HLay  = new QHBoxLayout;
    MainHorizontalLay = new QHBoxLayout;
    VerticalBox = new QGroupBox;
    //    HorBox = new QGroupBox;


}


void faceRecognition::setMembers()
{
    m_name_label->setText("Name:");
    m_name_label->setAlignment(Qt::AlignCenter);

    m_last_name_label->setText("Last Name:");
    m_last_name_label->setAlignment(Qt::AlignCenter);

    m_office_name_label->setText("Ofice Name:");
    m_office_name_label->setAlignment(Qt::AlignCenter);

    m_job_label->setText("Job:");
    m_job_label->setAlignment(Qt::AlignCenter);


    m_setFirstName->setMaxLength(20);
    m_setFirstName->setFixedSize(200,30);

    m_setLastName->setMaxLength(30);
    m_setLastName->setFixedSize(200,30);

    m_setOfficeName->setMaxLength(50);
    m_setOfficeName->setFixedSize(200,30);

    m_setJob->setMaxLength(20);
    m_setJob->setFixedSize(200,30);


    m_delete->setText("Delete");
    m_delete->setFixedSize(200,30);

    m_update->setText("Update");
    m_update->setFixedSize(200,30);

    m_id->setFixedSize(200,30);
    m_id->setPlaceholderText("Enter ID");

    DataBase db;
    if(p.size() > 0){
        p.clear();
    }
    p = db.SelectPersonInfo();
    model->clear();
    horizontalHeader.append("ID");
    horizontalHeader.append("Name");
    horizontalHeader.append("Last Name");
    horizontalHeader.append("Office");
    horizontalHeader.append("Position");

    cout << p.size() << endl;
    model->clear();
    for (int i = 0; i < p.size(); i++){
        verticalHeader.append(QString::fromUtf8(to_string(i + 1).c_str()));
         item = new QStandardItem(QString::fromUtf8(to_string(p[i].id).c_str()));
         model->setItem(i, 0, item);

         item = new QStandardItem(QString::fromUtf8(p[i].name.c_str()));
         model->setItem(i, 1, item);

         item = new QStandardItem(QString::fromUtf8(p[i].lastName.c_str()));
         model->setItem(i, 2, item);

         item = new QStandardItem(QString::fromUtf8(p[i].officeName.c_str()));
         model->setItem(i, 3, item);

         item = new QStandardItem(QString::fromUtf8(p[i].position.c_str()));
         model->setItem(i, 4, item);
    }
    p.clear();
    model->setHorizontalHeaderLabels(horizontalHeader);
    model->setVerticalHeaderLabels(verticalHeader);

    result->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    result->resizeRowsToContents();

    VerticalBox->setFixedSize(500,500);
    result->setFixedSize(690,500);
    result->resizeColumnsToContents();

    m_addPerson->setText("Add");
    m_addPerson->setFixedSize(200,40);
    result->setModel(model);

    DelGroupBox->setFixedSize(300,300);
}

void faceRecognition::setupLayouts()
{

    VLay->addWidget(m_name_label);
    VLay->addWidget(m_setFirstName,0,Qt::AlignHCenter);

    VLay->addWidget(m_last_name_label);
    VLay->addWidget(m_setLastName,0,Qt::AlignHCenter);

    VLay->addWidget(m_office_name_label);
    VLay->addWidget(m_setOfficeName,0,Qt::AlignHCenter);

    VLay->addWidget(m_job_label);
    VLay->addWidget(m_setJob,0,Qt::AlignHCenter);
    VLay->addWidget(m_addPerson,0,Qt::AlignHCenter);

    VLayDel->addWidget(m_id);

    VLayDel->addWidget(m_delete);

    VLayDel->addWidget(m_update);

    //    HLay->addWidget(m_upload_label);
    //    HLay->addWidget(m_upload_text_edit);
    //    HLay->addWidget(m_open_folder);

    VerticalBox->setLayout(VLay);
    //    HorBox->setLayout(HLay);

    DelGroupBox->setLayout(VLayDel);

    MainHorizontalLay->addWidget(DelGroupBox);
    MainHorizontalLay->addWidget(result);
    MainHorizontalLay->addWidget(VerticalBox);
    //    MainVerticalLay->addWidget(HorBox);
    setLayout(MainHorizontalLay);

}

void faceRecognition::connection()
{
    connect(m_addPerson,SIGNAL(clicked()),this,SLOT(onAddButtonClicked()));
    connect(m_delete,SIGNAL(clicked()),this,SLOT(onDeleteButtonClicked()));
    connect(m_update,SIGNAL(clicked()),this,SLOT(onUpdateButtonClicked()));

    //    connect(m_open_folder,SIGNAL(clicked()),this,SLOT(onOpenButtonClicked()));
}

void faceRecognition::onAddButtonClicked()
{
    int ret;
    if(!m_setFirstName->text().trimmed().isEmpty() && !m_setLastName->text().trimmed().isEmpty() && !m_setOfficeName->text().trimmed().isEmpty()
            && !m_setJob->text().trimmed().isEmpty())
    {
        DataBase db;
        TakeImage photos;
        std::vector<string> stringImagePath;
        string name = m_setFirstName->text().trimmed().toStdString();
        string lastName = m_setLastName->text().trimmed().toStdString();
        string officeName = m_setOfficeName->text().trimmed().toStdString();
        string job = m_setJob->text().trimmed().toStdString();
        Info person;
        person = db.SelectInfo(name);

        if(person.name == name && person.lastName == lastName && person.office == officeName && person.position == job)
        {
            QMessageBox *warning = new QMessageBox;

            warning->setText("This information is existing in database!");
            warning->setInformativeText("Do you want update your information");
            warning->setIcon(QMessageBox::Information);
            warning->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            warning->setDefaultButton(QMessageBox::Cancel);
            ret = warning->exec();
        }
        else
        {
            //stringImagePath = photos.takeImageFromVideo("/home/anahit/Desktop/testImages",name);
            this->repaint();
            db.InsertData(name,lastName,officeName,job,stringImagePath);
            //update_table();
        }

    }
    else
    {
        m_msg_box.setText("Fill in all the fields");
        m_msg_box.setIcon(QMessageBox::Warning);
        m_msg_box.setDefaultButton(QMessageBox::Ok);
        m_msg_box.exec();
    }



}

string faceRecognition::take_id(){

    return ID;

}

void faceRecognition::onUpdateButtonClicked(){
    this->repaint();
    ID = m_id->text().toStdString();
    UpdateInfo update;
    update.show();
    QEventLoop l;
    l.exec();
}
void faceRecognition::onDeleteButtonClicked(){
    string str = m_id->text().toStdString();
    DataBase db;
    this->repaint();
    db.DeleteById(str);
    //update_table();
}
