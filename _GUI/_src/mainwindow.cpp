#include "mainwindow.h"
#include <iostream>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    // Import stylesheet
    #ifdef STYLESHEET
    #endif

    // Cosmetic operations
    appIcon = new QIcon(":/images/../_img/SimComp_icon.png");
    this->setWindowIcon(*appIcon);
    this->setWindowTitle("Simulating Computers");

    // Go to WELCOME-state
    currentState = globals::EMPTY;
    lastState = globals::EMPTY;
    changeState(globals::RUN);

}

MainWindow::~MainWindow()
{
}

void MainWindow::changeState(globals::windowState nextState) {
    // Update member variables to accomdate for state change
    lastState = currentState;
    currentState = nextState;

    // Do state specific initizialization
    switch (nextState) {
        case globals::WELCOME:
            break;
        case globals::RUN:
            runW = new runWidget(this);
            runW->setAttribute(Qt::WA_DeleteOnClose);
            setCentralWidget(runW);
            break;
        case globals::EMPTY:
            break;
    }
}

void MainWindow::exitState() {
    switch (currentState) {
        case globals::WELCOME:
            break;
        case globals::RUN:
            break;
        case globals::EMPTY:
            break;
    }
}

void MainWindow::toRunState(QString filename) {
    changeState(globals::RUN);
}
