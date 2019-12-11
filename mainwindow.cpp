#include "faceRecognition.h"

#include <iostream>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QString>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QFile>
#include <QDir>
#include <QMessageBox>
#include <QFrame>
#include <QGroupBox>
#include <QFileDialog>
#include <QTextEdit>
#include <database.h>
#include "takeimage.h"
#include "database.h"

faceRecognition::faceRecognition(QWidget *parent): QWidget(parent)
{
    this->setMinimumSize(500,500);
    this->setMaximumSize(500,1000);
    createMembers();
    setMembers();
    setupLayouts();
    connection();
//   mPersonDataStruct->deserialize(mJsonpath);
//    PersonDataStruct *pobj =  new PersonDataStruct;
}

faceRecognition::~faceRecognition()
{

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
//    mPersonDataStruct = new Utility::PersonDataStruct;
//    QDir dir = QDir::current();
//    mJsonpath = dir.absolutePath() + "/" + Utility::jsonPathForPersonDate;
//    m_upload_label = new QLabel;
//    m_upload_text_edit = new QTextEdit;
//    m_open_folder = new QPushButton;
    VLay  = new QVBoxLayout;
//    HLay  = new QHBoxLayout;
    MainVerticalLay = new QVBoxLayout;
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

//    m_upload_text_edit->setMinimumSize(400,100);
//    m_upload_text_edit->setReadOnly(true);

//    m_upload_label->setText("Upload image:");
//    m_open_folder->setText("Open");

    m_addPerson->setText("Add");
    m_addPerson->setFixedSize(200,40);
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

//    HLay->addWidget(m_upload_label);
//    HLay->addWidget(m_upload_text_edit);
//    HLay->addWidget(m_open_folder);

    VerticalBox->setLayout(VLay);
//    HorBox->setLayout(HLay);

    MainVerticalLay->addWidget(VerticalBox);
//    MainVerticalLay->addWidget(HorBox);
    MainVerticalLay->addWidget(m_addPerson,0,Qt::AlignHCenter);
    setLayout(MainVerticalLay);

}

void faceRecognition::connection()
{
    connect(m_addPerson,SIGNAL(clicked()),this,SLOT(onAddButtonClicked()));
//    connect(m_open_folder,SIGNAL(clicked()),this,SLOT(onOpenButtonClicked()));
}

void faceRecognition::onAddButtonClicked()
{

//    if(!m_setFirstName->text().trimmed().isEmpty() && !m_setLastName->text().trimmed().isEmpty() && !m_setOfficeName->text().trimmed().isEmpty()
//            && !m_setJob->text().trimmed().isEmpty())
//    {

//        mPersonDataStruct->firstName = m_setFirstName->text();
//        mPersonDataStruct->lastName = m_setLastName->text();
//        mPersonDataStruct->officeName = m_setOfficeName->text();
//        mPersonDataStruct->job = m_setJob->text();

//        mPersonDataStruct->serialize(mJsonpath);

//    }
//    else
//    {
//        m_msg_box.setText("Fill in all the fields");
//        m_msg_box.setIcon(QMessageBox::Warning);
//        m_msg_box.setDefaultButton(QMessageBox::Ok);
//        m_msg_box.exec();
//    }
//    if( m_upload_text_edit->toPlainText().size() != 0 )
//    {
//       QStringList imagepath =  m_upload_text_edit->toPlainText().trimmed().split(QRegExp("\n"), QString::SkipEmptyParts);
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
           db.SelectInfo(name);

           if(person.name == name && person.lastName == lastName && person.office == officeName && person.position == job)
           {
               QMessageBox *warning = new QMessageBox;

               warning->setText("You were registrated");
               warning->setIcon(QMessageBox::Warning);
               warning->setDefaultButton(QMessageBox::Ok);
               warning->exec();
           }
           else
           {
               stringImagePath = photos.takeImageFromVideo("/home/anahit/Desktop/testImages",name);
               db.InsertData(name,lastName,officeName,job,stringImagePath);
           }

        }
        else
        {
            m_msg_box.setText("Fill in all the fields");
            m_msg_box.setIcon(QMessageBox::Warning);
            m_msg_box.setDefaultButton(QMessageBox::Ok);
            m_msg_box.exec();
        }

//       foreach (QString str , imagepath) {
//          stringImagePath.push_back(str.toStdString());
//       }

//    }
            this->close();

}

//void faceRecognition::onOpenButtonClicked()
//{

//    QStringList filename = QFileDialog::getOpenFileNames(this,"Select image","/home","Image Files (*png, *jpg)");
//    for(int i = 0; i < filename.size(); i++)
//    {
//        m_upload_text_edit->append(filename[i]);
//    }

//}


