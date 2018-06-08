#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "runwidget.h"
#include "globals.h"
#include <QMainWindow>
#include <QIcon>

#define STYLESHEET  0


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

    // State variables
    globals::windowState currentState;
    globals::windowState lastState;

    // State functions
    void exitState();

public slots:
    void changeState(globals::windowState nextState);
    void toRunState(QString filename);
};

#endif // MAINWINDOW_H
