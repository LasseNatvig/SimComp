#include "performancechart.h"
#include "globals.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

PerformanceChart::PerformanceChart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags), x(0), y(0),
    yRange(globals::PLOT_YRANGE) {
    // Create new series
    newSeries();

    // Chart configuration
    setMinimumSize(QSize(globals::PERFORMANCEWINDOW_MIN_WIDTH,
                         globals::PERFORMANCEWINDOW_MIN_HEIGHT));
    setMaximumSize(QSize(globals::PERFORMANCEWINDOW_MIN_WIDTH,
                         globals::PERFORMANCEWINDOW_MIN_HEIGHT));
    legend()->hide();
}

PerformanceChart::~PerformanceChart() {
    delete series;
    series = nullptr;
}

void PerformanceChart::newSeries() {
    axis = new QValueAxis;
    series = new QLineSeries;

    QPen pen(Qt::red);
    pen.setWidth(1);
    series->setPen(pen);
    series->append(x, y);

    addSeries(series);
    createDefaultAxes();
    setAxisX(axis, series);
    axis->setTickCount(globals::PLOT_XRANGE);

    axisX()->setRange(0, globals::PLOT_XRANGE);
    axisY()->setRange(0, yRange);
    axisX()->setTitleText("Time elapsed [s]");
    axisY()->setTitleText("MIPS");
}

void PerformanceChart::updatePerformance(double mips) {
    double dx = static_cast<double>(globals::TIMER_UPDATE)/
            static_cast<double>(1000);
    x += dx;
    y = mips;
    double tick_dx = plotArea().width() / axis->tickCount()*dx;
    if (x > globals::PLOT_XRANGE) // Start scrolling when plot is out of range
        scroll(tick_dx,0);

    series->append(x, y);
    if (mips > yRange) {
        yRange = mips;
        axisY()->setRange(0, yRange);
    }
}

void PerformanceChart::reset() {
    x = 0;
    y = 0;
    delete axis;
    delete series;
    newSeries();
    yRange = globals::PLOT_YRANGE;
    axisY()->setRange(0, yRange);
}
