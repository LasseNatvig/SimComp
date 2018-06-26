#ifndef IDEWIDGET_H
#define IDEWIDGET_H

#include <fstream>
#include <sstream>
#include <vector>
#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;
class BreakPointArea;


class IdeWidget : public QPlainTextEdit
{
    Q_OBJECT

public:
    IdeWidget(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent* event);
    void breakPointAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();
    int breakPointAreaWidth();
    void setBreakPoint(QPoint breakPoint);
    std::vector<int> getBreakPoints();

public slots:
    void save();
    void saveAs();
    void open(QString filename);
    void newFile();
    void updateBreakPoints(int lines);
    void clearBreakPoints();

signals:
    void filenameChanged(QString label);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateSideAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void updateBreakPointArea(const QRect &, int dy);
    bool saveWarning();

private:
    QWidget* lineNumberArea;
    QWidget* breakPointArea;
    int* breakPoints;
    int lines;
    QPoint breakPoint;
    QTextDocument* doc;
    QString filename;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(IdeWidget *editor) : QWidget(editor) {
        ideWidget = editor;
    }

    QSize sizeHint() const override {
        return QSize(ideWidget->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        ideWidget->lineNumberAreaPaintEvent(event);
    }

private:
    IdeWidget* ideWidget;
};


class BreakPointArea : public QWidget
{
public:
    BreakPointArea(IdeWidget* editor) : QWidget(editor) {
        ideWidget = editor;
    }
    QSize sizeHint() const override {
        return QSize(ideWidget->breakPointAreaWidth(), 0);
    }
protected:
    void paintEvent(QPaintEvent* event) override {
        ideWidget->breakPointAreaPaintEvent(event);
    }
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton)
            ideWidget->setBreakPoint(event->pos());
        update();
    }
private:
    IdeWidget* ideWidget;
};


#endif // IDEWIDGET_H
