#include "fileviewer.h"
#include "globals.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QSettings>
#include <QVBoxLayout>
#include <QLabel>

FileViewer::FileViewer(QStringList nameFilters, QWidget *parent) :
    QWidget(parent), nameFilters(nameFilters), filename("") {
    createModel();
    createView();
    filenameLbl = new QLabel("<b>File: </b>");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(filenameLbl);
    layout->addWidget(view);
    setLayout(layout);
    setMinimumSize(QSize(globals::FILEVIEWER_MIN_WIDTH,
                         globals::FILEVIEWER_MIN_HEIGHT));
}

void FileViewer::createModel() {
    QSettings settings;
    const QString DEFAULT_DIR_KEY("\\");
    model = new QFileSystemModel;
    model->setRootPath(settings.value(DEFAULT_DIR_KEY).toString());
    model->setNameFilters(nameFilters);
}

void FileViewer::createView() {
    view = new QTreeView;
    view->setModel(model);
    view->setRootIndex(model->index(model->rootPath()));
    connect(view, &QTreeView::activated, [this] (const QModelIndex &index) {
        if (view->currentIndex() == index)
            emit changeFile(this->model->rootPath() +
                            "/" + this->model->fileName(index));
    });
}

void FileViewer::setRootPath(QString path) {
    QSettings settings;
    const QString DEFAULT_DIR_KEY("\\");
    settings.setValue(DEFAULT_DIR_KEY, path);
    view->setRootIndex(model->setRootPath(path));
}

void FileViewer::setFilename(QString filename) {
    this->filename = filename;
    filenameLbl->setText("<b> File: </b>"+filename);
}
