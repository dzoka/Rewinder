#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <keyemitter.h>
#include <rewinder.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonKey0_clicked();
    void on_pushButtonKey1_clicked();
    void on_pushButtonKey2_clicked();
    void on_pushButtonKey3_clicked();
    void on_pushButtonKey4_clicked();
    void on_pushButtonKey5_clicked();
    void on_pushButtonKey6_clicked();
    void on_pushButtonKey7_clicked();
    void on_pushButtonKey8_clicked();
    void on_pushButtonKey9_clicked();
    void on_pushButtonKeyBckSp_clicked();
    void on_pushButtonKeyDot_clicked();
    void on_pushButtonKeyEnter_clicked();
    void on_pushButtonDirection_clicked();
    void on_pushButtonExit_clicked();
    void on_lineEditSpeedRatio_editingFinished();
    void on_lineEditSpeed_editingFinished();
    void on_lineEditSpeedTime_editingFinished();
    void on_lineEditLoopDancerBrake_editingFinished();
    void on_lineEditLoopDancerZero_editingFinished();
    void on_lineEditKp_editingFinished();
    void on_lineEditKi_editingFinished();
    void on_lineEditKd_editingFinished();
    void on_checkBoxPidDirection_toggled(bool checked);
    void on_lineEditPidReset_editingFinished();
    void on_lineEditPidTime_editingFinished();
    void on_lineEditPidOutMin_editingFinished();
    void on_lineEditPidOutMax_editingFinished();
    void on_accelerationIsFinished();
    void on_pushButtonTimeUp_clicked();
    void on_pushButtonTimeDown_clicked();
    void on_pushButtonGo_clicked();
    void on_pushButtonStop_clicked();
    void on_radioButton1_clicked();
    void on_radioButton2_clicked();
    void on_radioButton3_clicked();
    void on_pushButtonCounterReset_clicked();
    void on_lineEditCounterStep_editingFinished();
    void on_lineEditLength_editingFinished();
    void on_pushButtonSave_clicked();

private:
    Ui::MainWindow *ui;
    KeyEmitter keyEmitter;
    rewinder rw;
    void saveSettings();
};

#endif // MAINWINDOW_H
