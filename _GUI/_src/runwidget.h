#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include "../../_Simulator/_src/logger.h"
#include "../../_Simulator/_src/compSim.h"
#include "memorywindowwidget.h"
#include "idewidget.h"
#include "performancechart.h"
#include "globals.h"
#include <string>
#include <ctime>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QListWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QAction>
#include <QTabWidget>
#include <QThread>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QToolBar>

QT_CHARTS_USE_NAMESPACE

void showChange(QTableWidget* table, int rowIndex, int from, int to);

class SimulatorThread;

class RunWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RunWidget(QWidget *parent = nullptr);
    ComputerSimulation* getSimulator();
private:
    /* Core variables and functions */
    ComputerSimulation* simulator;
    QString filename;
    std::string simName = "AdHoc16_V03";
    QTimer* progressTimer;
    SimulatorThread* simThread;

    bool runStop;
    int lastInstructionCount = 0;
    bool simulationFinished = false;
    clock_t runStart;
    void load();

    /* TAB WIDGET */
    QTabWidget* tabs;
    QTableWidget* executionTable;
    QStringList tableHeader;
    void addStep(word PC);
    void addNextPC();
    IdeWidget* ide;
    void createTabs();



    /* UTILS */
    double getMIPS(clock_t ticks);
    bool validStart();

public slots:
    /* SIMULATOR SLOTS */
    void step();
    void run();
    void next();
    void reset();

    /* EDITOR SLOTS */
    void openFile();
    void newFile();
    void save();
    void saveAs();
    void undo();
    void redo();

private slots:
    void updateFilename(QString filename);
    void runFinished();
    void updatePerformance();

signals:
    void performanceChanged(double MIPS);
    void output(QString message);
    void instructionCountChanged(int instructionCount);
};


class SimulatorThread : public QThread {
    Q_OBJECT

public:
    SimulatorThread(ComputerSimulation* simulator);
    ~SimulatorThread();
    void run() override;

private:
    ComputerSimulation* simulator;
};

#endif // RUNWIDGET_H
