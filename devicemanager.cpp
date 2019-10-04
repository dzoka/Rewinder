// (c) 2016 Dzoka

#include "devicemanager.h"

devicemanager::devicemanager(QObject *parent) :
    QThread(parent)
{
    currentMotor1 = 0;
    currentMotor2 = 0;
    kp = 0;
    ki = 0;
    kd = 0;
    iTerm = 0;
    iTermCounter = 0;
    dTerm = 0;
    loopDancer = 0;
    loopDancerZero = 0;
    loopDancerLast = 0;
    motorRatio = 1;
    motorToSpeed = 1;
    outMin = -40;
    outMax = 40;
    pidTime = 500;
    pidReset = 5;
    pidDirection = false;
    // set some initial values on M-7002 and M-7024
    motor1 = 0;
    writeMotor1 = true;
    motor2 = 0;
    writeMotor2 = true;
    motorStart = false;
    writeMotorStart = true;
    motorForward = false;
    writeMotorForward = true;
    lastTime = QDateTime::currentDateTimeUtc();
}

double devicemanager::getMotorLeader() const
{
    if(motorForward)
    {
        return double(currentMotor1 * motorToSpeed);
    }
    else
    {
        return double(currentMotor2 * motorToSpeed);
    }
}

void devicemanager::run()
{
    u_int16_t loopDancerNew = 0;
    modbus_t *ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1);

    if(ctx == NULL)
    {
        emit errorEncountered_event(tr("unable create Modbus context"));
        return;
    }

    if(modbus_connect(ctx) == -1)
    {
        modbus_free (ctx);
        emit errorEncountered_event(tr("connection failed"));
        return;
    }

    doJob = true;
    while(doJob)
    {
        modbus_set_slave(ctx, 3);           // Vin0 = Loop dancer, device addr = 3
        QThread::msleep(5);
        if(modbus_read_input_registers(ctx, 0, 1, &loopDancerNew) == -1)
        {
            emit errorEncountered_event(tr("reading Loop dancer"));
            loopDancerNew = loopDancer;     // error reading - do not emit signal
        }
        if(loopDancer != loopDancerNew)
        {
            loopDancer = loopDancerNew;
            emit loopDancerChanged_event(loopDancer);
        }

        computePid();

        if(writeMotor1)
        {
            modbus_set_slave(ctx, 2);       // Vout0 = Motor1, device addr = 2
            QThread::msleep(5);
            if(modbus_write_register(ctx, 0, motor1) == -1)
            {
                emit errorEncountered_event(tr("writing Motor1"));
            }
            else
            {
                writeMotor1 = false;        // new value is written
                currentMotor1 = motor1;     // set as current value
                emit motorWrote_event(0);
            }
        }

        if(writeMotor2)
        {
            modbus_set_slave(ctx, 2);       // Vout1 = Motor2, device addr = 2
            QThread::msleep(5);
            if(modbus_write_register(ctx, 1, motor2) == -1)
            {
                emit errorEncountered_event(tr("writing Motor2"));
            }
            else
            {
                writeMotor2 = false;        // new value is written
                currentMotor2 = motor2;     // set as current value
                emit motorWrote_event(1);
            }
        }

        if(writeMotorForward)
        {
            modbus_set_slave(ctx, 3);   // RL2 = Forward/Reverse, device addr = 3
            QThread::msleep(5);
            if(modbus_write_bit(ctx, 2, motorForward) == -1)
            {
                emit errorEncountered_event(tr("setting Forward/Reverse"));
            }
            else
            {
                writeMotorForward = false;  // new value is set
            }
        }

        if(writeMotorStart)
        {
            modbus_set_slave(ctx, 3);       // RL0 = Start/Stop, device addr = 3
            QThread::msleep(5);
            if(modbus_write_bit(ctx, 0, motorStart) == -1)
            {
                emit errorEncountered_event(tr("setting Start/Stop"));
            }
            else
            {
                writeMotorStart = false;    // new value is set
            }
        }
    }

    modbus_close(ctx);
    modbus_free(ctx);
}

void devicemanager::stop()
{
    mutex.lock();
    doJob = false;
    mutex.unlock();
}

void devicemanager::setMotorSpeed(double value)
{
    mutex.lock();
    if(motorForward)
    {
        motor1 = value / motorToSpeed;    // m1 is leader
        motor2 = motor1 * motorRatio;
    }
    else
    {
        motor2 = value / motorToSpeed;    // otherwise m2 is leader
        motor1 = motor2 * motorRatio;
    }
    writeMotor1 = writeMotor2 = true;
    iTerm = 0;
    mutex.unlock();
}

void devicemanager::setMotorStart(bool value)
{
    mutex.lock();
    motorStart = value;
    writeMotorStart = true;
    mutex.unlock();
}

void devicemanager::setMotorDirection(bool value)
{
    mutex.lock();
    motorForward = value;
    writeMotorForward = true;
    mutex.unlock();
}

// adopted from
// https://github.com/br3ttb/Arduino-PID-Library/blob/master/PID_v1.cpp
void devicemanager::computePid()
{
    QDateTime currentTime = QDateTime::currentDateTimeUtc();
    if(lastTime.msecsTo(currentTime) < pidTime)
    {
        return;
    }
    else
    {
        lastTime = currentTime;
    }
    double error = loopDancerZero - loopDancer;
    if(iTermCounter > pidReset)
    {
        iTerm = 0;
        iTermCounter = 0;
    }
    else
    {
        iTermCounter++;
    }
    iTerm += (ki * error);
    double dInput = (loopDancer - loopDancerLast);
    double output = kp * error + iTerm - kd * dInput;

    if(output > outMax) output = outMax;
    else if(output < outMin) output = outMin;

    if(pidDirection) output *= -1;

    if(!writeMotor1 || !writeMotor2)
    {
        if(motorForward)                    // adjust follower
        {
            if(currentMotor1 > 0)
            {
                motor2 += output;           // m2 is follower
            }
            else
            {
                motor2 = 0;                 // stop if leader is stopped
            }
            writeMotor2 = true;
        }
        else
        {
            if(currentMotor2 > 0)
            {
                motor1 += output;           // otherwise m1 is follower
            }
            else
            {
                motor1 = 0;                 // stop if leader is stopped
            }
            writeMotor1 = true;
        }
    }

    loopDancerLast = loopDancer;
}
