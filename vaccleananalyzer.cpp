#include "vaccleananalyzer.h"
#include <QDebug>
#include <iostream>
#include <string>
#include <QFile>

VacCleanAnalyzer::VacCleanAnalyzer(int trackerID, QObject *parent) : QObject(parent)
{
    this->trackerID = trackerID;
}

void VacCleanAnalyzer::start()
{
    qDebug() << "starting vacCleanAnalyzer...";

    /* import tracking data
     * the QMap is ordered by the timestamps from the file,
     * the QVector holds 3 Values, the x, y and theta value
    */
    QMap<int, QVector<int> > trackingData = this->preprocessTrackingData();


    //TODO: calculate model (calculate pixels in image which are static objects)
    //use scenarioWorker.function(...)

    //while data available
    QMapIterator<int, QVector<int> > i(trackingData);
    while (i.hasNext()) {
      i.next();
      qDebug() << i.key() << ": " << i.value();
      this->distanceWorker.updateDistance(&(i.value()));
      qDebug() << "Distance: " << this->distanceWorker.getCurrentDistance();
        //TODO: calculate % covered
        //use coverageWorker.function(...)

        //TODO: calculate duration until % covered
        //use durationWorker.function(...)

        //TODO: calculate distance until % covered
        //use distanceWorker.function(...)
    }
    //end while

    //TODO: calculate average distance to walls
    //use wallDistanceWorker.function(...)

    emit finished();
}

void VacCleanAnalyzer::extractData(QMap<int, QVector<int> > *trackingData, QString filename, int xOffset, int yOffset)
{
    QFile dataCam(filename);
    if (dataCam.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!dataCam.atEnd())
        {
            QString line = dataCam.readLine();
            QStringList splitLine = line.simplified().split(' ', QString::SkipEmptyParts);
            if(splitLine.length() > 1 && splitLine.at(1).toInt() != 0){
                for (int i = 0; i < splitLine.at(1).toInt(); ++i) {
                    if(splitLine.at(2+i*4).toInt() == this->trackerID){
                        QVector<int> position(3);
                        position[0] = splitLine.at(4+i*4).toInt() + xOffset; //X
                        position[1] = splitLine.at(5+i*4).toInt() + yOffset; //Y
                        position[2] = splitLine.at(3+i*4).toInt(); //Theta
                        trackingData->insert(splitLine.at(0).toInt(), position);
                    }
                }
            }
        }
        dataCam.close();
    }
}

QMap<int, QVector<int> > VacCleanAnalyzer::preprocessTrackingData(){
    QMap<int, QVector<int> > trackingData;
    extractData(&trackingData, "CAMERA_1_tracking_data.txt", 1000, 0);
    extractData(&trackingData, "CAMERA_2_tracking_data.txt", 0, 0);
    extractData(&trackingData, "CAMERA_3_tracking_data.txt", 0, 1000);
    extractData(&trackingData, "CAMERA_4_tracking_data.txt", 1000, 1000);
    return trackingData;
}
