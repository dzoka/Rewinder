// (c) 2016,2017 Dzoka

#include "counter.h"

counter::counter(QObject *parent) :
    QThread(parent)
{
    doJob = false;
    wiringPiSetupGpio();
    pinMode(17,INPUT);
    pullUpDnControl(17,PUD_UP);
}

void counter::run()
{
    bool lastTime = false;
    doJob = true;
    while(doJob)
    {
        if(digitalRead(17))
        {
            if(lastTime == false)
            {
                lastTime = true;
                emit counterChangedHigh_event();
            }
        }
        else
        {
            lastTime = false;
        }
        QThread::usleep(10);
    }
}

void counter::stop()
{
    mutex.lock();
    doJob = false;
    mutex.unlock();
}
