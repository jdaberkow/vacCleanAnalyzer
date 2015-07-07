#ifndef DURATION_H
#define DURATION_H

#include <QObject>
#include <time.h>

class Duration : public QObject
{
    Q_OBJECT
public:
    explicit Duration(QObject *parent = 0);

    void updateDuration(const qulonglong* timestamp);
    int getCurrentDuration();
    QString getFormattedDuration();

signals:

public slots:

private:
    int duration;

    bool first;

    int firstHour;
    int firstMin;
    int firstSec;

    int currHour;
    int currMin;
    int currSec;
};

#endif // DURATION_H
