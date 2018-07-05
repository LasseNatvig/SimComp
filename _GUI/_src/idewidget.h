#ifndef IDEWIDGET_H
#define IDEWIDGET_H

#include <QPlainTextEdit>

template<class T, class Allocator = std::allocator<T>> class vector;
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class LineNumberArea;
class BreakpointArea;

class IdeWidget : public QPlainTextEdit {
    Q_OBJECT

public:
    IdeWidget(QWidget *parent = 0);
    ~IdeWidget();
    void lineNumberAreaPaintEvent(QPaintEvent* event);
    void breakpointAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();
    int breakpointAreaWidth();
    void setBreakpoint(QPoint breakpoint);
    std::vector<int> getBreakpoints();

public slots:
    void save();
    void saveAs();
    void open(QString filename);
    void openFileDialog();
    void newFile();
    void updateBreakpoints(int lines);
    void clearBreakpoints();
    QString getFilename() { return filename; }

signals:
    void fileChanged(QString /* File name */);
    void breakpointsChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateSideAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void updateBreakpointArea(const QRect &, int dy);
    bool saveWarning();

private:
    QWidget* lineNumberArea;
    QWidget* breakpointArea;
    int* breakpoints;
    int lines;
    QPoint breakpoint;
    QTextDocument* doc;
    QString filename;
};


class LineNumberArea : public QWidget {
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


class BreakpointArea : public QWidget {
public:
    BreakpointArea(IdeWidget* editor) : QWidget(editor) {
        ideWidget = editor;
    }
    QSize sizeHint() const override {
        return QSize(ideWidget->breakpointAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        ideWidget->breakpointAreaPaintEvent(event);
    }
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton)
            ideWidget->setBreakpoint(event->pos());
        update();
    }

private:
    IdeWidget* ideWidget;
};


#endif // IDEWIDGET_H
