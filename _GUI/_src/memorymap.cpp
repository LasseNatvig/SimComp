#include "memorymap.h"
#include "globals.h"
#include <QPainter>
#include <QColor>
#include <QRect>

MemoryMap::MemoryMap(QWidget *parent) : QWidget(parent),
    width(globals::MEMORYWINDOW_PIXMAP_WIDTH),
    height(globals::MEMORYWINDOW_PIXMAP_HEIGHT),
    pixelSize(globals::MEMORYWINDOW_PIXMAP_PIXSIZE) {
    setFixedSize(QSize(width, height));
}

void MemoryMap::setVector(const std::vector<std::string> &hexMap) {
    this->hexMap = hexMap;
    update();
}

void MemoryMap::setSize(QSize size) {
    width = size.width();
    height = size.height();
    setFixedSize(size);
}

void MemoryMap::setWidth(int width) {
    this->width = width;
    setFixedSize(QSize(width, height));
}

void MemoryMap::setHeight(int height) {
    this->height = height;
    setFixedSize(QSize(width, height));
}

void MemoryMap::setPixelSize(int pixelSize) {
    this->pixelSize = pixelSize;
}

void MemoryMap::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    int x = 0;
    int y = 0;
    for (auto& hexStr : hexMap) {
        QRect pixel(QPoint(x,y), QSize(pixelSize, pixelSize));
        painter.fillRect(pixel, QColor(std::stoul(hexStr, nullptr, 16),
                                       std::stoul(hexStr, nullptr, 16),
                                       std::stoul(hexStr, nullptr, 16)));
        x += pixelSize;
        if (x > width) {
            x = 0;
            y += pixelSize;
            if (y > height) {
                emit increaseSize();
                return;
            }
        }
    }
}

void MemoryMap::clear() {
    setVector(std::vector<std::string>());
    update();
}
