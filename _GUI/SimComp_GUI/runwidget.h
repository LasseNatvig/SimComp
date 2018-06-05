#ifndef RUNWIDGET_H
#define RUNWIDGET_H

#include <QWidget>
#include <QString>

class runWidget : public QWidget
{
    Q_OBJECT
public:
    explicit runWidget(QWidget *parent = nullptr, QString filename = "");

private:
    QString filename;
signals:

public slots:
};

#endif // RUNWIDGET_H
