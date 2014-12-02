#ifndef SHOWWAVE_H
#define SHOWWAVE_H

#include <QMainWindow>
#include"QList"
#include"QPainter"
#include"QTimer"
#include"QTimerEvent"
#include"QPaintEvent"
#include"QByteArray"
#include "win_qextserialport.h"
#include"QCloseEvent"

#define DATA_ARRAY_SIZE 1000

typedef unsigned short uint16;

namespace Ui {
    class showWave;
}

class showWave : public QMainWindow
{
    Q_OBJECT

public:
    explicit showWave(QWidget *parent = 0);
    ~showWave();

private:
    Ui::showWave *ui;

public:
    void paintEvent(QPaintEvent *e);
    void processEightData();
    void processTenOrTwelveData();

private:
    QPainter *painter;
    int COLUMN;

    QPen m_scaleLinePen;
    QPen m_waveDataPen;

    QRect rect;
    bool flag1;
    QPoint point[2];
    int height,width;
    QPoint initPoint;
    int timerId;
    qreal x;

    unsigned int gridnum;
    int *pointt;
    QString strSelectFileData;
    int flag;
    int N;
    int VAL;
    bool flag_load;
    bool display_wave_flag;
    int *num;

    int m_pen_width;
    int m_other_pen_width;
    int VALY;
    int RECT_HEIGHT;
    int *sum1,*sum2,*sum3,*sum4;
    //int SEEK_POS;
    int  left_data;
    bool high_data;
    bool start;

    QTimer *time;

public:


public:
    QByteArray temp;
    Win_QextSerialPort *myCom;  //声明对象

    showWave(qreal width, qreal height, QObject * parent = 0);
    void setXYCord();
    void showGrid();
    void showSinWave();
    void mathAvg();
    void slideAvg();
    void midAvg();
    void addWeightAvg();

    void setDefaultScaleLinePen();
    void setDefaultWaveDataPen();
    void setDefaultScope();
    void RealTimeWave();

public slots:
    void drawGridBtn();
    void gridSpin();
    void saveShowData(int *point);
    void loadFile();
    void showNumber();

    //void timerEvent(QTimerEvent *e);
    void closeEvent(QCloseEvent *e);
    void processShareFile();
    void test();

private slots:
    void on_showMathAvgBtn_clicked();
    void on_otherPenWidthSpin_valueChanged(int );
    void on_penWidthSpin_valueChanged(int );
    void on_showNormalBtn_clicked();
    void on_interalPixelsSpin_valueChanged(int );
    void on_pushButton_clicked();
    void on_midAvgBtn_clicked();
    void on_slideBtn_clicked();
    void on_NValueSpinBox_valueChanged(int );
    void on_pauseWaveBtn_clicked();
};

#endif // SHOWWAVE_H
