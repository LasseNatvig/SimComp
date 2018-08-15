#include "mainwindow.h"
#include "globals.h"
#include "runwidget.h"
#include "idewidget.h"
#include "memorywindow.h"
#include "performancechart.h"
#include "fileviewer.h"
#include <QStyle>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QDockWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QFileInfo>
#include <QDebug>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    // Create widgets
    createTabs();
    setCentralWidget(tabs);

    createPerformanceDock();
    createOutputDock();
    createStatusBar();
    createFileViewer();
    createMenuBar();

    // Cosmetic operations
    this->setWindowTitle("Simulating Computers");
    tabs->setCurrentIndex(1);

    // Set dock tabs to document mode
    setDocumentMode(true);
}

MainWindow::~MainWindow()
{
}


void MainWindow::createTabs() {
    tabs = new QTabWidget(this);
    runW = new RunWidget;
    ideW = new IdeWidget;
    connect(runW, &RunWidget::instructionCountChanged,
            [this] (int instructionCount) {
        this->statusBar()->showMessage("Instruction count: " +
                                       QString::number(instructionCount));
    });
    connect(ideW, &IdeWidget::fileChanged, runW, &RunWidget::load);
    connect(ideW, &IdeWidget::breakpointsChanged, [this] {
        this->runW->setBreakpoints(this->ideW->getBreakpoints());
    });

    tabs->addTab(runW, "Execution");
    tabs->addTab(ideW, "Editor");
}

void MainWindow::createOutputDock() {
    QDockWidget* outputDock = new QDockWidget("Output", this);
    outputDock->setAllowedAreas(Qt::TopDockWidgetArea |
                                Qt::BottomDockWidgetArea);
    outputDock->setFeatures(
                QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetClosable);
    outputDock->setMaximumHeight(globals::OUTPUTWINDOW_MAX_HEIGHT);

    outputBox = new QGroupBox;
    runBtn = new QPushButton(tr("&Run"));
    stepBtn = new QPushButton(tr("&Step"));
    nextBtn = new QPushButton(tr("&Continue"));
    resetBtn = new QPushButton(tr("&Reset"));
    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(runBtn);
    buttonLayout->addWidget(stepBtn);
    buttonLayout->addWidget(nextBtn);
    buttonLayout->setSpacing(0);
    connect(runBtn, &QPushButton::clicked, runW, &RunWidget::run);
    connect(stepBtn, &QPushButton::clicked, runW, &RunWidget::step);
    connect(nextBtn, &QPushButton::clicked, runW, &RunWidget::next);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::reset);

    outputLst = new QListWidget(outputBox);
    outputLst->setSelectionMode(QAbstractItemView::NoSelection);
    connect(runW, &RunWidget::output, this, &MainWindow::writeOutput);

    QGridLayout* outputBoxLayout = new QGridLayout;
    outputBoxLayout->addLayout(buttonLayout, 0, 0, 0, 1);
    outputBoxLayout->addWidget(outputLst, 0, 1, 0, 5);
    outputBoxLayout->addWidget(resetBtn, 0, 6, 0 , 1);
    outputBox->setLayout(outputBoxLayout);

    outputDock->setWidget(outputBox);
    outputAction = outputDock->toggleViewAction();
    addDockWidget(Qt::BottomDockWidgetArea, outputDock);
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
    connect(newAction, &QAction::triggered, ideW, &IdeWidget::newFile);
    connect(openAction, &QAction::triggered, ideW, &IdeWidget::openFileDialog);
    connect(saveAction, &QAction::triggered, ideW, &IdeWidget::save);
    connect(saveAsAction, &QAction::triggered, ideW, &IdeWidget::saveAs);

    editMenu = menuBar->addMenu(tr("&Edit"));
    undoAction = editMenu->addAction(tr("&Undo"));
    redoAction = editMenu->addAction(tr("&Redo"));
    undoAction->setShortcut(QKeySequence::Undo);
    redoAction->setShortcut(QKeySequence::Redo);
    connect(undoAction, &QAction::triggered, ideW, &IdeWidget::undo);
    connect(redoAction, &QAction::triggered, ideW, &IdeWidget::redo);

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
    outputAction->setShortcut(QKeySequence(tr("Ctrl+a")));
    fileViewerAction->setShortcut(QKeySequence(tr("Ctrl+f")));
    memoryMenu = viewMenu->addMenu(tr("&Memory Window"));
    memoryAction = memoryMenu->addAction(tr("&New Memory Window"));
    memoryAction->setShortcut(QKeySequence(tr("Ctrl+m")));
    memoryMenu->addSeparator();
    createStyles();
    styles[2].first->trigger(); // Set default style
    viewMenu->addAction(performanceAction);
    viewMenu->addAction(outputAction);
    viewMenu->addAction(fileViewerAction);
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

    // Update file viewer when ide changes file
    connect(ideW, &IdeWidget::fileChanged,
            [this] (QString filename) {
        QFileInfo temp(filename);
        this->fileViewer->setRootPath(temp.absolutePath());
        this->fileViewer->setFilename(temp.fileName());
    });
    // Enable opening from file viewer
    connect(fileViewer, &FileViewer::changeFile, ideW, &IdeWidget::open);
    dock->setWidget(fileViewer);
    fileViewerAction = dock->toggleViewAction();
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::createStyles() {
    styleMenu = viewMenu->addMenu(tr("&Change style"));
    styles = {
        QPair<QAction*, QString> (styleMenu->addAction(tr("&Blank")),
                                   ":/images/../_styles/blank.qss"),
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



/* Slots */
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

    MemoryWindow* window = new MemoryWindow(memoryDock,
                                     runW->getSimulator());
    connect(window, &MemoryWindow::windowNameChanged,
            memoryDock, &QDockWidget::setWindowTitle);
    connect(window, &MemoryWindow::newWindowRequested,
            this, &MainWindow::newMemoryWindow);
    connect(window, &MemoryWindow::deleteRequested,
            this, &MainWindow::deleteMemoryWindow);
    connect(runW, &RunWidget::memoryChanged,
            window, &MemoryWindow::updateDisplays);

    QAction* toggleAction = memoryDock->toggleViewAction();
    memoryMenu->addAction(toggleAction);
    memoryWindows.push_back(
                QPair<QDockWidget*, QAction*>(memoryDock, toggleAction));

    memoryDock->setWidget(window);
    addDockWidget(Qt::LeftDockWidgetArea, memoryDock);
}

void MainWindow::deleteMemoryWindow(MemoryWindow* memoryWindow) {
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
    runW->load(ideW->getFilename());
    statusBar()->showMessage("Not running.");
}



/* Utils */
void MainWindow::uncheckStyles(QAction* checkedStyle) {
    for (auto& pair : styles)
        if (pair.first != checkedStyle)
            pair.first->setChecked(false);
}
