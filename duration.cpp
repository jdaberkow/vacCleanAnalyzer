#include "duration.h"

Duration::Duration(QObject *parent) : QObject(parent)
{
    this->duration = 0;
    this->first = true;

    this->firstHour = 0;
    this->firstMin = 0;
    this->firstSec = 0;

    this->currHour = 0;
    this->currMin = 0;
    this->currSec = 0;
}

void Duration::updateDuration(const qulonglong *timestamp)
{
    qulonglong time = *timestamp;
    if (this->first) {
        this->firstHour = time / qulonglong(10000000000);
        time -= this->firstHour * qulonglong(10000000000);
        this->firstMin = time / qulonglong(100000000);
        time -= this->firstMin * qulonglong(100000000);
        this->firstSec = time / qulonglong(1000000);

        this->first = false;
    } else {
        this->currHour = time / qulonglong(10000000000);
        time -= this->currHour * qulonglong(10000000000);
        this->currMin = time / qulonglong(100000000);
        time -= this->currMin * qulonglong(100000000);
        this->currSec = time / qulonglong(1000000);
    }
}

int Duration::getCurrentDuration()
{
    this->duration = 0;
    if (this->currSec >= this->firstSec) {
        this->duration += this->currSec - this->firstSec;
    } else {
        this->duration += (this->currSec + 60) - this->firstSec;
        this->duration -= 60;
    }
    if (this->currMin >= this->currMin) {
        this->duration += (this->currMin - this->firstMin) * 60;
    } else {
        this->duration += ((this->currMin + 60) - this->firstMin) * 60;
        this->duration -= 60*60;
    }
    if (this->currHour >= this->firstHour) {
        this->duration += (this->currHour - this->firstHour) * 60 * 60;
    }
    return duration;
}

QString Duration::getFormattedDuration()
{
    int hour = 0;
    int min = 0;
    int sec = 0;

    if (this->currSec >= this->firstSec) {
        sec += this->currSec - this->firstSec;
    } else {
        sec += (this->currSec + 60) - this->firstSec;
        min -= 1;
    }
    if (this->currMin >= this->currMin) {
        min += (this->currMin - this->firstMin);
    } else {
        min += ((this->currMin + 60) - this->firstMin);
        hour -= 1;
    }
    if (min < 0) {
        min += 60;
        hour -= 1;
    }
    if (this->currHour >= this->firstHour) {
        hour += (this->currHour - this->firstHour);
    }

    QString formattedHour = QString::number(hour);
    if (hour < 10) {
        formattedHour = "0" + formattedHour;
    }
    QString formattedMin = QString::number(min);
    if (min < 10) {
        formattedMin = "0" + formattedMin;
    }
    QString formattedSec = QString::number(sec);
    if (sec < 10) {
        formattedSec = "0" + formattedSec;
    }


    QString formattedDuration = formattedHour + ":" + formattedMin + ":" + formattedSec;
    return formattedDuration;
}

