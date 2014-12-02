#include "showwave.h"
#include "ui_showwave.h"
#include"qdebug.h"
#include"qmath.h"
#include"stdlib.h"
#include"stdio.h"
#include"fstream"
#include"QFileDialog"
#include"QMessageBox"
#include "aboutform.h"

using namespace std;

#define PI 3.14159265
#define WIDTH 1220

#define D1 50
#define D2 50
#define D3 70
#define D4 80
#define READ_DATA false
#define DISPLAY_DATA true

typedef unsigned short uint16;

extern bool read_display_mutex;
extern bool clickedShowWaveBtn;
extern int precision_data;
extern QByteArray readData;

showWave::showWave(QWidget *parent) :
        QMainWindow(parent),ui(new Ui::showWave)
{
    ui->setupUi(this);

    //rect=ui->graphicWidget->rect();
    rect=QRect(0,0,WIDTH,600);
    pointt = new int[rect.width()];
    height =rect.height();
    width = rect.width();
    flag_load = false;

    high_data = false;
    left_data = 65535;
    start = true;

    COLUMN = DATA_ARRAY_SIZE;
    VALY = 1;
    RECT_HEIGHT = rect.height();
    display_wave_flag = false;

    m_pen_width = 1;
    m_other_pen_width = 1;

    ui->penWidthSpin->setValue(m_pen_width);
    ui->penWidthSpin->setRange(1,20);

    ui->otherPenWidthSpin->setValue(m_other_pen_width);
    ui->otherPenWidthSpin->setRange(1,20);

    //QMessageBox::information(this,"just test!","display window!",0);
    //SEEK_POS =0;
    flag1=false;
    flag=0;
    initPoint.setX(40);
    initPoint.setY(20);
    N=1;
    VAL=2;
    ui->interalPixelsSpin->setValue(VAL);
    ui->interalPixelsSpin->setRange(1,100);

    gridnum=10;
    ui->gridNum->setValue(10);
    ui->gridNum->setRange(1,100);
    x=0.0;

    ui->NValueSpinBox->setRange(1,100);
    ui->NValueSpinBox->setValue(N);

    num = new int[DATA_ARRAY_SIZE];
    memset(num,65530,sizeof(int)*DATA_ARRAY_SIZE );

    sum1=new int[COLUMN/N];
    sum2 = new int[COLUMN-N];
    sum3 = new int[COLUMN/N];
    sum4 =new int[COLUMN/N];


    //time = new QTimer();
    //time->start(50);

    connect(ui->drawGridBtn,SIGNAL(clicked()),this,SLOT(drawGridBtn()));
    connect(ui->gridNum,SIGNAL(valueChanged(int)),this,SLOT(gridSpin()));
    connect(ui->loadFileBtn,SIGNAL(clicked()),this,SLOT(loadFile()));
    connect(ui->showWaveBtn,SIGNAL(clicked()),this,SLOT(showNumber()));
    //connect(ui->saveWaveBtn,SIGNAL(clicked()),this,SLOT(saveShowData(int *);
    connect(ui->showMathAvgBtn,SIGNAL(clicked()),this,SLOT(on_showMathAvgBtn_clicked()));
    //connect(time,SIGNAL(timeout()),this,SLOT(test()));
}
void showWave::paintEvent(QPaintEvent *e)
{
    painter=new QPainter(this);
    painter->translate(initPoint.x(),initPoint.y());
    painter->begin(this);
    painter->drawRect(rect);
    painter->setPen(Qt::red);
    setXYCord();
    painter->fillRect(rect,QBrush(QColor(Qt::black),Qt::SolidPattern));

    if(!flag1)
    {
        showGrid();
    }
    RealTimeWave();
    painter->end();
}

showWave::~showWave()
{
    delete painter;
    delete ui;
    if((sum2!= NULL)&& (sum3 != NULL)&& (sum4!= NULL))
    {
        delete []sum2;
        delete []sum3;
        delete []sum4;
        sum2 = NULL;
        sum3 = NULL;
        sum4 = NULL;
    }

    delete pointt;
}

void showWave::setXYCord()
{
    //x
    painter->setPen(QPen(Qt::blue));
    for(int i=0;i<rect.width()/10;i++)
    {
        point[0].setX(i*10);
        point[0].setY(height);
        point[1].setX(i*10);
        point[1].setY(height-3);
        painter->drawLine(QPoint(point[0].x(),point[0].y()),QPoint(point[1].x(),point[1].y()));
    }
    painter->drawLine(QPoint(0,height),QPoint(width,height));

    //y
    painter->setPen(QPen(Qt::red));
    for(int i=height/10;i>0;i--)
    {
        point[0].setX(0);
        point[0].setY(i*10);
        point[1].setX(3);
        point[1].setY(i*10);
        painter->drawLine(QPoint(point[0].x(),point[0].y()),QPoint(point[1].x(),point[1].y()));
    }
    painter->drawLine(QPoint(0,0),QPoint(0,height));

    //y值
    int j=height/10-1;
    QFont font("Arial",6,QFont::Normal,true);
    painter->setFont(font);
    for(int i=-200;i<=height-200;i+=10)
    {
        painter->drawText(rect.x()-13,height-(i+200)+3,QString::number((height-3)/10-j-20,10));
        j--;
    }

    //x值
    j=1;
    painter->setFont(font);
    painter->setPen(QPen(Qt::black));
    for(int i=10;i<width;i+=10)
    {
        painter->drawText(rect.x()+i-3,height+10,QString::number(j,10));
        j++;
    }
}

void showWave::drawGridBtn()
{
    if(!flag1)
    {
        ui->drawGridBtn->setText("显示网格");
        flag1=true;
    }
    else
    {
        ui->drawGridBtn->setText("隐藏网格");
        flag1=false;
    }

    //setUpdatesEnabled(true);
    //update();
    repaint();
}

void showWave::showGrid()
{
    //x
    QPen pen(Qt::darkGreen);

    painter->setPen(pen);
    //tmpPainter->setPen(pen);

    //x
    point[0].setY(0);
    point[1].setY(height);

    for(int i=0;i<rect.width()/gridnum;i++)
    {
        point[0].setX(i*gridnum);
        point[1].setX(i*gridnum);
        painter->drawLine(QPoint(point[0].x(),point[0].y()),QPoint(point[1].x(),point[1].y()));
    }
    //y
    point[0].setX(0);
    point[1].setX(width);
    for(int i=height/gridnum;i>=0;i--)
    {
        point[0].setY(i*gridnum);
        point[1].setY(i*gridnum);
        painter->drawLine(QPoint(point[0].x(),point[0].y()),QPoint(point[1].x(),point[1].y()));
    }
}

void showWave::showSinWave()
{
    int xx=100;
    float tmp;
    painter->drawLine(QPoint(0,180),QPoint(rect.width(),180));
    for(;xx<rect.width()+100;xx++)
    {
        tmp=(int)(sin(xx*PI*1.0/100+x)*160+0.5)+180;
        painter->drawPoint(QPoint(xx-100,tmp));
        pointt[xx-100]=tmp;
    }
}

void showWave::gridSpin()
{
    gridnum=ui->gridNum->value();
    setUpdatesEnabled(true);
    update();
    repaint();
}

void showWave::saveShowData(int *point)
{
    QString path="./";
    QString tmpStr="";
    for(int i=0;i<rect.width();i++)
    {
        tmpStr.append(QString::number(pointt[i],10));
        tmpStr.append(" ");
    }
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
            saveFileStream<<tmpStr;
            saveFileStream.atEnd();
            saveFile.close();
        }
    }
}

void showWave::loadFile()
{
    int j;
    QString path="./";
    QString fileName=QFileDialog::getOpenFileName(this,
                                                  tr("Select Files(*.txt)"),
                                                  path,
                                                  tr("txt files(*.txt)"));
    if(fileName.isNull())
    {
        QMessageBox::warning(this,tr("File Name Error!"),tr("File name cann't be null!"),QMessageBox::Ok);
    }
    else
    {
        ifstream fin(fileName.toAscii().constData());

        for(j = 0;j < WIDTH; j++)
        {
            if(fin.eof())
                break;

            fin>>dec>>num[j];
        }
        COLUMN = j;

        fin.close();

        /*for(int k=0;k<COLUMN;k+=3)
        {
            temp=(num[k]+num[k+1]+num[k+2])/3.0;
            num[m]=(int)(temp+0.5);
            m++;
        }*/
    }

    flag_load = false;
}

void showWave::showNumber()
{
    setUpdatesEnabled(true);
    repaint(rect);
}


void showWave::mathAvg()
{
    memset(sum1,0,sizeof(int)*COLUMN/N);

    int i,j,k,temp,x;
    temp=COLUMN/N;
    k=0;
    x=0;

    for(i=0;i<temp;i++)
    {
        for(j=0;j<N;j++,k++)
        {
            sum1[i]+=num[k];
        }
        sum1[i]/=N;
    }

    i=0;
    for(int n=0;n<temp-1;n++)
    {
        int start=0;
        height = 100;
        for(int j=0;j<COLUMN-1;j++)
        {
            //painter->drawLine(QPoint(start,rect.height()-(sum1[j]+height)),QPoint(start+VAL,rect.height()-(sum1[j+1]+height)));
            painter->drawPoint(QPoint(start,num[j]));
            start+=VAL;

            if(start>WIDTH)
                break;
        }
    }
}

void showWave::on_NValueSpinBox_valueChanged(int )
{
    N=ui->NValueSpinBox->value();
    if( N>2 )
        ui->midAvgBtn->setEnabled(true);
    else
        ui->midAvgBtn->setEnabled(false);
    showNumber();
}

void showWave::on_slideBtn_clicked()
{
    flag=1;
    showNumber();
}

void showWave::slideAvg()
{
    memset(sum2,0,sizeof(int)*(COLUMN-N));

    int i,j,k;

    for(i=0;i<COLUMN-N;i++)
    {
        for(j=0;j<N;j++)
        {
            sum2[i]+=num[j+i];
        }
        sum2[i]/=N;

        if(i>WIDTH || num[i] >= 65535 )
            break;
    }

    i=0;
    for(k=0;k<COLUMN-N-1;k++)
    {
        //painter->drawPoint(QPoint(i,(RECT_HEIGHT-sum2[k]/VALY-D2)));
        painter->drawLine(QPoint(i,(RECT_HEIGHT-sum2[k]/VALY-D2)),QPoint(i+VAL,(RECT_HEIGHT-sum2[k+1]/VALY-D2)));
        i+=VAL;
        if(i > WIDTH || num[i] >= 65535)
            break;
    }
}

void showWave::on_midAvgBtn_clicked()
{
    flag=2;
    showNumber();
}

void showWave::midAvg()
{
    if(N < 3)
        return ;

    int i,j,k;
    int min,max;
    k = 0;
    min = 0;
    max = 0;

    memset(sum3,0,sizeof(int)*COLUMN/N);

    for(i=0;i<COLUMN/N;i++)
    {
        if(i>WIDTH || num[k+N] >= 65500 )
            break;

        for(j=0;j<N;j++)
        {
            if(min>num[k])
            {
                min=num[k];
            }
            else if(max<num[k])
            {
                max=num[k];
            }

            k++;
            sum3[i]+=num[k];
        }

        sum3[i]-=min+max;
        sum3[i]/=N-2;
    }

    j=0;
    for(k=0;k<i-1;k++)
    {
        painter->drawLine(QPoint(j,(RECT_HEIGHT-sum3[k]/VALY-D3)),QPoint(j+VAL,(RECT_HEIGHT-sum3[k+1]/VALY-D3)));
        j+=VAL;
    }
}

void showWave::on_pushButton_clicked()
{
    flag=3;
    showNumber();
}

void showWave::addWeightAvg()
{
    int i,j,k;
    k=0;
    int weights=0;

    memset(sum4,0,sizeof(int)*COLUMN/N);

    for(i=1;i<=N;i++)
    {
        weights+=i;
    }

    for(i=0;i<COLUMN/N;i++)
    {
        if(i>WIDTH || num[k+N] >= 65535)
            break;
        for(j=0;j<N;j++)
        {
            sum4[i]+=num[k]*(j+1);
            k++;
        }
        sum4[i]/=weights;
    }

    j=0;
    for(k=0;k<i-1;k++)
    {
        painter->drawLine(QPoint(j,(RECT_HEIGHT-sum4[k]/VALY-D4)),QPoint(j+VAL,(RECT_HEIGHT-sum4[k+1]/VALY)-D4));
        j+=VAL;
    }
}


void showWave::on_interalPixelsSpin_valueChanged(int )
{
    VAL=ui->interalPixelsSpin->value();
    showNumber();
}


void showWave::RealTimeWave()
{
    int startt=0;
    for(int j=0;j<COLUMN;j++)
    {
        painter->setPen(QPen(QPen(Qt::green,m_pen_width,Qt::SolidLine)));
        //painter->drawLine(QPoint(start,rect.height()-num[j]-D1),QPoint(start+VAL,rect.height()-num[j+1]-D1));
        painter->drawPoint(QPoint(startt,(RECT_HEIGHT-num[j]/VALY-D1)));

        startt+=VAL;

        if(startt>WIDTH || num[j] >= 65535 )
            break;
    }

    if(flag==1)
    {
        painter->setPen(QPen(Qt::white,m_other_pen_width,Qt::SolidLine));
        slideAvg();
    }
    else if(flag==2)
    {
        painter->setPen(QPen(Qt::red,m_other_pen_width,Qt::SolidLine));
        midAvg();
    }
    else if(flag==3)
    {
        painter->setPen(QPen(Qt::yellow,m_other_pen_width,Qt::SolidLine));
        addWeightAvg();
    }
    else if(flag==4)
    {
        painter->setPen(QPen(Qt::yellow,m_other_pen_width,Qt::SolidLine));
        addWeightAvg();
    }

}


/*void showWave::timerEvent(QTimerEvent *e)
{
    if(!flag_load)
    {
        if(read_display_mutex == DISPLAY_DATA)
        {
           processShareFile();
           read_display_mutex = READ_DATA;
        }
        else
        {
            return;
        }
    }

    showNumber();
}*/

void showWave::test()
{

    processShareFile();
    showNumber();
}


void showWave::closeEvent(QCloseEvent *e)
{
    this->close();
    clickedShowWaveBtn = false;

    //unlink("./shareData.txt");
}

void showWave::processShareFile()
{

    if(!flag_load)
    {
        if(read_display_mutex == DISPLAY_DATA)
        {
            if(precision_data == 8)
            {
                processEightData();
            }
            else if(precision_data == 10)
            {
                VALY = 4;
                processTenOrTwelveData();
            }
            else if(precision_data == 12)
            {
                VALY = 16;
                processTenOrTwelveData();
            }

            read_display_mutex = READ_DATA;
        }
        else
        {
            return;
        }
    }

}

void showWave::on_showNormalBtn_clicked()
{
    flag = 0;
    showNumber();
}

void showWave::on_penWidthSpin_valueChanged(int )
{
    m_pen_width = ui->penWidthSpin->value();
    showNumber();
}

void showWave::on_otherPenWidthSpin_valueChanged(int )
{
    m_other_pen_width = ui->otherPenWidthSpin->value();
    showNumber();
}

void showWave::processEightData()
{
    /*int j=0;
    ifstream fin("./shareData.txt",ios::in);
    if(!fin)
    {
        qDebug()<<"Cann't find the file!";
        return ;
    }
    fin.seekg(0,ios::end);
    COLUMN = fin.tellg()/2;
    num = new int[COLUMN];
    memset(num,-1,sizeof(int)*COLUMN);
    for(int m=0; m<50; m++)
        qDebug()<<num[m];

    fin.seekg(0,ios::beg);
    for(int j=0;j<COLUMN;j++)
    {
         fin>>dec>>num[j];
         qDebug()<<num[j]<<"-------------------";
         j++;
    }

    fin.close();
    unlink("./shareData.txt");*/

    int length = readData.length();
    for(int i=0;i < length;i++)
    {
        num[i] = ((int)readData[i]&255);
        qDebug()<<num[i]<<"-------8------";
    }

    readData = "";
    COLUMN = length;
}

void showWave::processTenOrTwelveData()
{
    /*int i=0,j=0;
    //int temp;
    //const unsigned char *buff;
    ifstream fin("./shareData.txt",ios::in);
    if(!fin)
    {
        qDebug()<<"Cann't find the file!";
        return ;
    }
    fin.seekg(SEEK_POS,ios::end);
    temp = fin.tellg();
    if(temp %2 !=0)
    {
        temp--;
    }
    buff = new unsigned char[temp];
    fin.seekg(SEEK_POS,ios::beg);
    SEEK_POS += temp + 1;

    for(int j=0;j<COLUMN;j++)
    {
         fin>>dec>>num[i];
         qDebug()<<num[i]<<"------------!!!";
         i++;

         if(fin.eof())
             break;
    }

    fin.close();
    unlink("./shareData.txt");*/

    /*if(temp1)
    for(int i = start;i < length-1-end; i+=2)
    {
        //num[k] = (int)(((uint16)readData[i])&255)*255+(int)((uint16)readData[i+1])&255;
        temp1 = ((int)readData[i]&255)*256;
        temp2 = ((int)readData[i+1]&255);
        num[k] = temp1;
        k++;
        qDebug()<<num[k]<<"-------10---//--16----"<<temp1<<temp2<<"------"<<((int)readData[i]&255)<<"--"<<((int)readData[i+1]&255);
        num[k] = temp1;
        qDebug()<<num[k]<<"------"<<temp1<<"********";
    }

    readData = "";
    COLUMN = length/2;*/

    /*int length = readData.length();
    if(start)
    {
        if(length%2 !=0)
        {
            int k = 0,i;

            for(i = 0;i < length-2; i+=2)
            {
                num[k] = num[k] = (((int)readData[i]&255)*256)+((int)readData[i+1]&255);
                qDebug()<<num[k]<<"-------10---------"<<(((int)readData[i]&255)*256)<<"***"<<((uint16)readData[i+1]&255);
                k++;
            }
            readData = "";
            COLUMN = length/2;

            left_data = ((int)readData[length-1]&255);
            high_data = true;
            start = false;
        }
        else
        {
            int k = 0,i;
            for(i = 0;i < length-1; i+=2)
            {
                num[k] = (((int)readData[i]&255)*256)+((int)readData[i+1]&255);
                qDebug()<<num[k]<<"-------10---------"<<((int)readData[i]&255)<<"--"<<((int)readData[i+1]&255);
                k++;
            }

            readData = "";
            COLUMN = length/2;

            start = true;
            high_data = false;
            left_data = 0;
        }
    }
    else
    {
        if(length%2 !=0)
        {

            int k = 0,i;

            for(i = 0;i < length-1; i+=2)
            {
                num[k] = (int)left_data*256+((int)readData[i]&255);
                qDebug()<<num[k]<<"-------10---------"<<((int)readData[i]&255)<<"--"<<((int)readData[i+1]&255);
                k++;
            }

            readData = "";
            COLUMN = length/2;

            left_data = 0;
            high_data = false;
            start = true;

            else
            {
                int k = 0,i;
                for(i = 0;i < length-2; i+=2)
                {
                    num[k] = (int)(((unsigned short )readData[i])&255)*256+(int)((unsigned short )readData[i+1]&255);
                    k++;
                    qDebug()<<num[k]<<"-------10---------"<<((unsigned short )readData[i]&255)<<"--"<<((unsigned short )readData[i+1]&255);
                }

                readData = "";
                COLUMN = length/2;
                left_data = (int)((uint16)readData[length-1])&255;
                high_data = true;
            }
        }
        else
        {
            int k = 0,i;

            for(i = 0;i < length-2; i+=2)
            {
                num[k] = left_data*256+((int)readData[i]&255);
                qDebug()<<num[k]<<"-------10---------"<<((int)readData[i]&255)<<"--"<<((int)readData[i+1]&255);
                k++;
            }

            readData = "";
            COLUMN = length/2;
            left_data = (((int)readData[length-1])&255);
            high_data = true;
            start = false;

            else
            {
                start = true;
                int k = 0,i;
                for(i = 0;i < length-1; i+=2)
                {
                    num[k] = (int)(((uint16)readData[i])&255)*256+(int)((uint16)readData[i+1])&255;
                    k++;
                    qDebug()<<num[k]<<"-------10---------"<<((int)readData[i]&255)<<"--"<<((int)readData[i+1]&255);
                }

                readData = "";
                COLUMN = length/2;
            }
        }
    }*/

    const char *pp;
    QString str = "";

    pp = readData.constData();
    bool ok = true;
    int j = 0,k =0;
    for(int i = 0;i<readData.length();i++,pp++)
    {
        if((*pp )!= ' ')
        {
            str[j] = *pp;
            j++;
        }
        else
        {
            j = 0;
            num[k] = str.toInt(&ok,10);
            str = "";
            qDebug()<<num[k]<<"------*******";
            k++;
        }
         /*
        else
        {
           if( j ==1)
            {
                num[k] = ch[0]-48;
            }
            else if(j == 2)
            {
                num[k] = (ch[1]-48)*10+ch[0]-48;
            }
            else if(j == 3)
            {
                num[k] = (ch[2]-48)*100+(ch[1]-48)*10+ch[0]-48;
            }
            else if(j == 4)
            {
                num[k] =(ch[3]-48)*1000+(ch[2]-48)*100+(ch[1]-48)*10+ch[0]-48;
            }
            qDebug()<<num[k]<<"------*******";
            k++;
            j = 0;
        }*/
    }

    painter->setPen(QPen(Qt::cyan));
}


void showWave::on_showMathAvgBtn_clicked()
{
    flag = 1;
    showNumber();
}

void showWave::on_pauseWaveBtn_clicked()
{
    if(!display_wave_flag)
        display_wave_flag = true;
    else
        display_wave_flag = false;
}






