// (c) 2016,2017 Dzoka

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&rw, SIGNAL(loopDancerChanged_event(const QString)), ui->lineEditLoopDancer, SLOT(setText(const QString)));
    connect(&rw, SIGNAL(counterChanged_event(const QString)), ui->lineEditCounter, SLOT(setText(const QString)));
    connect(&rw, SIGNAL(errorEncountered_event(const QString, const int)), ui->statusBar, SLOT(showMessage(QString, int)));
    connect(&rw, SIGNAL(motor1changed_event(const QString)), ui->lineEditCurrentSpeed, SLOT(setText(const QString)));
    connect(&rw, SIGNAL(accelerationIsFinished_event()), this, SLOT(on_accelerationIsFinished()));
    connect(&rw, SIGNAL(speedSettingChanged_event(const QString)), ui->lineEditSpeed, SLOT(setText(const QString)));
    connect(&rw, SIGNAL(lengthChanged_event(const QString)), ui->lineEditLength, SLOT(setText(const QString)));

    QFile file("Rewinder.conf");
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            while(!in.atEnd())
            {
                QString line = in.readLine(50);
                QStringList list = line.split("=");
                if(list[0] == "kp")
                {
                    ui->lineEditKp->setText(list[1]);
                    rw.setKp(ui->lineEditKp->text());
                }
                else if(list[0] == "ki")
                {
                    ui->lineEditKi->setText(list[1]);
                    rw.setKi(ui->lineEditKi->text());
                }
                else if(list[0] == "kd")
                {
                    ui->lineEditKd->setText(list[1]);
                    rw.setKd(ui->lineEditKd->text());
                }
                else if(list[0] == "pidtime")
                {
                    ui->lineEditPidTime->setText(list[1]);
                    rw.setPidTime(ui->lineEditPidTime->text());
                }
                else if(list[0] == "pidoutmin")
                {
                    ui->lineEditPidOutMin->setText(list[1]);
                    rw.setPidOutMin(ui->lineEditPidOutMin->text());
                }
                else if(list[0] == "pidoutmax")
                {
                    ui->lineEditPidOutMax->setText(list[1]);
                    rw.setPidOutMax(ui->lineEditPidOutMax->text());
                }
                else if(list[0] == "pidreset")
                {
                    ui->lineEditPidReset->setText(list[1]);
                    rw.setPidReset(ui->lineEditPidReset->text());
                }
                else if(list[0] == "piddirection")
                {
                    if(list[1] == "1")
                    {
                        ui->checkBoxPidDirection->setChecked(true);
                        rw.setPidDirection(true);
                    }
                }
                else if(list[0] == "speedratio")
                {
                    ui->lineEditSpeedRatio->setText(list[1]);
                    rw.setSpeedRatio(ui->lineEditSpeedRatio->text());
                }
                else if(list[0] == "speedtime")
                {
                    ui->lineEditSpeedTime->setText(list[1]);
                    rw.setSpeedTime(ui->lineEditSpeedTime->text());
                }
                else if(list[0] == "loopdancerbrake")
                {
                    ui->lineEditLoopDancerBrake->setText(list[1]);
                    rw.setLoopDancerBrake(ui->lineEditLoopDancerBrake->text());
                }
                else if(list[0] == "counterstep")
                {
                    ui->lineEditCounterStep->setText(list[1]);
                    rw.setCounterStep(ui->lineEditCounterStep->text());
                }
            }
            file.close();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_accelerationIsFinished()
{
    ui->lineEditSpeed->setEnabled(true);
    ui->pushButtonGo->setEnabled(true);
    ui->pushButtonDirection->setEnabled(true);
    ui->lineEditSpeedRatio->setEnabled(true);
    ui->lineEditSpeed->setEnabled(true);
    ui->lineEditSpeedTime->setEnabled(true);
    ui->lineEditLoopDancerBrake->setEnabled(true);
}

void MainWindow::on_pushButtonKey0_clicked()
{
    keyEmitter.emitKey(Qt::Key_0);
}

void MainWindow::on_pushButtonKey1_clicked()
{
    keyEmitter.emitKey(Qt::Key_1);
}

void MainWindow::on_pushButtonKey2_clicked()
{
    keyEmitter.emitKey(Qt::Key_2);
}

void MainWindow::on_pushButtonKey3_clicked()
{
    keyEmitter.emitKey(Qt::Key_3);
}

void MainWindow::on_pushButtonKey4_clicked()
{
    keyEmitter.emitKey(Qt::Key_4);
}

void MainWindow::on_pushButtonKey5_clicked()
{
    keyEmitter.emitKey(Qt::Key_5);
}

void MainWindow::on_pushButtonKey6_clicked()
{
    keyEmitter.emitKey(Qt::Key_6);
}

void MainWindow::on_pushButtonKey7_clicked()
{
    keyEmitter.emitKey(Qt::Key_7);
}

void MainWindow::on_pushButtonKey8_clicked()
{
    keyEmitter.emitKey(Qt::Key_8);
}

void MainWindow::on_pushButtonKey9_clicked()
{
    keyEmitter.emitKey(Qt::Key_9);
}

void MainWindow::on_pushButtonKeyBckSp_clicked()
{
    keyEmitter.emitKey(Qt::Key_Backspace);
}

void MainWindow::on_pushButtonKeyDot_clicked()
{
    keyEmitter.emitKey(Qt::Key_Period);
}

void MainWindow::on_pushButtonKeyEnter_clicked()
{
    keyEmitter.emitKey(Qt::Key_Enter);
}

void MainWindow::on_pushButtonGo_clicked()
{
    //if(rw.acc.current == 0)
    {
        // set current Loop Dancer value as zero value
        ui->lineEditLoopDancerZero->setText(ui->lineEditLoopDancer->text());
        rw.setLoopDancerZero(ui->lineEditLoopDancerZero->text());
    }
    rw.motorStart();
    ui->pushButtonGo->setEnabled(false);
    ui->pushButtonDirection->setEnabled(false);
    ui->lineEditSpeedRatio->setEnabled(false);
    ui->lineEditSpeed->setEnabled(false);
    ui->lineEditSpeedTime->setEnabled(false);
    ui->lineEditLoopDancerBrake->setEnabled(false);
}

void MainWindow::on_pushButtonDirection_clicked()
{
    if(ui->pushButtonDirection->text() == "Right")
    {
        rw.setMotorDirection(true);
        ui->pushButtonDirection->setText("Left");
    }
    else
    {
        rw.setMotorDirection(false);
        ui->pushButtonDirection->setText("Right");
    }
}

void MainWindow::on_pushButtonExit_clicked()
{
    saveSettings();
    this->close();
}

void MainWindow::saveSettings()
{
    QFile file("Rewinder.conf");
    if(file.exists())
    {
    }
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << "kp=" << ui->lineEditKp->text() << "\n";
        out << "ki=" << ui->lineEditKi->text() << "\n";
        out << "kd=" << ui->lineEditKd->text() << "\n";
        out << "pidtime=" << ui->lineEditPidTime->text() << "\n";
        out << "pidoutmin=" << ui->lineEditPidOutMin->text() << "\n";
        out << "pidoutmax=" << ui->lineEditPidOutMax->text() << "\n";
        out << "pidreset=" << ui->lineEditPidReset->text() << "\n";
        if(ui->checkBoxPidDirection->isChecked())
        {
            out << "piddirection=" << "1" << "\n";
        }
        else
        {
            out << "piddirection=" << "0" << "\n";
        }
        out << "speedratio=" << ui->lineEditSpeedRatio->text() << "\n";
        out << "speedtime=" << ui->lineEditSpeedTime->text() << "\n";
        out << "loopdancerbrake=" << ui->lineEditLoopDancerBrake->text() << "\n";
        out << "counterstep=" << ui->lineEditCounterStep->text() << "\n";
        file.flush();
        file.close();
    }
}

void MainWindow::on_lineEditSpeedRatio_editingFinished()
{
    rw.setSpeedRatio(ui->lineEditSpeedRatio->text());
}

void MainWindow::on_lineEditSpeed_editingFinished()
{
    rw.setSpeed(ui->lineEditSpeed->text());
}

void MainWindow::on_lineEditSpeedTime_editingFinished()
{
    rw.setSpeedTime(ui->lineEditSpeedTime->text());
}

void MainWindow::on_lineEditLoopDancerBrake_editingFinished()
{
    rw.setLoopDancerBrake(ui->lineEditLoopDancerBrake->text());
}

void MainWindow::on_lineEditLoopDancerZero_editingFinished()
{
    rw.setLoopDancerZero(ui->lineEditLoopDancerZero->text());
}

void MainWindow::on_lineEditKp_editingFinished()
{
    rw.setKp(ui->lineEditKp->text());
}

void MainWindow::on_lineEditKi_editingFinished()
{
    rw.setKi(ui->lineEditKi->text());
}

void MainWindow::on_lineEditKd_editingFinished()
{
    rw.setKd(ui->lineEditKd->text());
}

void MainWindow::on_checkBoxPidDirection_toggled(bool checked)
{
    rw.setPidDirection(checked);
}

void MainWindow::on_lineEditPidReset_editingFinished()
{
    rw.setPidReset(ui->lineEditPidReset->text());
}

void MainWindow::on_lineEditPidTime_editingFinished()
{
    rw.setPidTime(ui->lineEditPidTime->text());
}

void MainWindow::on_lineEditPidOutMin_editingFinished()
{
    rw.setPidOutMin(ui->lineEditPidOutMin->text());
}

void MainWindow::on_lineEditPidOutMax_editingFinished()
{
    rw.setPidOutMax(ui->lineEditPidOutMax->text());
}

void MainWindow::on_pushButtonTimeUp_clicked()
{
    int t = ui->lineEditSpeedTime->text().toInt();
    if(t < 20)
    {
        ui->lineEditSpeedTime->setText(QString::number(++t));
        rw.setSpeedTime(ui->lineEditSpeedTime->text());
    }
}

void MainWindow::on_pushButtonTimeDown_clicked()
{
    int t = ui->lineEditSpeedTime->text().toInt();
    if(t > 0)
    {
        ui->lineEditSpeedTime->setText(QString::number(--t));
        rw.setSpeedTime(ui->lineEditSpeedTime->text());
    }
}

void MainWindow::on_pushButtonStop_clicked()
{
    rw.motorStop();
}

void MainWindow::on_radioButton1_clicked()
{
    rw.setDiameterRatio(ui->radioButton1->text());
}

void MainWindow::on_radioButton2_clicked()
{
    rw.setDiameterRatio(ui->radioButton2->text());
}

void MainWindow::on_radioButton3_clicked()
{
    rw.setDiameterRatio(ui->radioButton3->text());
}

void MainWindow::on_pushButtonCounterReset_clicked()
{
    rw.resetCounter();
}

void MainWindow::on_lineEditCounterStep_editingFinished()
{
    rw.setCounterStep(ui->lineEditCounterStep->text());
}

void MainWindow::on_lineEditLength_editingFinished()
{
    rw.setLength(ui->lineEditLength->text());
}

void MainWindow::on_pushButtonSave_clicked()
{
    saveSettings();
}
