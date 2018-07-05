#include "idewidget.h"
#include "globals.h"
#include <vector>
#include <QGridLayout>
#include <QtWidgets>
#include <QPlainTextDocumentLayout>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

// See http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html


IdeWidget::IdeWidget(QWidget *parent) : QPlainTextEdit(parent), lines(1) {
    breakpointArea = new BreakpointArea(this);
    lineNumberArea = new LineNumberArea(this);

    QFont font = document()->defaultFont();
    font.setFamily("Courier New");
    document()->setDefaultFont(font);

    breakpoints = new int[lines] {};
    connect(this, &QPlainTextEdit::blockCountChanged, this,
            &IdeWidget::updateSideAreaWidth);
    connect(this, &QPlainTextEdit::blockCountChanged, this,
            &IdeWidget::updateBreakpoints);
    connect(this, &QPlainTextEdit::updateRequest, this,
            &IdeWidget::updateLineNumberArea);
    connect(this, &QPlainTextEdit::updateRequest, this,
            &IdeWidget::updateBreakpointArea);
    updateSideAreaWidth(0);
}

IdeWidget::~IdeWidget() {
    delete breakpoints;
    breakpoints = nullptr;
}



/* Line number */
int IdeWidget::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().boundingRect(QLatin1Char('9')).width()
            * digits;
    return space;
}

void IdeWidget::updateSideAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth()+breakpointAreaWidth(), 0, 0, 0);
}

void IdeWidget::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(),
                               lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateSideAreaWidth(0);
}

void IdeWidget::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(
                QRect(cr.left()+breakpointAreaWidth(),
                      cr.top(), lineNumberAreaWidth(), cr.height()));
    breakpointArea->setGeometry(
                QRect(cr.left(), cr.top(),
                      lineNumberAreaWidth(), cr.height()));
}

void IdeWidget::highlightCurrentLine() {
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

void IdeWidget::lineNumberAreaPaintEvent(QPaintEvent *event) {
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
            painter.drawText(0, top, lineNumberArea->width(),
                             fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


/* Breakpoints */
int IdeWidget::breakpointAreaWidth() {
    return 10;
}

void IdeWidget::updateBreakpointArea(const QRect &rect, int dy) {
    if (dy)
        breakpointArea->scroll(0, dy);
    else
        breakpointArea->update(0, rect.y(),
                               breakpointArea->width(), rect.height());
    if (rect.contains(viewport()->rect()))
        updateSideAreaWidth(0);
}

void IdeWidget::updateBreakpoints(int newLines) {
    int* temp = new int[newLines];
    for (int i = 0; i < newLines; i++) {
        if (i >= lines)
            temp[i] = 0;
        else
            temp[i] = breakpoints[i];
    }
    delete [] breakpoints;
    lines = newLines;
    breakpoints = temp;
}

void IdeWidget::clearBreakpoints() {
    for (int i = 0; i < lines; i++) {
        breakpoints[i] = 0;
    }
    update();
}

void IdeWidget::breakpointAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(breakpointArea);
    QRect bpaRect(event->rect().x(), event->rect().y(),
                  breakpointAreaWidth(), event->rect().height());
    painter.fillRect(bpaRect, Qt::darkGray);

    QTextBlock block = firstVisibleBlock();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    int blockNumber = block.blockNumber();
    while (block.isValid() && top <= event->rect().bottom()) {
        QRect r(0, top, breakpointAreaWidth(), fontMetrics().height());
        if (breakpoints[blockNumber])
            painter.fillRect(r,Qt::red);
        ++blockNumber;
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
    }
}

void IdeWidget::setBreakpoint(QPoint breakpoint) {
    QTextBlock block = firstVisibleBlock();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    int blockNumber = block.blockNumber();
    while (block.isValid()) {
        if (breakpoint.y() >= top && breakpoint.y() <= bottom){
            if (breakpoints[blockNumber])
                breakpoints[blockNumber] = 0;
            else
                breakpoints[blockNumber] = 1;
        }
        ++blockNumber;
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
    }
    emit breakpointsChanged();
}

std::vector<int> IdeWidget::getBreakpoints() {
    std::vector<int> ret;
    for (int i = 0; i < lines; i++)
        if (breakpoints[i])
            ret.push_back(i+1);
    return ret;
}


/* Core */
void IdeWidget::save() {
    if (filename.isEmpty())
        saveAs();
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;
    QTextStream out(&file);
    out << toPlainText();
    file.close();
    document()->setModified(false);
    emit fileChanged(filename);
}

void IdeWidget::saveAs() {
    const QString DEFAULT_DIR_KEY("\\");
    QSettings settings;
    filename = QFileDialog::getSaveFileName(
                this, "Save",
                settings.value(DEFAULT_DIR_KEY).toString(),
                tr("Assembler program (*.sasm)"));
    if (filename.isEmpty())
        return;
    save();
}

void IdeWidget::open(QString filename) {
    if (document()->isModified())
        saveWarning();
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;
    QTextStream in(&file);
    setPlainText(in.readAll());
    this->filename = filename;
    document()->setModified(false);
    updateSideAreaWidth(0);
    emit fileChanged(filename);
}

void IdeWidget::openFileDialog() {
    const QString DEFAULT_DIR_KEY("\\");
    QSettings settings;

    QString selectedFile = QFileDialog::getOpenFileName(
                this, "Open", settings.value(DEFAULT_DIR_KEY).toString(),
                tr("Assembler program (*.sasm)"));

    if (!selectedFile.isEmpty()) {
        QDir currentDir;
        settings.setValue(DEFAULT_DIR_KEY,
                            currentDir.absoluteFilePath(selectedFile));
        filename = selectedFile;
        open(filename);
    }
}

void IdeWidget::newFile() {
    if (document()->isModified()) {
        if (!saveWarning()) return;
    }
    document()->setModified(false);
    clear();
    saveAs();
}

bool IdeWidget::saveWarning() {
    int ans = QMessageBox::warning(
                this, tr("SimComp"),
                tr("The document has changes, do you want to save them?"),
                QMessageBox::Save | QMessageBox::Discard
                | QMessageBox::Cancel, QMessageBox::Save);
    switch (ans) {
        case QMessageBox::Cancel:
            return false;
        case QMessageBox::Save:
            save();
        case QMessageBox::Discard:
            return true;
    }
}
