#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include <QWidget>
#include <vector>
#include <string>

class MemoryMap : public QWidget {
    Q_OBJECT
public:
    explicit MemoryMap(QWidget *parent = nullptr);

signals:
    void increaseSize();

public slots:
    void setVector(const std::vector<std::string> &hexMap);
    void setPixelSize(int pixelSize);
    void setSize(QSize size);
    void setWidth(int width);
    void setHeight(int height);   
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<std::string> hexMap;
    int pixelSize;
    int width;
    int height;
};

#endif // MEMORYMAP_H
