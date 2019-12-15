#ifndef UPDATEINFO_H
#define UPDATEINFO_H

#pragma once
#include <QWidget>
#include <faceRecognition.h>
class QPushButton;
class QLineEdit;
class QLabel;
class QVBoxLayout;
class QGroupBox;

class UpdateInfo : public QWidget
{
    Q_OBJECT
public:
    UpdateInfo(QWidget *parent = 0);
    ~UpdateInfo();
    void takeFaceRec(faceRecognition &fc);
public slots:
    void onUpdateButtonClicked();
private:
    void createMembers();
    void setMembers();
    void setupLayouts();
    void connection();

    QLineEdit *m_setFirstName, *m_setID, *m_setLastName,  *m_setOfficeName, *m_setJob;
    QLabel *m_name_label, *m_id_label, *m_last_name_label, *m_office_name_label, *m_job_label;
    QPushButton *m_update;
    QVBoxLayout *VLay;
    QGroupBox *VerticalBox;
};




#endif // UPDATEINFO_H
