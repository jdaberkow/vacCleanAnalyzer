#include "coverage.h"

#include <QDebug>
#include <QFile>
#include <QPen>

Coverage::Coverage(QObject *parent) : QObject(parent)
{

}

Coverage::Coverage(int width, int height, int diameterInCM, QObject *parent) : QObject(parent)
{
    this->width = width;
    this->height = height;
    this->diameterInPX = diameterInCM * 3;

    this->coverageImage = new QImage(width, height, QImage::Format_ARGB32);
    this->coverageImage->fill(Qt::transparent);

    this->scenarioImage = new QImage("scenario_with_walls.png", "png");

    this->coveragePainter = new QPainter(this->coverageImage);
    this->coveragePainter->setPen(QPen(Qt::green,this->diameterInPX,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

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
    int redSum = 0;

    if (!this->scenarioImage->isNull()) {
        for (int currentX = 0; currentX < width; ++currentX)
        {
            for (int currentY = 0; currentY < height; ++currentY)
            {
                if(QColor(this->scenarioImage->pixel(currentX, currentY)) == Qt::red){
                    redSum++;
                }
                else if (QColor(this->coverageImage->pixel(currentX, currentY)) == Qt::green) {
                    greenSum++;
                }
            }
        }
    } else {
        for (int currentX = 0; currentX < width; ++currentX)
        {
            for (int currentY = 0; currentY < height; ++currentY)
            {
                if (QColor(this->coverageImage->pixel(currentX, currentY)) == Qt::green) {
                    greenSum++;
                }
            }
        }
    }

    qreal total = width*height -redSum;
    qreal percentage = qreal(greenSum)/total;
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

void Coverage::exportScenarioAndCoverageImage(QString filename)
{
    QImage overlay = createImageWithOverlay(*this->coverageImage, *this->scenarioImage);
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    overlay.save(&file, "PNG");
}

QImage Coverage::createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage)
{
    QImage imageWithOverlay = QImage(baseImage.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&imageWithOverlay);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, baseImage);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, overlayImage);

    painter.end();

    return imageWithOverlay;
}

