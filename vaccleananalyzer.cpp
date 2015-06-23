#include "vaccleananalyzer.h"
#include <QDebug>

VacCleanAnalyzer::VacCleanAnalyzer(QObject *parent) : QObject(parent)
{

}

void VacCleanAnalyzer::start()
{
    qDebug() << "starting vacCleanAnalyzer...";

    //TODO: import image

    //TODO: calculate model (calculate pixels in image which are static objects)
    //use scenarioWorker.function(...)

    //while data available
        //TODO: calculate % covered
        //use coverageWorker.function(...)

        //TODO: calculate duration until % covered
        //use durationWorker.function(...)

        //TODO: calculate distance until % covered
        //use distanceWorker.function(...)
    //end while

    //TODO: calculate average distance to walls
    //use wallDistanceWorker.function(...)

    emit finished();
}
