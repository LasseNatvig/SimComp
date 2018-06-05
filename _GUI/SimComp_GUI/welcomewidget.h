#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include "globals.h"
#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

class welcomeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit welcomeWidget(QWidget *parent = nullptr);

signals:
    void next(QString filename);

public slots:

private slots:
    void openFile();

private:
    // Labels and buttons
    QLabel* title_lbl;
    QLabel* description_lbl;
    QPushButton* chooseFile_btn;

    // Program filename
    QString filename;

};

#endif // WELCOMEWIDGET_H
