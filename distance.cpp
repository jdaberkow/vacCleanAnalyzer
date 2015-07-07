#include "distance.h"
#include <QVector>
#include <math.h>

Distance::Distance(QObject *parent) : QObject(parent)
{
    this->prevX = 0;
    this->prevY = 0;
    this->distance = 0.0;
    this->firstData = true;
}

void Distance::updateDistance(const QVector<int> *data)
{
    if(this->firstData) {
        this->prevX = data->at(0);
        this->prevY = data->at(1);
        this->firstData = false;
    } else {
        int deltaX = data->at(0) - prevX;
        int deltaY = data->at(1) - prevY;
        float newDistance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
        this->distance += newDistance;

        this->prevX = data->at(0);
        this->prevY = data->at(1);
    }
}

float Distance::getCurrentDistance()
{
    return distance;
}

float Distance::getDistanceInM()
{
    return (this->distance/180*60)/100;
}

