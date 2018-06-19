#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include "../../_Simulator/_src/logger.h"
#include "../../_Simulator/_src/compSim.h"
#include "memorywindowwidget.h"
#include "idewidget.h"
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
#include <QFileDialog>
#include <QMenuBar>
#include <QAction>
#include <QTabWidget>

void showChange(QTableWidget* table, int rowIndex, int from, int to);

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

    bool simulationFinished = false;
    double getMIPS(clock_t ticks);
    void step();
    void run();
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
    QListWidget* stats_lst;
    void addStats(clock_t ticks); // For appending statistics to stats_lst
    // - BOTTOM
    QGroupBox* buttonBox;
    QPushButton* start_btn;
    QAction* startAction;
    QPushButton* reset_btn;
    QAction* resetAction;
    QPushButton* dump_btn;
    QAction* dumpAction;
    void createSidePanel();

    /* TAB WIDGET */
    QTabWidget* tabs;
    QTableWidget* table;
    QStringList tableHeader;
    IdeWidget* ide;

    void addStep(word PC);
    void createTabs();

    /* MEMORY DUMP WINDOW */
    MemoryWindowWidget* memoryWindow;

private slots:
    void startSim();
    void resetSim();
    void openFile();
    void newFile();
    void memoryDump();
    void setButtonText(int currentIndex);
    void updateProgramLabel(QString filename);
    void runFromShortCut();

signals:

public slots:
};

#endif // RUNWIDGET_H
