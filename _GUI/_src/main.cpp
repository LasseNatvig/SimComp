#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[]) {
	QApplication a{ argc, argv }; // Start application
    a.setApplicationName("SimComp");
    a.setOrganizationName("NTNU - IDI");
    MainWindow w; // Create MainWindow w
    w.show(); // display w
    return a.exec();
}