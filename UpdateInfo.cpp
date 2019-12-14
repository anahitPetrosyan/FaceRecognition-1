#include <UpdateInfo.h>
#include <iostream>
#include <database.h>
#include <QPushButton>
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

void UpdateInfo::createMembers(){
    m_setFirstName = new QLineEdit;
    m_setLastName = new QLineEdit;
    m_setOfficeName = new QLineEdit;
    m_setJob = new QLineEdit;
    m_setID = new QLineEdit;

    m_id_label = new QLabel;
    m_name_label = new QLabel;
    m_last_name_label = new QLabel;
    m_office_name_label = new QLabel;
    m_job_label = new QLabel;

    m_update = new QPushButton;

    VLay = new QVBoxLayout;

    VerticalBox = new QGroupBox;
}

void UpdateInfo::setMembers(){
    m_id_label->setText("ID");
    m_id_label->setAlignment(Qt::AlignCenter);

    m_name_label->setText("Name:");
    m_name_label->setAlignment(Qt::AlignCenter);

    m_last_name_label->setText("Last Name:");
    m_last_name_label->setAlignment(Qt::AlignCenter);

    m_office_name_label->setText("Ofice Name:");
    m_office_name_label->setAlignment(Qt::AlignCenter);

    m_job_label->setText("Job:");
    m_job_label->setAlignment(Qt::AlignCenter);

    ///////////////////////////////////////////////////////
    m_setID->setMaxLength(10);
    m_setID->setFixedSize(200,30);

    m_setFirstName->setMaxLength(20);
    m_setFirstName->setFixedSize(200,30);

    m_setLastName->setMaxLength(30);
    m_setLastName->setFixedSize(200,30);

    m_setOfficeName->setMaxLength(50);
    m_setOfficeName->setFixedSize(200,30);

    m_setJob->setMaxLength(20);
    m_setJob->setFixedSize(200,30);

    ////////////////////////////////////////////////////////

    m_update->setText("Update");
    m_update->setFixedSize(200,30);

}

void UpdateInfo::setupLayouts(){

    VLay->addWidget(m_id_label);
    VLay->addWidget(m_setID,0,Qt::AlignHCenter);

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

void UpdateInfo::onUpdateButtonClicked(){
    string ID = m_setID->text().toStdString();
    string name = m_setFirstName->text().toStdString(), last_name = m_setLastName->text().toStdString();
    string office = m_setOfficeName->text().toStdString(), job = m_setJob->text().toStdString();
    if(name.size() != 0 && last_name.size() != 0 && office.size() != 0 && job.size() != 0){
        DataBase mydb;
        mydb.UpdateById(ID,name,last_name,office,job);
    }
    this->close();
}




