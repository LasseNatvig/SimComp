#include "mainwindow.h"
#include <QStyle>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Import stylesheet
    #ifdef STYLESHEET
    #endif
    runW = new RunWidget(this);
    runW->setAttribute(Qt::WA_DeleteOnClose);
    connect(runW, SIGNAL(instructionCountChanged(int)),
            this, SLOT(updateInstructionCount(int)));
    setCentralWidget(runW);

    createPerformanceDock();
    createActionDock();
    createStatusBar();
    createMenuBar();

    // Cosmetic operations
    appIcon = new QIcon(":/images/../_img/SimComp_icon.png");
    this->setWindowIcon(*appIcon);
    this->setWindowTitle("Simulating Computers");
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActionDock() {
    QDockWidget* actionDock = new QDockWidget("Output", this);
    actionDock->setAllowedAreas(Qt::TopDockWidgetArea |
                                Qt::BottomDockWidgetArea);
    actionDock->setFeatures(
                QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetClosable);
	actionDock->setMaximumHeight(125);
    actionToolBox = new QGroupBox;

    runBtn = new QPushButton(tr("&Run"));
    stepBtn = new QPushButton(tr("&Step"));
    nextBtn = new QPushButton(tr("&Next"));
    resetBtn = new QPushButton(tr("&Reset"));
    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(runBtn);
    buttonLayout->addWidget(stepBtn);
    buttonLayout->addWidget(nextBtn);
    buttonLayout->setSpacing(0);
    connect(runBtn, SIGNAL(clicked()), runW, SLOT(run()));
    connect(stepBtn, SIGNAL(clicked()), runW, SLOT(step()));
    connect(nextBtn, SIGNAL(clicked()), runW, SLOT(next()));
    connect(resetBtn, SIGNAL(clicked()), this, SLOT(reset()));

    outputLst = new QListWidget(this);
    connect(runW, SIGNAL(output(QString)), this, SLOT(writeOutput(QString)));

    QGridLayout* actionToolBoxLayout = new QGridLayout;
    actionToolBoxLayout->addLayout(buttonLayout, 0, 0, 0, 1);
    actionToolBoxLayout->addWidget(outputLst, 0, 1, 0, 5);
    actionToolBoxLayout->addWidget(resetBtn, 0, 6, 0 , 1);
    actionToolBox->setLayout(actionToolBoxLayout);

    actionDock->setWidget(actionToolBox);
    actionAction = actionDock->toggleViewAction();
    addDockWidget(Qt::BottomDockWidgetArea, actionDock);
}

void MainWindow::createPerformanceDock() {
    QDockWidget* performanceDock = new QDockWidget("Performance", this);
    performanceDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                     Qt::RightDockWidgetArea);
    performanceDock->setFeatures(
                QDockWidget::DockWidgetClosable |
                QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable);
    performanceAction = performanceDock->toggleViewAction();

    performanceChart = new PerformanceChart;
    performanceChartView = new QChartView(performanceChart, this);
    performanceChartView->setWindowTitle("Performance");
    performanceChartView->setRenderHint(QPainter::Antialiasing);
    performanceChartView->setWindowFlag(Qt::Tool);
    performanceChartView->setMinimumSize(
                QSize(globals::PERFORMANCEWINDOW_MIN_WIDTH,
                      globals::PERFORMANCEWINDOW_MIN_HEIGHT));    
    connect(runW, &RunWidget::performanceChanged, performanceChart,
            &PerformanceChart::updatePerformance);

    performanceDock->setWidget(performanceChartView);
    addDockWidget(Qt::RightDockWidgetArea, performanceDock);
}

void MainWindow::createMenuBar() {
    menuBar = new QMenuBar;

    fileMenu = menuBar->addMenu(tr("&File"));
    newAction = new QAction(tr("&New"));
    openAction = new QAction(tr("&Open"));
    saveAction = new QAction(tr("&Save"));
    saveAsAction = new QAction(tr("&Save As"));
    newAction->setShortcut(QKeySequence::New);
    openAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    connect(newAction, SIGNAL(triggered()),
            runW, SLOT(newFile()));
    connect(openAction, SIGNAL(triggered()),
            runW, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()),
            runW, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()),
            runW, SLOT(saveAs()));

    editMenu = menuBar->addMenu(tr("&Edit"));
    undoAction = new QAction(tr("&Undo"));
    redoAction = new QAction(tr("&Redo"));
    undoAction->setShortcut(QKeySequence::Undo);
    redoAction->setShortcut(QKeySequence::Redo);
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    connect(undoAction, SIGNAL(triggered()), runW, SLOT(undo()));
    connect(redoAction, SIGNAL(triggered()), runW, SLOT(redo()));

    buildMenu = menuBar->addMenu(tr("&Build"));
    runAction = new QAction(tr("&Run"));
    stepAction = new QAction(tr("&Step"));
    nextAction = new QAction(tr("&Next"));
    resetAction = new QAction(tr("&Reset"));
    runAction->setShortcut(QKeySequence::Refresh);
    resetAction->setShortcut(QKeySequence(tr("Shift+r")));
    stepAction->setShortcut(QKeySequence(tr("Shift+s")));
    nextAction->setShortcut(QKeySequence(tr("Shift+n")));
    buildMenu->addAction(runAction);
    buildMenu->addAction(stepAction);
    buildMenu->addAction(nextAction);
    buildMenu->addSeparator();
    buildMenu->addAction(resetAction);
    connect(runAction, SIGNAL(triggered()), runW, SLOT(run()));
    connect(stepAction, SIGNAL(triggered()), runW, SLOT(step()));
    connect(nextAction, SIGNAL(triggered()), runW, SLOT(next()));
    connect(resetAction, SIGNAL(triggered()), this, SLOT(reset()));

    viewMenu = menuBar->addMenu(tr("&View"));

    performanceAction->setShortcut(QKeySequence(tr("Ctrl+p")));
    actionAction->setShortcut(QKeySequence(tr("Ctrl+a")));

    memoryMenu = viewMenu->addMenu(tr("&Memory Window"));
    memoryAction = new QAction(tr("&New Memory Window"));
    memoryAction->setShortcut(QKeySequence(tr("Ctrl+m")));
    memoryMenu->addAction(memoryAction);
    memoryMenu->addSeparator();

    viewMenu->addAction(performanceAction);
    viewMenu->addAction(actionAction);
    connect(memoryAction, SIGNAL(triggered()),
            this, SLOT(newMemoryWindow()));

    setMenuBar(menuBar);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Not running.");
}



/* SLOTS */
void MainWindow::newMemoryWindow() {
    QDockWidget* memoryDock = new QDockWidget("Memory Window #" +
                                              QString::number(
                                                  memoryWindows.size()), this);
    memoryDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                Qt::RightDockWidgetArea);
    memoryDock->setFeatures(
                QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetClosable);
    memoryDock->setAttribute(Qt::WA_DeleteOnClose);

    MemoryWindowWidget* window = new MemoryWindowWidget(this,
                                                        runW->getSimulator());
    connect(window, SIGNAL(windowNameChanged(QString)),
            memoryDock, SLOT(setWindowTitle(QString)));
    connect(window, SIGNAL(newWindowRequested()),
            this, SLOT(newMemoryWindow()));
    connect(window, SIGNAL(deleteRequested(MemoryWindowWidget*)),
            this, SLOT(deleteMemoryWindow(MemoryWindowWidget*)));
    connect(runW, SIGNAL(memoryChanged()), window, SLOT(updateDisplays()));

    QAction* toggleAction = memoryDock->toggleViewAction();
    memoryMenu->addAction(toggleAction);
    memoryWindows.push_back(
                QPair<MemoryWindowWidget*, QAction*>(window, toggleAction));

    memoryDock->setWidget(window);
    addDockWidget(Qt::LeftDockWidgetArea, memoryDock);
}

void MainWindow::deleteMemoryWindow(MemoryWindowWidget* memoryWindow) {
    for (auto& pair : memoryWindows) {
        if (pair.first == memoryWindow) {
            memoryMenu->removeAction(pair.second);
            pair.first->close();
            break;
        }
    }
}

void MainWindow::writeOutput(QString message) {
    outputLst->addItem(message);
    outputLst->scrollToBottom();
}

void MainWindow::updateInstructionCount(int instructionCount) {
    statusBar()->showMessage("Instruction count: " +
                             QString::number(instructionCount));
}

void MainWindow::reset() {
    performanceChart->reset();
    runW->reset();
    statusBar()->showMessage("Not running.");
}
