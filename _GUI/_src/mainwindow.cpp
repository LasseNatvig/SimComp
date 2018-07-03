#include "mainwindow.h"
#include "globals.h"
#include "runwidget.h"
#include "memorywindowwidget.h"
#include "performancechart.h"
#include "fileviewer.h"
#include <QStyle>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QDockWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QFileInfo>
#include <QDebug>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    runW = new RunWidget(this);
    runW->setAttribute(Qt::WA_DeleteOnClose);
    connect(runW, &RunWidget::instructionCountChanged,
            [this] (int instructionCount) {
        this->statusBar()->showMessage("Instruction count: " +
                                       QString::number(instructionCount));
    });
    setCentralWidget(runW);

    createPerformanceDock();
    createActionDock();
    createStatusBar();
    createMenuBar();
    createFileViewer();

    // Cosmetic operations
    appIcon = new QIcon(":/images/../_img/SimComp_icon.png");
    this->setWindowIcon(*appIcon);
    this->setWindowTitle("Simulating Computers");

    setDocumentMode(true);
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
    actionDock->setMaximumHeight(globals::OUTPUTWINDOW_MAX_HEIGHT);

    actionToolBox = new QGroupBox(this);
    runBtn = new QPushButton(tr("&Run"), actionToolBox);
    stepBtn = new QPushButton(tr("&Step"), actionToolBox);
    nextBtn = new QPushButton(tr("&Next"), actionToolBox);
    resetBtn = new QPushButton(tr("&Reset"), actionToolBox);
    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(runBtn);
    buttonLayout->addWidget(stepBtn);
    buttonLayout->addWidget(nextBtn);
    buttonLayout->setSpacing(0);
    connect(runBtn, &QPushButton::clicked, runW, &RunWidget::run);
    connect(stepBtn, &QPushButton::clicked, runW, &RunWidget::step);
    connect(nextBtn, &QPushButton::clicked, runW, &RunWidget::next);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::reset);

    outputLst = new QListWidget(actionToolBox);
    connect(runW, &RunWidget::output, this, &MainWindow::writeOutput);

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
    performanceDock->hide();
}

void MainWindow::createMenuBar() {
    menuBar = new QMenuBar(this);

    fileMenu = menuBar->addMenu(tr("&File"));
    newAction = fileMenu->addAction(tr("&New"));
    openAction = fileMenu->addAction(tr("&Open"));
    fileMenu->addSeparator();
    saveAction = fileMenu->addAction(tr("&Save"));
    saveAsAction = fileMenu->addAction(tr("&Save As"));
    newAction->setShortcut(QKeySequence::New);
    openAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(newAction, &QAction::triggered, runW, &RunWidget::newFile);
    connect(openAction, &QAction::triggered, runW, &RunWidget::openFileDialog);
    connect(saveAction, &QAction::triggered, runW, &RunWidget::save);
    connect(saveAsAction, &QAction::triggered, runW, &RunWidget::saveAs);

    editMenu = menuBar->addMenu(tr("&Edit"));
    undoAction = editMenu->addAction(tr("&Undo"));
    redoAction = editMenu->addAction(tr("&Redo"));
    undoAction->setShortcut(QKeySequence::Undo);
    redoAction->setShortcut(QKeySequence::Redo);
    connect(undoAction, &QAction::triggered, runW, &RunWidget::undo);
    connect(redoAction, &QAction::triggered, runW, &RunWidget::redo);

    buildMenu = menuBar->addMenu(tr("&Build"));
    runAction = buildMenu->addAction(tr("&Run"));
    stepAction = buildMenu->addAction(tr("&Step"));
    nextAction = buildMenu->addAction(tr("&Next"));
    buildMenu->addSeparator();
    resetAction = buildMenu->addAction(tr("&Reset"));
    runAction->setShortcut(QKeySequence::Refresh);
    resetAction->setShortcut(QKeySequence(tr("Shift+r")));
    stepAction->setShortcut(QKeySequence(tr("Shift+s")));
    nextAction->setShortcut(QKeySequence(tr("Shift+n")));
    connect(runAction, &QAction::triggered, runW, &RunWidget::run);
    connect(stepAction, &QAction::triggered, runW, &RunWidget::step);
    connect(nextAction, &QAction::triggered, runW, &RunWidget::next);
    connect(resetAction, &QAction::triggered, this, &MainWindow::reset);

    viewMenu = menuBar->addMenu(tr("&View"));
    performanceAction->setShortcut(QKeySequence(tr("Ctrl+p")));
    actionAction->setShortcut(QKeySequence(tr("Ctrl+a")));
    memoryMenu = viewMenu->addMenu(tr("&Memory Window"));
    memoryAction = memoryMenu->addAction(tr("&New Memory Window"));
    memoryAction->setShortcut(QKeySequence(tr("Ctrl+m")));
    memoryMenu->addSeparator();
    createStyles();
    styles[3].first->trigger(); // Set start style
    viewMenu->addAction(performanceAction);
    viewMenu->addAction(actionAction);
    connect(memoryAction, &QAction::triggered,
            this, &MainWindow::newMemoryWindow);

    setMenuBar(menuBar);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Not running.");
}

void MainWindow::createFileViewer() {
    QDockWidget* dock = new QDockWidget("File Viewer");
    QStringList nameFilters("*.sasm");
    fileViewer = new FileViewer(nameFilters);
    connect(runW, &RunWidget::filenameChanged,
            [this] (QString filename) {
        QFileInfo temp(filename);
        this->fileViewer->setRootPath(temp.absolutePath());
        this->fileViewer->setFilename(temp.fileName());
    });
    connect(fileViewer, &FileViewer::changeFile, runW, &RunWidget::open);
    dock->setWidget(fileViewer);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::createStyles() {
    styleMenu = viewMenu->addMenu(tr("&Change style"));
    styles = {
        QPair<QAction*, QString> (styleMenu->addAction(tr("&Blank")),
                                   ":/images/../_styles/blank.qss"),
        QPair<QAction*, QString> (styleMenu->addAction(tr("&Material Blue")),
                                   ":/images/../_styles/material-blue.qss"),
        QPair<QAction*, QString> (styleMenu->addAction(tr("&QTDark")),
                                   ":/images/../_styles/QTDark.qss"),
        QPair<QAction*, QString> (styleMenu->addAction(tr("&MSVS Dark")),
                                    ":/images/../_styles/vs_dark.qss")
    };
    for (auto& pair : styles) {
        pair.first->setCheckable(true);
        connect(pair.first, &QAction::triggered, [pair, this] {
            QFile file(pair.second);

            if (!file.open(QFile::ReadOnly | QFile::Text))
                qDebug() << "Cound not import stylesheet.";
            else {
                uncheckStyles(pair.first);
                pair.first->setChecked(true);
                QTextStream t(&file);
                this->setStyleSheet(t.readAll());
            }
        });
    }
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

    MemoryWindowWidget* window = new MemoryWindowWidget(memoryDock,
                                     runW->getSimulator());
    connect(window, &MemoryWindowWidget::windowNameChanged,
            memoryDock, &QDockWidget::setWindowTitle);
    connect(window, &MemoryWindowWidget::newWindowRequested,
            this, &MainWindow::newMemoryWindow);
    connect(window, &MemoryWindowWidget::deleteRequested,
            this, &MainWindow::deleteMemoryWindow);
    connect(runW, &RunWidget::memoryChanged,
            window, &MemoryWindowWidget::updateDisplays);

    QAction* toggleAction = memoryDock->toggleViewAction();
    memoryMenu->addAction(toggleAction);
    memoryWindows.push_back(
                QPair<QDockWidget*, QAction*>(memoryDock, toggleAction));

    memoryDock->setWidget(window);
    addDockWidget(Qt::LeftDockWidgetArea, memoryDock);
}

void MainWindow::deleteMemoryWindow(MemoryWindowWidget* memoryWindow) {
    for (auto& pair : memoryWindows) {
        if (pair.first->widget() == memoryWindow) {
            memoryMenu->removeAction(pair.second); // Remove action from menu
            removeDockWidget(pair.first); // Remove dock from MainWindow
            pair.first->setParent(nullptr); // Remove Parent relation
            delete pair.first; // Delete Dock
            memoryWindows.removeOne(pair); // Remove pair from vector
            break;
        }
    }
}

void MainWindow::writeOutput(QString message) {
    outputLst->addItem(message);
    outputLst->scrollToBottom();
}

void MainWindow::reset() {
    performanceChart->reset();
    runW->reset();
    statusBar()->showMessage("Not running.");
}



/* UTILS */
void MainWindow::uncheckStyles(QAction* checkedStyle) {
    for (auto& pair : styles)
        if (pair.first != checkedStyle)
            pair.first->setChecked(false);
}
