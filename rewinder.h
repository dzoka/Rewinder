#ifndef REWINDER_H
#define REWINDER_H

#include <QObject>
#include <QString>
#include <devicemanager.h>
#include <acceleration.h>
#include <counter.h>

class rewinder : public QObject
{
    Q_OBJECT

public:
    explicit rewinder(QObject *parent = 0);
    ~rewinder();
    void motorStart();
    void motorStop();
    void setMotorDirection(const bool value);
    void setCounterStep(QString value);
    void setLength(QString value);

public slots:
    void setDiameterRatio(const QString value) { dm.setMotorRatio(value.toDouble()); }
    void setKp(const QString value) { dm.setKp(value.toDouble()); }
    void setKi(const QString value) { dm.setKi(value.toDouble()); }
    void setKd(const QString value) { dm.setKd(value.toDouble()); }
    void setLoopDancerBrake(const QString value);
    void setLoopDancerZero(const QString value);
    void setPidDirection(const bool value) { dm.setPidDirection(value); }
    void setPidTime(const QString value) { dm.setPidTime(value.toInt()); }
    void setPidOutMin(const QString value) { dm.setPidOutMin(value.toInt()); }
    void setPidOutMax(const QString value) { dm.setPidOutMax(value.toInt()); }
    void setPidReset(const QString value) { dm.setPidReset(value.toInt()); }
    void setSpeedRatio(const QString value);
    void setSpeed(const QString value);
    void setSpeedTime(const QString value);
    void resetCounter();

protected slots:
    void on_accelerationFinished();
    void on_loopDancer(const int value);
    void on_counter();
    void on_errorMessage(const QString message);
    void on_motorWrote(int m);

signals:
    void accelerationIsFinished_event();
    void counterChanged_event(const QString value);
    void errorEncountered_event(const QString message, const int time);
    void loopDancerChanged_event(const QString value);
    void motor1changed_event(const QString value);
    void speedSettingChanged_event(const QString value);
    void lengthChanged_event(const QString value);

private:
    acceleration acc;
    counter cnt;
    double counterStep;
    double counterLength;
    double length;              // length to rewind
    double accLength;           // length on spool after acceleration
    int loopDancer;
    int loopDancerBrake;
    int loopDancerZero;
    devicemanager dm;
    QString errorMessage;
};

#endif // REWINDER_H
