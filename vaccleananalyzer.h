#ifndef VACCLEANANALYZER_H
#define VACCLEANANALYZER_H

#include <QObject>

class VacCleanAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit VacCleanAnalyzer(QObject *parent = 0);

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
};

#endif // VACCLEANANALYZER_H
