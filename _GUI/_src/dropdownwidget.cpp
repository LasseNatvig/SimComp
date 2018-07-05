#include "dropdownwidget.h"
#include <QApplication>
#include <QStyle>
#include <QPushButton>
#include <QBoxLayout>

DropdownWidget::DropdownWidget(Direction direction, QWidget *parent) :
    DropdownWidget(direction, "", parent) { }

DropdownWidget::DropdownWidget(Direction direction, QString text, QWidget *parent) :
    QWidget(parent), text(text), direction(direction), _hidden(true) {
    create();
    setLayout(layout);
}

void DropdownWidget::create() {
    Qt::AlignmentFlag alignment;
    switch (direction) {
        case Left:
            showIcon = QIcon(QApplication::style()->standardIcon(
                             QStyle::SP_MediaSeekForward));
            hideIcon = QIcon(QApplication::style()->standardIcon(
                                 QStyle::SP_MediaSeekBackward));
            layout = new QBoxLayout(QBoxLayout::LeftToRight);
            alignment = Qt::AlignLeading;
            break;
        case Right:
            showIcon = QIcon(QApplication::style()->standardIcon(
                             QStyle::SP_MediaSeekBackward));
            hideIcon = QIcon(QApplication::style()->standardIcon(
                                 QStyle::SP_MediaSeekForward));
            layout = new QBoxLayout(QBoxLayout::RightToLeft);
            alignment = Qt::AlignTrailing;
            break;
        case Down:
            showIcon = QIcon(QApplication::style()->standardIcon(
                                 QStyle::SP_TitleBarUnshadeButton));
            hideIcon = QIcon(QApplication::style()->standardIcon(
                                 QStyle::SP_TitleBarShadeButton));
            layout = new QBoxLayout(QBoxLayout::TopToBottom);
            alignment = Qt::AlignTop;
            break;
        case Up:
            showIcon = QIcon(QApplication::style()->standardIcon(
                             QStyle::SP_TitleBarShadeButton));
            hideIcon = QIcon(QApplication::style()->standardIcon(
                                 QStyle::SP_TitleBarUnshadeButton));
            layout = new QBoxLayout(QBoxLayout::BottomToTop);
            alignment = Qt::AlignBottom;
            break;
    }

    button = new QPushButton(showIcon, text);
    connect(button, &QPushButton::clicked, this, &DropdownWidget::toggle);
    layout->addWidget(button, 0, alignment);
}

void DropdownWidget::toggle() {
    if (!_hidden) {
        for (auto& widget : widgets)
            widget->hide();
        _hidden = true;
        button->setIcon(showIcon);
        emit toggleChanged(true);
    }
    else {
        for (auto& widget : widgets)
            widget->show();
        _hidden = false;
        button->setIcon(hideIcon);
        emit toggleChanged(false);
    }
}

bool DropdownWidget::hidden() const {
    return _hidden;
}

void DropdownWidget::setText(const QString &text) {
    this->text = text;
    button->setText(text);
}

DropdownWidget::Direction DropdownWidget::getDirection() const {
    return direction;
}

void DropdownWidget::addWidget(QWidget *widget, int stretch, Qt::Alignment alignment) {
    widget->hide();
    widgets.push_back(widget);
    layout->addWidget(widget, stretch, alignment);
}
