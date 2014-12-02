#ifndef HANDLEDATA_H
#define HANDLEDATA_H

#include <QThread>

class HandleData : public QThread
{
    Q_OBJECT
public:
    explicit HandleData(QObject *parent = 0);
    void run();
    void stop();

signals:
    void send_Handle_Data();

public slots:

};

#endif // HANDLEDATA_H
