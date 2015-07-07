#include "duration.h"

Duration::Duration(QObject *parent) : QObject(parent)
{
    this->duration = 0;
    this->firstTimestamp.tm_hour = 0;
    this->firstTimestamp.tm_min = 0;
    this->firstTimestamp.tm_sec = 0;
}

void Duration::updateDuration(int timestamp)
{
    //int milliseconds = timestamp % 100;
//    int seconds = (timestamp/100) % 100;
//    int minutes = (timestamp/10000) % 100;
//    int hours = (timestamp/1000000) % 100;
    int seconds = (timestamp) % 100;
    int minutes = (timestamp/100) % 100;
    int hours = (timestamp/10000) % 100;
    struct tm actualTime;
    actualTime.tm_hour = hours;
    actualTime.tm_min = minutes;
    actualTime.tm_sec = seconds;
    if(this->firstTimestamp.tm_hour == 0){
        this->firstTimestamp.tm_hour = hours;
        this->firstTimestamp.tm_min = minutes;
        this->firstTimestamp.tm_sec = seconds;
    }else{
        duration = difftime(mktime(&actualTime),mktime (&firstTimestamp));
    }
}

double Duration::getCurrentDuration()
{
    return duration;
}

