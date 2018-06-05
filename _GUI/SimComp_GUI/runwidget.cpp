#include "runwidget.h"
#include <QGridLayout>
#include <iostream>

runWidget::runWidget(QWidget *parent, QString filename) : QWidget(parent)
{
    // Create labels and buttons
    title_lbl = new QLabel("Choose mode to run:\n \""+filename+"\nin.");
    run_btn = new QPushButton("Run");
    dropdownMenu = new QComboBox;
    // Create layouts
    QGridLayout* gridLayout = new QGridLayout;

    // Add widgets to layout
    gridLayout->addWidget(title_lbl, 0, 0);
    gridLayout->addWidget(dropdownMenu, 1, 0);
    gridLayout->addWidget(run_btn, 1,1);

    // Set layout for the window(widget)
    this->setLayout(gridLayout);

    // Make connections
//    connect(chooseFile_btn, SIGNAL(clicked()), this, SLOT(openFile()));
}
