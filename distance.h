#ifndef DISTANCE_H
#define DISTANCE_H

#include <QObject>

class Distance : public QObject
{
    Q_OBJECT
public:
    explicit Distance(QObject *parent = 0);

    void updateDistance(const QVector<int> *data);
    float getCurrentDistance();
    float getDistanceInM();

signals:

public slots:

private:
    bool firstData;
    int prevX;
    int prevY;
    float distance;
};

#endif // DISTANCE_H
