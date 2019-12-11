#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
class QPushButton;
class QLineEdit;
class QVBoxLayout;
class QLabel;
class QString;
class QFrame;
class QHBoxLayout;
class QGroupBox;
class QTextEdit;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onAddButtonClicked();
//    void onOpenButtonClicked();
private:
    void createMembers();
    void setMembers();
    void setupLayouts();
    void connection();
    QPushButton *m_addPerson/*, *m_open_folder*/;
    QLineEdit *m_setFirstName, *m_setLastName,  *m_setOfficeName, *m_setJob;
    QLabel *m_name_label, *m_last_name_label, *m_office_name_label, *m_job_label/*,  *m_upload_label*/;
    QString mJsonpath;
    QMessageBox m_msg_box;
    //QTextEdit *m_upload_text_edit;
    QVBoxLayout *MainVerticalLay,*VLay;
    QHBoxLayout *HLay;
    QGroupBox *VerticalBox/*, *HorBox*/;
};
#endif // MAINWINDOW_H
