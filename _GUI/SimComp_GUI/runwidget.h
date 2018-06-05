#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

class runWidget : public QWidget
{
    Q_OBJECT
public:
    explicit runWidget(QWidget *parent = nullptr, QString filename = "");

private:
    QLabel* title_lbl;
    QComboBox* dropdownMenu;
    QPushButton* run_btn;
    QString filename;
signals:

public slots:
};

#endif // RUNWIDGET_H
