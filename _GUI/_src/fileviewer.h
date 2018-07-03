#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QWidget>

class QTreeView;
class QFileSystemModel;
class QLabel;

class FileViewer : public QWidget
{
    Q_OBJECT
public:
    explicit FileViewer(QStringList nameFilters ,QWidget *parent = nullptr);

signals:
    void changeFile(QString filename);

public slots:
    void setRootPath(QString path);
    void setFilename(QString filename);
private:
    QFileSystemModel* model;
    QTreeView* view;
    QStringList nameFilters;
    QLabel* filenameLbl;
    QString filename;

    void createModel();
    void createView();
};

#endif // FILEVIEWER_H
