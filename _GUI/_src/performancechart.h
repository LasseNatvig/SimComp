#ifndef PERFORMANCECHART_H
#define PERFORMANCECHART_H

// SEE https://doc-snapshots.qt.io/qt5-5.10/qtcharts-dynamicspline-example.html

#include <QChart>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class PerformanceChart : public QChart {
public:
    PerformanceChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    ~PerformanceChart();

public slots:
    void updatePerformance(double mips);
    void reset();
    void newSeries();

private:
    QLineSeries *series;
    QStringList titles;
    QValueAxis *axis;
    double x;
    double y;
    int yRange;
};

#endif // PERFORMANCECHART_H
