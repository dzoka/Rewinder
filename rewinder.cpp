// (c) 2016,2017 Dzoka

#include "rewinder.h"

rewinder::rewinder(QObject *parent) :
    QObject(parent)
{
    counterStep = 1;
    length = 0;
    counterLength = 0;
    accLength = 0;
    loopDancerBrake = 0;
    loopDancerZero = 0;
    dm.start();
    connect(&dm, SIGNAL(loopDancerChanged_event(const int)), this, SLOT(on_loopDancer(const int)));
    connect(&dm, SIGNAL(errorEncountered_event(QString)), this, SLOT(on_errorMessage(QString)));
    connect(&dm, SIGNAL(motorWrote_event(int)), this, SLOT(on_motorWrote(int)));
    acc.dm = &dm;
    connect(&acc, SIGNAL(accelerationFinished_event()), this, SLOT(on_accelerationFinished()));
    cnt.start();
    connect(&cnt, SIGNAL(counterChangedHigh_event()), this, SLOT(on_counter()));
}

rewinder::~rewinder()
{
    if(acc.isRunning())
    {
        acc.stop();
        acc.wait();
    }
    dm.stop();
    dm.wait();
    cnt.stop();
    cnt.wait();
}

// signal from acceleration
void rewinder::on_accelerationFinished()
{
    // if accelerated, then save spooled length to calculate when decelerate
    if(acc.setSpeed > 0)
    {
        accLength = counterLength;
    }
    emit accelerationIsFinished_event();
}

// signal from device manager
void rewinder::on_motorWrote(int motor)
{
    if(motor == 0)
    {
    }
    double speed;
    speed = dm.getMotorLeader();
    QString motorSpeed = QString::number(speed);
    emit motor1changed_event(motorSpeed);
}

// signal from device manager
// update UI value
void rewinder::on_loopDancer(const int value)
{
    loopDancer = value;
    QString loopDancerString = QString::number(loopDancer);
    emit loopDancerChanged_event(loopDancerString);
    if(std::abs(loopDancer - loopDancerZero) > loopDancerBrake)
    {
        motorStop();
    }
}

// signal from device manager
// update UI value
void rewinder::on_counter()
{
    counterLength = counterLength + counterStep;
    emit counterChanged_event(QString::number(counterLength));
    if(accLength > 0 && length > 0)     // acceleration is finished and user specified length to spool
    {
        if(counterLength >= (length - accLength))
        {
            acc.setSpeed = 0;
            emit speedSettingChanged_event(QString::number(acc.setSpeed));
            length = 0;         // don't come here again
            emit lengthChanged_event(QString::number(length));
            motorStart();
        }
    }
}

void rewinder::motorStart()
{
    if(acc.isRunning())
    {
        acc.stop();             // have not finished previous incline/decline - stop it now
        acc.wait();
    }
    if(length > 0)
    {
        accLength = 0;
        resetCounter();
    }
    acc.start();                // start acceleration/deceleration
}

// emergency stop, no deceleration
void rewinder::motorStop()
{
    dm.setMotorStart(false);  // stop motors at driver level
    if(acc.isRunning())
    {
        acc.stop();
        acc.wait();
    }
    dm.setMotorSpeed(0);
}

void rewinder::setCounterStep(QString value)
{
    counterStep = value.toDouble();
}

void rewinder::resetCounter()
{
    counterLength = 0;
    emit counterChanged_event(QString::number(counterLength));
}

void rewinder::setMotorDirection(const bool value)
{
    dm.setMotorDirection(value);
}

// abs tolerance from loop dancer zero value
void rewinder::setLoopDancerBrake(QString value)
{
    loopDancerBrake = value.toInt();
}

void rewinder::setLoopDancerZero(const QString value)
{
    loopDancerZero = value.toInt();
    dm.setLoopDancerZero(loopDancerZero);
}

// signal from device manager
void rewinder::on_errorMessage(const QString message)
{
    errorMessage = "ERROR: " + message;
    emit errorEncountered_event(errorMessage, 3000);
}

// set ratio between winding drums
void rewinder::setSpeedRatio(QString value)
{
    dm.setSpeedRatio(value.toDouble());
}

// set rewinder work speed
void rewinder::setSpeed(QString value)
{
    acc.setSpeed = value.toDouble();
}

// set acceleration time in sec to reach work speed
void rewinder::setSpeedTime(QString value)
{
    acc.time = value.toInt();
}

// set length to rewind
void rewinder::setLength(QString value)
{
    length = value.toDouble();
}
