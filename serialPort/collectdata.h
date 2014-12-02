#ifndef COLLECTDATA_H
#define COLLECTDATA_H

#include <QThread>

class CollectData : public QThread
{
    Q_OBJECT
public:
    explicit CollectData(QObject *parent = 0);
    void run();
    void stop();

signals:
    void coleectDataNow();

public slots:

};

#endif // COLLECTDATA_H
