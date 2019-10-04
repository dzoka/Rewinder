// (c) 2016 Dzoka

#include "acceleration.h"

acceleration::acceleration(QObject *parent) :
    QThread(parent)
{
}

void acceleration::stop()
{
    doJob = false;
}

// accelearate / decelerate motor from current value to setSpeed
void acceleration::run()
{
    currentSpeed = dm->getMotorLeader();
    accelerate = setSpeed > currentSpeed ? true : false;
    if(accelerate)
    {
        step = (setSpeed - currentSpeed) / time;
        dm->setMotorStart(true);                // start the motor
    }
    else
    {
        step = (currentSpeed - setSpeed) / time;
    }
    doJob = true;
    while(doJob)
    {
        if(accelerate)
        {
            currentSpeed += step;               // accelerate
            if(currentSpeed > setSpeed)
            {
                currentSpeed = setSpeed;
            }
        }
        else
        {
            currentSpeed -= step;               // decelerate
            if(currentSpeed < setSpeed)
            {
                currentSpeed = setSpeed;
            }
        }
        dm->setMotorSpeed(currentSpeed);
        if(currentSpeed == setSpeed)
        {
            doJob = false;                      // done
        }
        else
        {
            QThread::sleep(1);
        }
    }
    if(currentSpeed == 0)
    {
        dm->setMotorStart(false);       // stop the motor
    }
    emit accelerationFinished_event();
}
