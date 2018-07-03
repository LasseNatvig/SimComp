#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QList>
#include <QVector>
#include <QPair>

class QStatusBar;
class QGroupBox;
class QPushButton;
class QListWidget;
class RunWidget;
class MemoryWindowWidget;
class PerformanceChart;
class FileViewer;

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
    QAction* performanceAction;
    QAction* actionAction;
    QMenu* memoryMenu;
    QAction* memoryAction;
    QMenu* styleMenu;
    QVector<QPair<QAction*, QString>> styles;
    void createStyles();

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
    QtCharts::QChartView* performanceChartView;
    void createPerformanceDock();

    // File Viewer
    FileViewer* fileViewer;
    void createFileViewer();

    // Memory Windows
    QList<QPair<QDockWidget*,QAction*>> memoryWindows;

public slots:
    void newMemoryWindow();
    void deleteMemoryWindow(MemoryWindowWidget* window);
    void reset();
    void writeOutput(QString message);
    void uncheckStyles(QAction* checkedStyle);
};

#endif // MAINWINDOW_H
