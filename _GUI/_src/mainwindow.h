#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "runwidget.h"
#include "memorywindowwidget.h"
#include "performancechart.h"
#include "globals.h"
#include <QMainWindow>
#include <QIcon>
#include <QSettings>
#include <QToolBar>
#include <QStatusBar>
#include <QVector>
#include <QPair>

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // Icon
    QIcon* appIcon;

    // Central widget
    RunWidget* runW;

    // Menu bar
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
    QAction* actionAction;
    QMenu* memoryMenu;
    void createMenuBar();
    void createStatusBar();

    // Action dock
    QGroupBox* actionToolBox;
    QPushButton* runBtn;
    QPushButton* stepBtn;
    QPushButton* nextBtn;
    QPushButton* resetBtn;
    QListWidget* outputLst;
    QAction* closePerformanceAction;
    void createActionDock();

    // Performance Dock
    PerformanceChart* performanceChart;
    QChartView* performanceChartView;
    void createPerformanceDock();

    // Memory Windows
    QVector<QPair<MemoryWindowWidget*,QAction*>> memoryWindows;

public slots:
    void newMemoryWindow();
    void reset();
    void writeOutput(QString message);
    void updateInstructionCount(int instructionCount);
};

#endif // MAINWINDOW_H
