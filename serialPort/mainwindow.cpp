#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QDebug"
#include"QMessageBox"
#include"windows.h"
#include"QFileDialog"
#include"QDesktopServices"
#include"QFile"
#include"stdlib.h"
#include"stdio.h"
#include"fstream"
using namespace std;

#define READ_DATA false
#define DISPLAY_DATA true

extern bool read_display_mutex;
extern bool clickedShowWaveBtn;
extern int precision_data;

QByteArray readData;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    time = new QTimer();
    stopShow=false;
    strSelectFileData="";
    readData = "";
    showHexFlag=false;
    precision_data = 8;

    initialSerial();//初始化串口
    read_display_mutex = READ_DATA;
    clickedShowWaveBtn = false;

    sysIcon=new QSystemTrayIcon(this);
    QIcon icon=QIcon(trUtf8("./sysTray.png"));
    sysIcon->setIcon(icon);
    sysIcon->setToolTip(trUtf8("Freescale_Smart_Car && Qt_Software!"));
    sysIcon->show();
    sysIcon->showMessage("sysTrayIcon","Qt software is runnning!",QSystemTrayIcon::Information,8000);

    menu=new QMenu(this);
    exitAct=new QAction(QIcon(trUtf8("../exit.png")),trUtf8("&Exit"),this);
    aboutAct=new QAction(QIcon(trUtf8("../about.png")),trUtf8("&About"),this);

    menu->addAction(exitAct);
    menu->addAction(aboutAct);
    sysIcon->setContextMenu(menu);

    ui->sendDataBtn->setEnabled(false);
    ui->closeSerial->setEnabled(false);
    ui->showWaveBtn->setEnabled(false);
    ui->selectFileBtn->setEnabled(false);
    ui->clearReceiveBtn->setEnabled(false);
    ui->clearSendBtn->setEnabled(false);
    ui->saveShowData->setEnabled(false);
    ui->stopShowBtn->setEnabled(false);
    ui->sendDataEdit->setEnabled(false);
    ui->autoSend->setEnabled(false);
    ui->autoShowCheck->setEnabled(false);
    ui->hexShowCheck->setEnabled(false);
    ui->sendFileBtn->setEnabled(false);
    ui->eightPreRadio->setChecked(true);

    hd =new HandleData();

    connect(ui->sendDataBtn,SIGNAL(clicked()),this,SLOT(sendDataBtn()));
    //connect(ui->sendDataBtn,SIGNAL(clicked()),this,SLOT(readMyCom()));
    //connect(ui->sendFileBtn,SIGNAL(clicked()),this,SLOT(readMyCom()));
    connect(ui->closeSerial,SIGNAL(clicked()),this,SLOT(closeSerial()));
    connect(ui->openSerial,SIGNAL(clicked()),this,SLOT(openSerial()));
    connect(ui->showWaveBtn,SIGNAL(clicked()),this,SLOT(showWaveBtn()));
    connect(ui->closeWindow,SIGNAL(clicked()),this,SLOT(closeWindow()));
    connect(ui->clearSendBtn,SIGNAL(clicked()),this,SLOT(clearSendArea()));
    connect(ui->clearReceiveBtn,SIGNAL(clicked()),this,SLOT(clearReceiveDataArea()));
    connect(ui->stopShowBtn,SIGNAL(clicked()),this,SLOT(stopShowData()));
    connect(ui->saveShowData,SIGNAL(clicked()),this,SLOT(saveShowData()));
    connect(ui->selectFileBtn,SIGNAL(clicked()),this,SLOT(SelectSendFileData()));
    connect(ui->sendFileBtn,SIGNAL(clicked()),this,SLOT(sendFileData()));
    connect(ui->autoSend,SIGNAL(clicked()),this,SLOT(autoSendTimeSet()));
    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(showAbout()));
}

void MainWindow::showWaveBtn()
{
    if(!clickedShowWaveBtn)
    {
        sw.setFixedSize(1360,700);
        sw.showNormal();
        sw.move(QPoint(0,0));
        sw.setWindowTitle("Display Wave!");
        sw.show();

        connect(hd,SIGNAL(send_Handle_Data()),&sw,SLOT(test()),Qt::QueuedConnection);
        hd->start(QThread::NormalPriority);

        clickedShowWaveBtn = true;
    }
    else
    {
        QMessageBox::warning(this,tr("Warning!!!"),tr("The wave window has been displayed!"),QMessageBox::Abort);
        hd->stop();
    }
}

void MainWindow::initialSerial()
{
    ui->portName->addItem("COM3");
    ui->portName->addItem("COM1");
    ui->portName->addItem("COM2"); 
    ui->portName->addItem("COM4");
    ui->portName->addItem("COM5");
    ui->portName->addItem("COM6");
    ui->portName->addItem("COM7");
    ui->portName->addItem("COM8");
    ui->portName->addItem("COM9");

    ui->baudRate->addItem("9600");
    ui->baudRate->addItem("1200");
    ui->baudRate->addItem("2400");
    ui->baudRate->addItem("4800");
    ui->baudRate->addItem("14400");
    ui->baudRate->addItem("19200");
    ui->baudRate->addItem("38400");
    ui->baudRate->addItem("56000");
    ui->baudRate->addItem("115200");
    ui->baudRate->addItem("128000");
    ui->baudRate->addItem("256000");

    ui->dataBits->addItem("8");
    ui->dataBits->addItem("5");
    ui->dataBits->addItem("6");
    ui->dataBits->addItem("7");

    ui->parityCom->addItem("none");
    ui->parityCom->addItem("odd");//奇校验
    ui->parityCom->addItem("even");//偶校验

    ui->stopBit->addItem("1");
    ui->stopBit->addItem("1.5");
    ui->stopBit->addItem("2");

    ui->flowType->addItem("off");
    ui->flowType->addItem("hardware");
    ui->flowType->addItem("xonxoff");
}

void MainWindow::readMyCom()
{
    if(read_display_mutex == READ_DATA)
     {
         temp = myCom->readAll();

         if(temp.isEmpty())
         {
             return ;
         }
         if(!clickedShowWaveBtn)
         {
             read_display_mutex = READ_DATA;
         }
         else
         {
             /*if(ui->tenPreRadio->isChecked() || ui->twelPreRadio->isChecked())
             {
                 processShareData();
             }
             else
             {
                 readData = temp;
             }*/
             readData = temp;

             read_display_mutex = DISPLAY_DATA;
         }

         showPlainText();
         temp.clear();
         qDebug()<<"readData!";
     }
 }

 void MainWindow::showPlainText()
{
    if(!stopShow)
    {
        if(temp.isEmpty())
        {
            return ;
        }
        /*if(ui->autoShowCheck->isChecked())
        {
            if(ui->reciveData->isForwardAvailable())
                ui->reciveData->clear();
        }*/

        if(ui->hexShowCheck->isChecked())
        {
            //ui->reciveData->insertPlainText(stringSpaceToHex(temp.toHex(),' '));
            //ui->reciveData->append(stringSpaceToHex(temp.toHex(),' '));
            ui->reciveData->append(temp.data());
        }
        else
        {
            //ui->reciveData->insertPlainText(temp);
            ui->reciveData->append(temp.data());
            //将串口的数据显示在窗口的文本浏览器中
        }

        //qDebug()<<temp<<" bytes! "<<"\n";

        /*const char fileName[]="./test_data.txt";
        ofstream  fout(fileName,ios::app|ios::out);
        fout<<"This is a Qt program!";
        fout<<hex<<"Hello qorld!";
        fout<<(ui->reciveData->toPlainText().constData());
        fout<<ends;
        fout.close();*/
    }
    else
    {
        myCom->readAll();
        ui->reciveData->insertPlainText("");
    }
}

QString MainWindow::stringSpaceToHex(QByteArray str,char tag)
{
    int length=str.length();
    //qDebug()<<str<<"\n";

    QString result="";
    int j=0;
    for(int i=0;i<length;i+=2)
    {
        result[j]=str[i];
        result[j+1]=str[i+1];
        result[j+2]=tag;
        j+=3;
    }

    qDebug()<<result<<"\n";
    return result;
}

void MainWindow::sendDataBtn()
{
    if(ui->sendDataEdit->text()=="")
        QMessageBox::warning(this,tr("Warning!"),tr("Cann't send empty string!\n   Try again1"),QMessageBox::Ok);
    myCom->write(ui->sendDataEdit->text().toAscii());
    qDebug()<<"write:"<<myCom->bytesToWrite()<<"bytes";//要发送的字节数
}

void MainWindow::openSerial()
{
    //connect(cd,SIGNAL(coleectDataNow()),this,SLOT(readMyCom()),Qt::DirectConnection);
    //cd->start(QThread::HighPriority);
    connect(time,SIGNAL(timeout()),this,SLOT(timeToReadCom()));

    QString portName=ui->portName->currentText();
    QString baudRate=ui->baudRate->currentText();
    QString dataBits=ui->dataBits->currentText();
    QString parity=ui->parityCom->currentText();
    QString stopBit=ui->stopBit->currentText();
    QString flow=ui->flowType->currentText();

    if(ui->eightPreRadio->isChecked())
    {
        precision_data = 8;
    }
    else if(ui->tenPreRadio->isChecked())
    {
        precision_data = 10;
    }
    else if(ui->twelPreRadio->isChecked())
    {
        precision_data = 12;
    }

    myCom=new Win_QextSerialPort(portName,QextSerialBase::EventDriven);
    //connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    myCom->open(QIODevice::ReadWrite);
    //timeId  = startTimer(1000);
    time->start(600);

    if(baudRate==tr("1200"))
        myCom->setBaudRate(BAUD1200);
    else if(baudRate==tr("1800"))
        myCom->setBaudRate(BAUD1800);
    else if(baudRate==tr("2400"))
        myCom->setBaudRate(BAUD2400);
    else if(baudRate==tr("4800"))
        myCom->setBaudRate(BAUD4800);
    else if(baudRate==tr("9600"))
        myCom->setBaudRate(BAUD9600);
    else if(baudRate==tr("14400"))
        myCom->setBaudRate(BAUD14400);
    else if(baudRate==tr("19200"))
        myCom->setBaudRate(BAUD19200);
    else if(baudRate==tr("38400"))
        myCom->setBaudRate(BAUD38400);
    else if(baudRate==tr("56000"))
        myCom->setBaudRate(BAUD56000);
    else if(baudRate==tr("115200"))
        myCom->setBaudRate(BAUD115200);
    else if(baudRate==tr("128000"))
        myCom->setBaudRate(BAUD128000);
    else if(baudRate==tr("256000"))
        myCom->setBaudRate(BAUD256000);

    if(dataBits==tr("5"))
        myCom->setDataBits(DATA_5);
    else if(dataBits==tr("6"))
        myCom->setDataBits(DATA_6);
    else if(dataBits==tr("7"))
        myCom->setDataBits(DATA_7);
    else if(dataBits==tr("8"))
        myCom->setDataBits(DATA_8);

    if(parity==tr("none"))
        myCom->setParity(PAR_NONE);
    else if(parity==tr("odd"))
        myCom->setParity(PAR_ODD);
    else if(parity==tr("even"))
        myCom->setParity(PAR_EVEN);

    if(stopBit==tr("1"))
        myCom->setStopBits(STOP_1);
    else if(stopBit==tr("1.5"))
        myCom->setStopBits(STOP_1_5);
    else if(stopBit==tr("2"))
        myCom->setStopBits(STOP_2);

    if(flow==tr("off"))
        myCom->setFlowControl(FLOW_OFF);
    if(flow==tr("hardware"))
        myCom->setFlowControl(FLOW_HARDWARE);
    if(flow==tr("xonxoff"))
        myCom->setFlowControl(FLOW_XONXOFF);

    //myCom->setFlowControl(FLOW_OFF);//数据流控制设置，设置为无数据流控制
    myCom->setTimeout(500);

    ui->openSerial->setEnabled(false);
    ui->closeSerial->setEnabled(true);
    ui->sendDataBtn->setEnabled(true);
    ui->portName->setEnabled(false);
    ui->baudRate->setEnabled(false);
    ui->dataBits->setEnabled(false);
    ui->parityCom->setEnabled(false);
    ui->stopBit->setEnabled(false);
    ui->flowType->setEnabled(false);
    ui->closeSerial->setEnabled(true);
    ui->showWaveBtn->setEnabled(true);
    ui->selectFileBtn->setEnabled(true);
    ui->clearReceiveBtn->setEnabled(true);
    ui->clearSendBtn->setEnabled(true);
    ui->saveShowData->setEnabled(true);
    ui->stopShowBtn->setEnabled(true);
    ui->sendDataEdit->setEnabled(true);
    ui->autoSend->setEnabled(true);
    ui->autoShowCheck->setEnabled(true);
    ui->hexShowCheck->setEnabled(true);
    ui->sendFileBtn->setEnabled(false);
    ui->eightPreRadio->setEnabled(false);
    ui->tenPreRadio->setEnabled(false);
    ui->twelPreRadio->setEnabled(false);

    autoSendTime=new AutoSendData(myCom);
}

void MainWindow::closeSerial()
{
    myCom->close();
    //this->killTimer(timeId);
    time->stop();
    hd->terminate();

    ui->openSerial->setEnabled(true);
    ui->closeSerial->setEnabled(false);
    ui->sendDataBtn->setEnabled(false);
    ui->portName->setEnabled(true);
    ui->baudRate->setEnabled(true);
    ui->dataBits->setEnabled(true);
    ui->parityCom->setEnabled(true);
    ui->stopBit->setEnabled(true);
    ui->flowType->setEnabled(true);
    ui->showWaveBtn->setEnabled(false);
    ui->selectFileBtn->setEnabled(false);
    ui->clearReceiveBtn->setEnabled(false);
    ui->clearSendBtn->setEnabled(false);
    ui->saveShowData->setEnabled(false);
    ui->stopShowBtn->setEnabled(false);
    ui->sendDataEdit->setEnabled(false);
    ui->autoSend->setEnabled(false);
    ui->autoShowCheck->setEnabled(false);
    ui->hexShowCheck->setEnabled(false);
    ui->sendFileBtn->setEnabled(false);
    ui->eightPreRadio->setEnabled(true);
    ui->tenPreRadio->setEnabled(true);
    ui->twelPreRadio->setEnabled(true);

    delete myCom;
    myCom = NULL;
    //unlink("./shareData.txt");
}

MainWindow::~MainWindow()
{
    delete ui;

    if(myCom != NULL)
       delete myCom;
}

void MainWindow::timeToReadCom()
{
    readMyCom();
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    this->close();
    sw.close();
    //unlink("./shareData.txt");
}

void MainWindow::closeWindow()
{
    this->close();
    sw.close();

    //unlink("./shareData.txt");
}


void MainWindow::clearSendArea()
{
    ui->sendDataEdit->clear();
}

void MainWindow::clearReceiveDataArea()
{
    ui->reciveData->clear();
}

void MainWindow::stopShowData()
{
    if(!stopShow)
    {
        ui->stopShowBtn->setText(tr("继续显示"));
        stopShow=true;
    }
    else
    {
        ui->stopShowBtn->setText(tr("停止显示"));
        stopShow=false;
    }
}

void MainWindow::saveShowData()
{
    QString path="./";
    QString fileName=QFileDialog::getSaveFileName(this,
                                                  tr("Save Files(*.txt)"),
                                                  path,
                                                  tr("txt files(*.txt)"));
    if(fileName.isNull())
    {
        QMessageBox::warning(this,tr("File Name Error!"),tr("file name cann't be null!"),QMessageBox::Ok);
    }
    else
    {
        QFile saveFile;
        saveFile.setFileName(fileName);
        if(saveFile.open(QIODevice::ReadWrite|QFile::Text))
        {
            QTextStream saveFileStream(&saveFile);
            saveFileStream<<ui->reciveData->toPlainText();
            saveFileStream.atEnd();
            saveFile.close();
        }
    }
}

void MainWindow::SelectSendFileData()
{
    QString path="./";
    QString fileName=QFileDialog::getOpenFileName(this,
                                                  tr("Select Files(*.txt)"),
                                                  path,
                                                  tr("txt files(*.txt)"));
    if(fileName.isNull())
    {
        QMessageBox::warning(this,tr("File Name Error!"),tr("file name cann't be null!"),QMessageBox::Ok);
    }
    else
    {
        QFile openFile;
        openFile.setFileName(fileName);
        if(openFile.open(QIODevice::ReadOnly|QFile::Text))
        {
            QTextStream openFileStream(&openFile);
            openFileStream>>strSelectFileData,
            openFileStream.atEnd();
            openFile.close();
        }
    }
    ui->sendFileBtn->setEnabled(true);

    if(strSelectFileData=="")
    {
        QMessageBox::warning(this,tr("Warning!"),tr("没有选择文件或者选择的文件为空!\n请重新选择文件后再发送!"),QMessageBox::Ok);
        return ;
    }
    ui->sendDataEdit->clear();
    ui->sendDataEdit->setText(strSelectFileData);
}

void MainWindow::sendFileData()
{
    myCom->write(ui->sendDataEdit->text().toAscii());
    strSelectFileData="";
    ui->sendFileBtn->setEnabled(false);
    ui->selectFileBtn->setEnabled(true);
}

void MainWindow::autoSendTimeSet()
{
    autoSendTime->setWindowTitle("Auto Send!");
    autoSendTime->show();
}


/*void MainWindow::timerEvent(QTimerEvent *e)
{
    readMyCom();
}*/

void MainWindow::processShareData()
{
    /*int j = 0;
    ofstream fout("./shareData.txt",ios::app|ios::out);
    fout<<temp.constData();
    fout.close();*/
}

void MainWindow::iconSystemActived(QSystemTrayIcon::ActivationReason reason)
{
}

void MainWindow::showAbout()
{
    af.show();
}










