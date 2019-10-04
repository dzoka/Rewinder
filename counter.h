#ifndef COUNTER_H
#define COUNTER_H

#include <QThread>
#include <QMutex>
#include <wiringPi.h>

class counter : public QThread
{
    Q_OBJECT

public:
    explicit counter(QObject *parent = 0);
    void stop();

signals:
    void counterChangedHigh_event();

protected:
    void run();

private:
    volatile bool doJob;
    QMutex mutex;

};

#endif // COUNTER_H
