#include <QtGui/QApplication>
#include "mainwindow.h"
#include"QTextCodec"

bool read_display_mutex;
bool clickedShowWaveBtn;
int precision_data;
bool start;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    MainWindow w;
    w.setWindowTitle("SerialPort Communication!");
    w.show();

    return a.exec();
}
