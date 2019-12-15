#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#pragma once
#include "QStandardItemModel"
#include "QStandardItem"
#include <QWidget>
#include <QMessageBox>
#include <string>
class QPushButton;
class QLineEdit;
class QTableView;
class QVBoxLayout;
class QLabel;
class QString;
class QFrame;
class QHBoxLayout;
class QGroupBox;
class QTextEdit;
using namespace std;
class faceRecognition : public QWidget
{
    Q_OBJECT

public:
    faceRecognition(QWidget *parent = 0);
    ~faceRecognition();
public slots:
    void onAddButtonClicked();
    void onDeleteButtonClicked();
    void onUpdateButtonClicked();

    void update_table();
    string take_id();

signals:
    void personDateEmmiter(QString name, QString lastName, QString officeName, QString job);
private:
    void createMembers();
    void setMembers();
    void setupLayouts();
    void connection();
    QPushButton *m_update, *m_addPerson/*, *m_open_folder*/;
    QLineEdit *m_setFirstName, *m_setLastName,  *m_setOfficeName, *m_setJob, *m_id;
    QLabel *m_name_label, *m_last_name_label, *m_office_name_label, *m_job_label/*,  *m_upload_label*/;
    QString mJsonpath;
    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;
    QPushButton *m_delete;
    QStringList horizontalHeader;
    QStringList verticalHeader;
    QVBoxLayout *VLayDel;
    QTableView *result;
    QMessageBox m_msg_box;
    //QTextEdit *m_upload_text_edit;
    QHBoxLayout *MainHorizontalLay;
    QVBoxLayout *VLay;
    QHBoxLayout *HLay;
    QGroupBox *VerticalBox, *DelGroupBox;/*, *HorBox*/

};

#endif // FACERECOGNITION_H
