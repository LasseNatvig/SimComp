#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include "../../_Simulator/_src/logger.h"
#include "../../_Simulator/_src/compSim.h"
#include "globals.h"
#include <string>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QListWidget>

void showChange(QTableWidget* table, int rowIndex, int from, int to);

class runWidget : public QWidget
{
    Q_OBJECT
public:
    explicit runWidget(QWidget *parent = nullptr);
    void setFilename(QString filename);

private:
    /* Core variables and functions */
    ComputerSimulation* simulator;
    QString filename;
    std::string simName = "AdHoc16_V03";
    double getMIPS(clock_t ticks);


    /* CHOOSE PROGRAM */
    QGroupBox* programBox;
    QLabel* program_lbl;
    QPushButton* open_btn;

    /* SIDE PANEL: */
    // - TOP
    QGroupBox* dropdownMenuBox;
    QLabel* select_mode_lbl;
    QComboBox* dropdownMenu;
    // - MIDDLE
    QLabel* description_lbl;
    QLabel* icon_img;
    QListWidget* stats_lst;
    void appendStats(clock_t ticks); // For appending statistics to stats_lst
    // - BOTTOM
    QGroupBox* buttonBox;
    QPushButton* start_btn;
    QPushButton* end_btn;

    /* TABLE */
    QTableWidget* table;
    QStringList tableHeader;


private slots:
    void startSim();
    void step();
    void endSim();
    void setButtonText(int currentIndex);
    void openFile();

signals:

public slots:
};

#endif // RUNWIDGET_H
