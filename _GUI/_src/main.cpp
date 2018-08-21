#include "mainwindow.h"
#include <QApplication>
// testing edit
int main(int argc, char *argv[])
{
    // Start application
    QApplication a(argc, argv);
    a.setApplicationName("SimComp");
    a.setOrganizationName("NTNU - IDI");

    // Create MainWindow and display it
    MainWindow w;
    w.show();

    return a.exec();
}
