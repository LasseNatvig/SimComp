#include "idewidget.h"
#include <iostream>
#include <QGridLayout>
#include <QtWidgets>
#include <QPlainTextDocumentLayout>
#include <QFileDialog>
#include <QSettings>

// See http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html


IdeWidget::IdeWidget(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    breakPointArea = new BreakPointArea(this);
    lines = 1;
    breakPoints = new int[lines] {};
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateSideAreaWidth(int)));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateBreakPoints(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateBreakPointArea(QRect,int)));
 //   connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateSideAreaWidth(0);
    highlightCurrentLine();
}


/* LINE NUMBER */
int IdeWidget::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().boundingRect(QLatin1Char('9')).width() * digits;

    return space;
}

void IdeWidget::updateSideAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth()+breakPointAreaWidth(), 0, 0, 0);
}

void IdeWidget::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateSideAreaWidth(0);
}

void IdeWidget::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left()+breakPointAreaWidth(), cr.top(), lineNumberAreaWidth(), cr.height()));
    breakPointArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void IdeWidget::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::blue).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void IdeWidget::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


/* BREAK POINTS*/
int IdeWidget::breakPointAreaWidth() {
    return 10;
}

void IdeWidget::updateBreakPointArea(const QRect &rect, int dy) {
    if (dy)
        breakPointArea->scroll(0, dy);
    else
        breakPointArea->update(0, rect.y(), breakPointArea->width(), rect.height());
    if (rect.contains(viewport()->rect()))
        updateSideAreaWidth(0);
}

void IdeWidget::updateBreakPoints(int newLines) {
    int* temp = new int[newLines];
    for (int i = 0; i < newLines; i++) {
        if (i >= lines)
            temp[i] = 0;
        else
            temp[i] = breakPoints[i];
    }
    delete [] breakPoints;
    lines = newLines;
    breakPoints = temp;
}

void IdeWidget::clearBreakPoints() {
    for (int i = 0; i < lines; i++) {
        breakPoints[i] = 0;
    }
    update();
}

void IdeWidget::breakPointAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(breakPointArea);
    QRect bpaRect(event->rect().x(), event->rect().y(), breakPointAreaWidth(), event->rect().height());
    painter.fillRect(bpaRect, Qt::darkGray);

    QTextBlock block = firstVisibleBlock();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    int blockNumber = block.blockNumber();
    while (block.isValid() && top <= event->rect().bottom()) {
        QRect r(0, top, breakPointAreaWidth(), fontMetrics().height());
        if (breakPoints[blockNumber])
            painter.fillRect(r,Qt::red);
        ++blockNumber;
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
    }
}

void IdeWidget::setBreakPoint(QPoint breakPoint) {
    QTextBlock block = firstVisibleBlock();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    int blockNumber = block.blockNumber();
    while (block.isValid()) {
        if (breakPoint.y() >= top && breakPoint.y() <= bottom){
            if (breakPoints[blockNumber])
                breakPoints[blockNumber] = 0;
            else
                breakPoints[blockNumber] = 1;
        }
        ++blockNumber;
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
    }
}


/* CORE */
void IdeWidget::save() {
    if (filename.isEmpty())
        saveAs();
    std::ofstream file;
    file.open(filename.toStdString());
    doc = this->document();
    file << (doc->toPlainText()).toStdString();
    file.close();
}

void IdeWidget::saveAs() {
    const QString DEFAULT_DIR_KEY("/");
    QSettings settings;
    filename = QFileDialog::getSaveFileName(
                this, "Save", settings.value(DEFAULT_DIR_KEY).toString(), tr("Assembler program (*.sasm)"));
    if (filename.isEmpty())
        return;
    emit updateLabel(filename);
    save();
}

void IdeWidget::open(QString filename) {
    this->filename = filename;
    std::ifstream file;
    file.open(filename.toStdString());
    if (file.fail()) return;
    emit updateLabel(filename);

    std::stringstream ss;
    std::string line;
    while (getline(file, line))
        ss << line;
    file.close();

    doc = new QTextDocument(QString::fromStdString(ss.str()));
    updateBreakPoints(doc->blockCount());
    QPlainTextDocumentLayout* plainDoc = new QPlainTextDocumentLayout(doc);
    doc->setDocumentLayout(plainDoc);
    setDocument(doc);
}
