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
    createMenuBar();
    createActionDock();
    createStatusBar();

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
                QDockWidget::DockWidgetFloatable);
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

    outputLst = new QListWidget;
    connect(runW, SIGNAL(output(QString)), this, SLOT(writeOutput(QString)));

    QGridLayout* actionToolBoxLayout = new QGridLayout;
    actionToolBoxLayout->addLayout(buttonLayout, 0, 0);
    actionToolBoxLayout->addWidget(outputLst, 0, 1, 0, 5);
    actionToolBoxLayout->addWidget(resetBtn, 0, 6);
    actionToolBox->setLayout(actionToolBoxLayout);

    actionDock->setWidget(actionToolBox);
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
    closePerformanceAction = new QAction(tr("&Close"));
    closePerformanceAction->setShortcut(QKeySequence::Close);
    performanceChartView->addAction(closePerformanceAction);
    connect(closePerformanceAction, SIGNAL(triggered()),
            performanceChartView, SLOT(close()));
    performanceChartView->setWindowTitle("Performance");
    performanceChartView->setRenderHint(QPainter::Antialiasing);
    performanceChartView->setWindowFlag(Qt::Tool);
    performanceChartView->setFixedSize(
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
    memoryAction = new QAction(tr("&New Memory Window"));
    memoryAction->setShortcut(QKeySequence(tr("Ctrl+m")));
    performanceAction->setShortcut(QKeySequence(tr("Ctrl+p")));
    viewMenu->addAction(memoryAction);
    viewMenu->addAction(performanceAction);
    connect(memoryAction, SIGNAL(triggered()),
            this, SLOT(newMemoryWindow()));

    setMenuBar(menuBar);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Not running.");
}


/* SLOTS */
void MainWindow::newMemoryWindow() {
    MemoryWindowWidget* window = new MemoryWindowWidget(this,
                                                        runW->getSimulator());
    window->setWindowFlags(Qt::Window);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    memoryWindows.push_back(window);
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
