#include "runwidget.h"
#include <iostream>

runWidget::runWidget(QWidget *parent, QString filename) : QWidget(parent)
{
    std::cout << filename.toStdString();
}
