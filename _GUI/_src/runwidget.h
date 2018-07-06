#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include "globals.h"
#include <string>
#include <vector>
#include <ctime>
#include <QWidget>
#include <QThread>

class QTimer;
class QTabWidget;
class QTableWidget;
class SimulatorThread;
class ComputerSimulation;

class RunWidget : public QWidget {
    Q_OBJECT
public:
    explicit RunWidget(QWidget *parent = nullptr);
    ~RunWidget();
    ComputerSimulation* getSimulator();
    static void showChange(QTableWidget* table, const QBrush &defaultBrush,
                           int rowIndex, int from, int to);

private:
    ComputerSimulation* simulator;
    std::string simName = "AdHoc16_V03";
    QTimer* progressTimer;
    SimulatorThread* simThread;
    QString filename;
    std::vector<int> breakpoints;
    bool runStop;
    int lastInstructionCount = 0;
    bool simulationFinished = false;
    clock_t runStart;

    QTableWidget* executionTable;
    QStringList tableHeader;
    void addStep(uint16_t PC);
    void addNextPC();
    void createTable();

    double getMIPS(clock_t ticks);
    bool validStart();

public slots:
    void step();
    void run();
    void next();
    void reset();
    void load(QString filename);
    void setBreakpoints(std::vector<int> breakpoints);

private slots:
    void runFinished();
    void updatePerformance();

signals:
    void performanceChanged(double /* MIPS */);
    void output(QString /* Message */);
    void instructionCountChanged(int /* Instruction count */);
    void memoryChanged();
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
