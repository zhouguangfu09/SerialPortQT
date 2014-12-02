#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "handledata.h"
#include <QMainWindow>
#include"win_qextserialport.h"
#include "qextserialbase.h"
#include "autosenddata.h"
#include"QTimerEvent"
#include"showwave.h"
#include"QTimer"
#include"QSystemTrayIcon"
#include "aboutform.h"
//#include "collectdata.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Win_QextSerialPort *myCom;

    bool stopShow;
    QString strSelectFileData;
    AutoSendData *autoSendTime;
    bool showHexFlag;

    showWave sw;
    int timeId;
    QByteArray temp;

    QSystemTrayIcon *sysIcon;
    QMenu *menu;
    QAction *exitAct,*aboutAct;
    HandleData *hd;
    AboutForm af;
    QTimer *time ;

    //Thread---Handle_data

private slots:
    void sendDataBtn();
    void openSerial();
    void closeSerial();
    void showWaveBtn();
    void closeWindow();
    void clearSendArea();
    void clearReceiveDataArea();
    void stopShowData();
    void saveShowData();
    void SelectSendFileData();
    void sendFileData();
    void autoSendTimeSet();
    void showPlainText();
    //void timerEvent(QTimerEvent *e);
    void timeToReadCom();

    void iconSystemActived(QSystemTrayIcon::ActivationReason reason);
    void showAbout();

public:
    void initialSerial();
    void closeEvent(QCloseEvent* e);
    QString stringSpaceToHex(QByteArray str,char tag);
    void processShareData();
    void readMyCom();

};

#endif // MAINWINDOW_H
