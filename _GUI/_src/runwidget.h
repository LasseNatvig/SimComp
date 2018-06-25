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

QT_CHARTS_USE_NAMESPACE

void showChange(QTableWidget* table, int rowIndex, int from, int to);

class SimulatorThread;

class RunWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RunWidget(QWidget *parent = nullptr);
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
    void step();
    void run();
    void next();
    void load();

    /* MENU BAR */
    QMenuBar* menuBar;
    QMenu* fileMenu;
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QMenu* editMenu;
    QAction* undoAction;
    QAction* redoAction;
    QMenu* buildMenu;
    QAction* runAction;
    QAction* resetAction;
    QAction* stepAction;
    QAction* nextAction;
    QMenu* viewMenu;
    QAction* memoryAction;
    QAction* performanceAction;
    void createMenuBar();

    /* PROGRAM INFO */
    QGroupBox* programBox;
    QLabel* program_lbl;
    void createProgramInfo();

    /* SIDE PANEL: */
    // - TOP
    QGroupBox* dropdownMenuBox;
    QLabel* select_mode_lbl;
    QComboBox* dropdownMenu;
    // - MIDDLE
    QLabel* description_lbl;
    QLabel* icon_img;
    QListWidget* info_lst;
    void addRunStats(clock_t ticks); // For appending statistics to info_lst
    // - BOTTOM
    QGroupBox* buttonBox;
    QPushButton* start_btn;
    QPushButton* reset_btn;
    void createSidePanel();

    /* TAB WIDGET */
    QTabWidget* tabs;
    QTableWidget* table;
    QStringList tableHeader;
    IdeWidget* ide;
    void addStep(word PC);
    void createTabs();

    /* WINDOWS */
    MemoryWindowWidget* memoryWindow;
    PerformanceChart* performanceChart;
    QChartView* performanceChartView;
    QAction* closePerformanceAction;
    void createWindows();

    /* UTILS */
    double getMIPS(clock_t ticks);
private slots:
    void reset();
    void openFile();
    void newFile();
    void openMemoryWindow();
    void openPreformanceWindow();
    void setButtonText(int currentIndex);
    void updateFilename(QString filename);
    void runFromShortCut();
    void stepFromShortCut();
    void nextFromShortCut();
    void runFinished();
    void updatePerformance();
    void writeInfo(QString info);

signals:

public slots:
    void startSim();
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
