#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QList>
#include <QVector>
#include <QPair>

class QTabWidget;
class QStatusBar;
class QGroupBox;
class QPushButton;
class QListWidget;
class RunWidget;
class IdeWidget;
class MemoryWindow;
class PerformanceChart;
class FileViewer;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // Tabs (Central widget)
    QTabWidget* tabs;
    RunWidget* runW;
    IdeWidget* ideW;
    void createTabs();

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
    QAction* outputAction;
    QMenu* memoryMenu;
    QAction* memoryAction;
    QMenu* styleMenu;
    QVector<QPair<QAction*, QString>> styles;
    void createStyles();

    void createMenuBar();
    void createStatusBar();

    // Output dock
    QGroupBox* outputBox;
    QPushButton* runBtn;
    QPushButton* stepBtn;
    QPushButton* nextBtn;
    QPushButton* resetBtn;
    QListWidget* outputLst;
    QAction* closePerformanceAction;
    void createOutputDock();

    // Performance Dock
    PerformanceChart* performanceChart;
    QtCharts::QChartView* performanceChartView;
    void createPerformanceDock();

    // File Viewer
    FileViewer* fileViewer;
    QAction* fileViewerAction;
    void createFileViewer();

    // Memory Windows
    QList<QPair<QDockWidget*,QAction*>> memoryWindows;

public slots:
    void newMemoryWindow();
    void deleteMemoryWindow(MemoryWindow* window);
    void reset();
    void writeOutput(QString message);
    void uncheckStyles(QAction* checkedStyle);
};

#endif // MAINWINDOW_H
