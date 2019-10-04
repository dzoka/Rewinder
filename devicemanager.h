#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QThread>
#include <QMutex>
#include <modbus/modbus.h>
#include <modbus/modbus-rtu.h>
#include <QDateTime>

class devicemanager : public QThread
{
    Q_OBJECT

public:
    explicit devicemanager(QObject *parent = 0);
    int getLoopDancer() const { return loopDancer; }
    double getMotorLeader() const;
    void setMotorRatio(const double value) { motorRatio = value; }
    void setKp(const double value) { kp = value; }
    void setKi(const double value) { ki = value; }
    void setKd(const double value) { kd = value; }
    void setLoopDancerZero(const int value) { loopDancerZero = value; }
    void setMotorDirection(const bool value);
    void setMotorSpeed(const double value);
    void setMotorStart(const bool value);
    void setPidDirection(const bool value) { pidDirection = value; }
    void setPidTime(const int value) { pidTime = value; }
    void setPidOutMin(const int value) { outMin = value; }
    void setPidOutMax(const int value) { outMax = value; }
    void setPidReset(const int value) { pidReset = value; }
    void setSpeedRatio(const double value) { motorToSpeed = value; }
    void stop();

signals:
    void errorEncountered_event(const QString message);
    void loopDancerChanged_event(const int value);
    void motorWrote_event(int m);

protected:
    void run();

private:
    void computePid();
    int currentMotor1;                  // current voltage on motor1
    int currentMotor2;                  // current voltage on motor2
    volatile bool doJob;                // thread loop
    QDateTime lastTime;                 // used by PID
    double kp;                          // PID proportional
    double ki;                          // PID integral
    double kd;                          // PID derivative
    double iTerm;
    int iTermCounter;                   // when to reset iTerm
    double dTerm;
    int loopDancer;                     // Vin0 on M-7002, device addr = 3
    int loopDancerZero;                 // Set Point for PID
    int loopDancerLast;                 // uses PID
    int motor1;                         // Vout0 on M-7024, device addr = 2
    int motor2;                         // Vout1 on M-7024, device addr = 2
    double motorRatio;                  // motor rotation ratio
    double motorToSpeed;                // speed ratio
    bool motorStart;                    // RL0 on M-7002, device addr = 3
    bool motorForward;                  // RL2 on M-7002, device addr = 3
    QMutex mutex;
    int outMin;                         // PID min
    int outMax;                         // PID max
    int pidTime;
    int pidReset;
    bool pidDirection;
    bool writeMotor1;                   // value changed, write new Motor1 value
    bool writeMotor2;                   // value changed, write new Motor2 value
    bool writeMotorForward;             // value changed, set new flag
    bool writeMotorStart;               // value changed, set new flag
};

#endif // DEVICEMANAGER_H
