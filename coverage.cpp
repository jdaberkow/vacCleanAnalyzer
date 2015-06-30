#include "coverage.h"

#include <QDebug>
#include <QFile>
#include <QPen>

Coverage::Coverage(QObject *parent) : QObject(parent)
{

}

Coverage::Coverage(int width, int height, int diameter, QObject *parent) : QObject(parent)
{
    this->width = width;
    this->height = height;
    this->diameter = diameter;

    this->coverageImage = new QImage(width, height, QImage::Format_ARGB32);
    this->coverageImage->fill(Qt::transparent);

    this->coveragePainter = new QPainter(this->coverageImage);
    this->coveragePainter->setPen(QPen(Qt::green,diameter,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

    this->prevPoint = QPoint(0, 0);
    this->prevPointSet = false;
}

void Coverage::updateCoverage(const QVector<int> *data)
{
    if (this->prevPointSet)
    {
        QPoint newPoint(data->at(0), data->at(1));
        this->coveragePainter->drawLine(this->prevPoint, newPoint);
        this->prevPoint = newPoint;
    }
    else
    {
        this->prevPoint = QPoint(data->at(0), data->at(1));
        this->prevPointSet = true;
    }
}

qreal Coverage::getCurrentCoveragePercent()
{
    int greenSum = 0;

    for (int currentX = 0; currentX < width; ++currentX)
    {
        for (int currentY = 0; currentY < height; ++currentY)
        {
            if (QColor(this->coverageImage->pixel(currentX, currentY)) == Qt::green) {
                greenSum++;
            }
        }
    }

    qreal percentage = qreal(greenSum)/qreal(width*height);
    percentage = percentage * 10000;
    percentage = qreal(qRound(percentage)) / 100;
    return percentage;
}

QImage* Coverage::getCurrentCoverageImage()
{
    return this->coverageImage;
}

void Coverage::exportCurrentCoverageImage()
{
    QFile file("coverage.png");
    file.open(QIODevice::WriteOnly);
    this->coverageImage->save(&file, "PNG");
}

