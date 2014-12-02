#ifndef AUTOSENDDATA_H
#define AUTOSENDDATA_H

#include <QtGui/QWidget>
#include"win_qextserialport.h"
#include "qextserialbase.h"
#include"QTimerEvent"
#include"QTimer"

namespace Ui {
class AutoSendData;
}

class AutoSendData : public QWidget
{
    Q_OBJECT
    
public:
    explicit AutoSendData(QWidget *parent = 0);
    AutoSendData(Win_QextSerialPort *myCom=0,QWidget *parent=0);
    ~AutoSendData();
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::AutoSendData *ui;
    int timeId;
    int interval;
    bool flag;
    Win_QextSerialPort *myCom;

public:
    void timerEvent(QTimerEvent *e);
    void closeEvent(QCloseEvent* e);

public slots:
    void setAutoTime();
    void autoTimeSpinSet();
};

#endif // AUTOSENDDATA_H
