#include "welcomewidget.h"
#include <iostream>

welcomeWidget::welcomeWidget(QWidget *parent) : QWidget(parent)
{
    // Create labels and buttons
    title_lbl = new QLabel("Welcome to SimComp Version 0.30");
    description_lbl = new QLabel("Please choose which file you would like to run:");
    chooseFile_btn = new QPushButton("Open");

    // Create layouts
    QGridLayout* gridLayout = new QGridLayout;

    // Add widgets to layout
    gridLayout->addWidget(title_lbl, 0, 0);
    gridLayout->addWidget(description_lbl, 1, 0);
    gridLayout->addWidget(chooseFile_btn, 1,1);

    // Set layout for the window(widget)
    this->setLayout(gridLayout);

    // Make connections
    connect(chooseFile_btn, SIGNAL(clicked()), this, SLOT(openFile()));
}


void welcomeWidget::openFile() {
    // Starts a file dialog which lets the user choose a assembler program to run
    filename = QFileDialog::getOpenFileName(this, tr("Open file"), "/",tr("Assembler program (*.sasm)"));

    // Emit signal for moving to next screen if file is choosen
    if (!filename.isNull()) {
        emit next(filename);
        this->close();
    }
}
