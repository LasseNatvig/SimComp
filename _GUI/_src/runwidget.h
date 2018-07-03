#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include "globals.h"
#include <string>
#include <ctime>
#include <QWidget>
#include <QThread>

class QTimer;
class QTabWidget;
class QAction;
class QGroupBox;
class QTableWidget;
class QPushButton;
class QLabel;
class SimulatorThread;
class ComputerSimulation;
class IdeWidget;
class MemoryWindowWidget;
class PerformanceChart;

class RunWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RunWidget(QWidget *parent = nullptr);
    ComputerSimulation* getSimulator();
    static void showChange(QTableWidget* table, const QBrush &defaultBrush,
                           int rowIndex, int from, int to);

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
    void addStep(uint16_t PC);
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
    void openFileDialog();
    void open(QString filename);
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
    void filenameChanged(QString filename);
    void output(QString message);
    void instructionCountChanged(int instructionCount);
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
