#ifndef DURATION_H
#define DURATION_H

#include <QObject>
#include <time.h>

class Duration : public QObject
{
    Q_OBJECT
public:
    explicit Duration(QObject *parent = 0);

    void updateDuration(int timestamp);
    double getCurrentDuration();

signals:

public slots:

private:
    struct tm firstTimestamp;
    double duration;
};

#endif // DURATION_H
