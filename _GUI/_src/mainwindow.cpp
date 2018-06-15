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

    // Make runWidget central widget (Takes over window)
    runW = new runWidget(this);
    runW->setAttribute(Qt::WA_DeleteOnClose);
    setCentralWidget(runW);
}

MainWindow::~MainWindow()
{
}


