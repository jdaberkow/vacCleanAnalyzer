#ifndef VACCLEANANALYZER_H
#define VACCLEANANALYZER_H

#include <QObject>
#include "scenariomodel.h"
#include "coverage.h"
#include "distance.h"
#include "duration.h"
#include "walldistance.h"

class VacCleanAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit VacCleanAnalyzer(int trackerID, QObject *parent = 0);

    void extractData(QMap<qulonglong, QVector<int> > *trackingData, QString fileName, int xOffset, int yOffset);
signals:
    void finished();

public slots:
    void start();

private:
    ScenarioModel scenarioWorker;
    Coverage coverageWorker;
    Distance distanceWorker;
    Duration durationWorker;
    WallDistance wallDistanceWorker;
    QMap<qulonglong, QVector<int> > preprocessTrackingData();
    int trackerID;
};

#endif // VACCLEANANALYZER_H
