#ifndef DROPDOWNWIDGET_H
#define DROPDOWNWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QVector>

class DropdownWidget : public QWidget
{
    Q_OBJECT
public:
    enum Direction {
        Left,
        Right,
        Up,
        Down
    };
    explicit DropdownWidget(Direction direction, QWidget *parent = nullptr);
    explicit DropdownWidget(Direction direction, QString text, QWidget* parent = nullptr);
signals:
    void toggleChanged(bool hidden);
public slots:
    void toggle();
    bool hidden() const;
    Direction getDirection() const;
    void addWidget(QWidget *widget, int stretch = 0,
                   Qt::Alignment aligment = Qt::Alignment());
    void setText(const QString text);
private:
    QBoxLayout* layout;
    QPushButton* button;
    QIcon showIcon;
    QIcon hideIcon;
    QString text = "";
    Direction direction;
    QVector<QWidget*> widgets;
    bool _hidden;
    void create();
};

#endif // DROPDOWNWIDGET_H
