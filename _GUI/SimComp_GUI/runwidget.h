#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include "../../_Simulator/_src/logger.h"
#include "../../_Simulator/_src/utils.h"
#include "../../_Simulator/_src/compSim.h"
#include <string>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

class runWidget : public QWidget
{
    Q_OBJECT
public:
    explicit runWidget(QWidget *parent = nullptr, QString filename = "");

private:
    // Core variables and functions
    ComputerSimulation* simulator;
    QString filename;
    std::string simName = "AdHoc16_V03";
    double getMIPS(clock_t ticks);
    void startSingleStepMode();

    // Labels
    QLabel* description_lbl;
    QLabel* filename_lbl;

    // Action objects
    QComboBox* dropdownMenu;
    QPushButton* start_btn;


private slots:
    void startSim();
    void nextStep();

signals:

public slots:
};

#endif // RUNWIDGET_H
