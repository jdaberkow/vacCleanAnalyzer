#ifndef COVERAGE_H
#define COVERAGE_H

#include <QObject>
#include <QImage>
#include <QPainter>
#include <QPoint>

class Coverage : public QObject
{
    Q_OBJECT
public:
    explicit Coverage(QObject *parent = 0);
    explicit Coverage(int height, int width, int diameterInCM, QObject *parent = 0);

    void updateCoverage(const QVector<int> *data);
    qreal getCurrentCoveragePercent();
    QImage *getCurrentCoverageImage();
    void exportCurrentCoverageImage();
    void exportScenarioAndCoverageImage(QString filename);

signals:

public slots:

private:
    QImage *coverageImage;
    QPainter *coveragePainter;

    QImage *scenarioImage;
    QImage createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage);

    QPoint prevPoint;
    bool prevPointSet;

    int height, width, diameterInPX;
};

#endif // COVERAGE_H
