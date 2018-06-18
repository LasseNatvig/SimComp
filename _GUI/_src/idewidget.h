#ifndef IDEWIDGET_H
#define IDEWIDGET_H

#include <fstream>
#include <sstream>
#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;


class IdeWidget : public QPlainTextEdit
{
    Q_OBJECT

public:
    IdeWidget(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

public slots:
    void save();
    void saveAs();
    void open(QString filename);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget* lineNumberArea;
    QTextDocument* doc;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(IdeWidget *editor) : QWidget(editor) {
        IdeWidget = editor;
    }

    QSize sizeHint() const override {
        return QSize(IdeWidget->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        IdeWidget->lineNumberAreaPaintEvent(event);
    }

private:
    IdeWidget *IdeWidget;
};







#endif // IDEWIDGET_H
