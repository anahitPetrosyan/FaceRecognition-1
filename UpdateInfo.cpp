#include <UpdateInfo.h>
#include <iostream>
#include <database.h>
#include <QPushButton>
#include <faceRecognition.h>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>
using namespace std;
UpdateInfo::UpdateInfo(QWidget *parent): QWidget(parent)
{
    this->setFixedSize(250,500);
    createMembers();
    setMembers();
    setupLayouts();
    connection();
}
UpdateInfo::~UpdateInfo(){}

PersonInfo update_info;


void UpdateInfo::createMembers(){
    m_setFirstName = new QLineEdit;
    m_setLastName = new QLineEdit;
    m_setOfficeName = new QLineEdit;
    m_setJob = new QLineEdit;

    m_name_label = new QLabel;
    m_last_name_label = new QLabel;
    m_office_name_label = new QLabel;
    m_job_label = new QLabel;

    m_update = new QPushButton;

    VLay = new QVBoxLayout;

    VerticalBox = new QGroupBox;
}
string ID1;
void UpdateInfo::setMembers(){

    m_name_label->setText("Name:");
    m_name_label->setAlignment(Qt::AlignCenter);

    m_last_name_label->setText("Last Name:");
    m_last_name_label->setAlignment(Qt::AlignCenter);

    m_office_name_label->setText("Ofice Name:");
    m_office_name_label->setAlignment(Qt::AlignCenter);

    m_job_label->setText("Job:");
    m_job_label->setAlignment(Qt::AlignCenter);

    ///////////////////////////////////////////////////////

    faceRecognition fc;
    ID1 = fc.take_id();
    DataBase mydb;
    update_info = mydb.SelectInfoByID(ID1);
//    cout << "name:" << update_info.name << " last name:" << update_info.lastName << " office:" << update_info.officeName << " position:" << update_info.position << endl;

    m_setFirstName->setMaxLength(20);
    m_setFirstName->setFixedSize(200,30);
    m_setFirstName->setText(QString::fromStdString(update_info.name));

    m_setLastName->setMaxLength(30);
    m_setLastName->setFixedSize(200,30);
    m_setLastName->setText(QString::fromStdString(update_info.lastName));

    m_setOfficeName->setMaxLength(50);
    m_setOfficeName->setFixedSize(200,30);
    m_setOfficeName->setText(QString::fromStdString(update_info.officeName));

    m_setJob->setMaxLength(20);
    m_setJob->setFixedSize(200,30);
    m_setJob->setText(QString::fromStdString(update_info.position));


    ////////////////////////////////////////////////////////

    m_update->setText("Update");
    m_update->setFixedSize(200,30);

}

void UpdateInfo::setupLayouts(){


    VLay->addWidget(m_name_label);
    VLay->addWidget(m_setFirstName,0,Qt::AlignHCenter);

    VLay->addWidget(m_last_name_label);
    VLay->addWidget(m_setLastName,0,Qt::AlignHCenter);

    VLay->addWidget(m_office_name_label);
    VLay->addWidget(m_setOfficeName,0,Qt::AlignHCenter);

    VLay->addWidget(m_job_label);
    VLay->addWidget(m_setJob,0,Qt::AlignHCenter);

    VLay->addWidget(m_update);

    setLayout(VLay);
}


void UpdateInfo::connection(){
    connect(m_update,SIGNAL(clicked()),this,SLOT(onUpdateButtonClicked()));
}
static faceRecognition *fc1;
void UpdateInfo::takeFaceRec(faceRecognition &fc){
    fc1 = &fc;
}
void UpdateInfo::onUpdateButtonClicked(){
    //string ID = fc.take_id();
    //m_setID->setText(ID.c_str());
//    string ID = "26";

    string name = m_setFirstName->text().toStdString(), last_name = m_setLastName->text().toStdString();
    string office = m_setOfficeName->text().toStdString(), job = m_setJob->text().toStdString();
    if(name.size() != 0 && last_name.size() != 0 && office.size() != 0 && job.size() != 0){
        DataBase mydb;
        fc1->repaint();

        mydb.UpdateById(ID1,name,last_name,office,job);
        fc1->update_table();
    }
    this->close();
}




