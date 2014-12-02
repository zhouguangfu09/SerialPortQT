#include "autosenddata.h"
#include "ui_autosenddata.h"
#include"QMessageBox"

AutoSendData::AutoSendData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoSendData)
{
    ui->setupUi(this);
}


AutoSendData::AutoSendData(Win_QextSerialPort *myCom,QWidget *parent):
   ui(new Ui::AutoSendData)
{
    ui->setupUi(this);
    this->myCom=myCom;
    interval=0;
    flag=true;
    ui->autoSendTimeSpin->setRange(1,1000);
    ui->autoSendTimeSpin->setValue(100);
    ui->setAutoTimeBtn->setEnabled(true);
    connect(ui->autoSendTimeSpin,SIGNAL(valueChanged(int)),this,SLOT(autoTimeSpinSet()));
    connect(ui->setAutoTimeBtn,SIGNAL(clicked()),this,SLOT(setAutoTime()));
}

AutoSendData::~AutoSendData()
{
    killTimer(timeId);
    delete ui;    
}

void AutoSendData::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void AutoSendData::timerEvent(QTimerEvent *e)
{
     myCom->write(ui->plainTextEdit->toPlainText().toAscii());
}

void AutoSendData::setAutoTime()
{
    if(flag)
    {
        if(ui->plainTextEdit->toPlainText()=="")
        {
            QMessageBox::warning(this,tr("Warning!"),trUtf8("The input text is empty! \nPlease try again!!"),QMessageBox::Ok);
            return ;
        }

        //ui->setAutoTimeBtn->setText(trUtf8("ֹͣ"));
        ui->setAutoTimeBtn->setText(trUtf8("Stop"));
        timeId=startTimer(interval);
        flag=false;
        ui->autoSendTimeSpin->setEnabled(false);
        ui->plainTextEdit->setEnabled(false);
    }
    else
    {
        ui->setAutoTimeBtn->setText(trUtf8("Set"));
        killTimer(timeId);
        flag=true;
        ui->autoSendTimeSpin->setEnabled(true);
        ui->plainTextEdit->setEnabled(true);
    }
}

void AutoSendData::autoTimeSpinSet()
{
    interval=ui->autoSendTimeSpin->value();
}

void AutoSendData::closeEvent(QCloseEvent* e)
{
    //delete myCom;
    killTimer(timeId);
    this->close();
}










