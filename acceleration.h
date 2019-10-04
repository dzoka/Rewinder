#ifndef ACCELERATION_H
#define ACCELERATION_H

#include <QThread>
#include <devicemanager.h>

class acceleration : public QThread
{
    Q_OBJECT

public:
    explicit acceleration(QObject *parent = 0);
    void stop();
    devicemanager *dm;
    double setSpeed;
    int time;

signals:
    void accelerationFinished_event();

public slots:

protected:
    void run();

private:
    bool accelerate;
    volatile bool doJob;
    double currentSpeed;
    double step;
};

#endif // ACCELERATION_H
