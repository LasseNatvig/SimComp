#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    // Import stylesheet
    #ifdef STYLESHEET
    // importStyle();
    #endif

    // Cosmetic operations
    appIcon = new QIcon(":/images/../_img/SimComp_icon.png");
    this->setWindowIcon(*appIcon);
    this->setWindowTitle("Simulating Computers");

    // Go to WELCOME-state
    currentState = globals::EMPTY;
    lastState = globals::EMPTY;
    changeState(globals::WELCOME);

}

MainWindow::~MainWindow()
{
}

void MainWindow::changeState(globals::windowState nextState) {
    //  Only dissconnect signals if signals have been connected
    exitState();

    // Update member variables to accomdate for state change
    lastState = currentState;
    currentState = nextState;

    // Do state specific initizialization
    switch (nextState) {
        case globals::WELCOME:
            welcomeW = new welcomeWidget(nullptr);

            // Delete object when closed
            welcomeW->setAttribute(Qt::WA_DeleteOnClose);

            // Connect signals
            connect(welcomeW, SIGNAL(next(QString)), this, SLOT(toRunState(QString)));

            setCentralWidget(welcomeW);
            break;

        case globals::RUN:
            runW->setAttribute(Qt::WA_DeleteOnClose);
            setCentralWidget(runW);
            //connect(runW, SIGNAL(next(globals::windowState), this, SLOT(changeState(globals::windowState)));
            break;

        case globals::EMPTY:
            break;
    }
}

void MainWindow::exitState() {
    switch (currentState) {
        case globals::WELCOME:
            disconnect(welcomeW, SIGNAL(next(QString)), this, SLOT(toRunState(QString)));
            break;
        case globals::RUN:
            break;
        case globals::EMPTY:
            break;
    }
}

void MainWindow::toRunState(QString filename) {
    runW = new runWidget(nullptr, filename);
    changeState(globals::RUN);
}
