#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include <QWidget>
#include <QPixmap>
#include <vector>
#include <string>

class MemoryMap : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryMap(QWidget *parent = nullptr);
    void setVector(const std::vector<std::string> &hexMap);
signals:
    void increaseSize();
public slots:
    void setPixelSize(int pixelSize);
    void setSize(QSize size);
    void setWidth(int width);
    void setHeight(int height);
    void paintEvent(QPaintEvent *event) override;
    void clear();
private:
    QPixmap* memoryPixmap;
    std::vector<std::string> hexMap;
    int pixelSize;
    int width;
    int height;
};

#endif // MEMORYMAP_H
