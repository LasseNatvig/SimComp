#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "runwidget.h"
#include "globals.h"
#include <QMainWindow>
#include <QIcon>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // Icon
    QIcon* appIcon;

    // Child widgets
    runWidget* runW;

public slots:
};

#endif // MAINWINDOW_H
