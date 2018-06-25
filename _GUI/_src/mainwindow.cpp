#include "mainwindow.h"
#include <QStyle>
#include <QVBoxLayout>

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

    // Make RunWidget central widget (Takes over window)
    runW = new RunWidget(this);
    runW->setAttribute(Qt::WA_DeleteOnClose);
    setCentralWidget(runW);
}

MainWindow::~MainWindow()
{
}


