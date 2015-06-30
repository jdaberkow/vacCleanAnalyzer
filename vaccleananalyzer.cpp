#include "vaccleananalyzer.h"
#include <QDebug>
#include <QFile>

#define IMAGE_WIDTH 2000
#define IMAGE_HEIGHT 2000
#define ROBOT_RADIUS 100

VacCleanAnalyzer::VacCleanAnalyzer(int trackerID, QObject *parent) : QObject(parent)
{
    this->trackerID = trackerID;
}

void VacCleanAnalyzer::start()
{
    qDebug() << "starting vacCleanAnalyzer...";

    this->coverageWorker = new Coverage(IMAGE_WIDTH, IMAGE_HEIGHT, ROBOT_RADIUS);
    this->distanceWorker = new Distance();

    /* import tracking data
     * the QMap is ordered by the timestamps from the file,
     * the QVector holds 3 Values, the x, y and theta value
    */
    QMap<int, QVector<int> > trackingData = this->preprocessTrackingData();

    //TODO: calculate model (calculate pixels in image which are static objects?)
    //use scenarioWorker.function(...)

    //Iterate over every line and calculate needed data successively
    int counter = 0;
    QMapIterator<int, QVector<int> > trackingDataIterator(trackingData);
    while (trackingDataIterator.hasNext()) {
        counter++;
        trackingDataIterator.next();

        this->coverageWorker->updateCoverage(&(trackingDataIterator.value()));
        this->distanceWorker->updateDistance(&(trackingDataIterator.value()));
        //this->durationWorker->updateDuration(&(trackingDataIterator.value()));

        //if coverage is at 50%, 60%, 70%, 80%, 90%....
        //then print distance and duration

        //DEBUG: print messages every 50 tracking points
        qreal coveragePercent = this->coverageWorker->getCurrentCoveragePercent();
        if (counter % 50 == 0) {
            qDebug() << "progressed" << counter << "tracking points so far...";
            qDebug() << "Current Coverage:" << coveragePercent << "% | Current Distance:" << this->distanceWorker->getCurrentDistance();
        }
    }
    //end while

    //TODO: calculate average distance to walls
    //use wallDistanceWorker->function(...)

    qDebug() << "\nFinished calculations.";
    qDebug() << "Total Trackpoints:" << counter;
    qDebug() << "Total Coverage:   " << this->coverageWorker->getCurrentCoveragePercent();
    qDebug() << "Total Distance:   " << this->distanceWorker->getCurrentDistance();

    qDebug() << "\nExporting Coverage Image...";
    this->coverageWorker->exportCurrentCoverageImage();

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

            if(splitLine.length() > 1 && splitLine.at(1).toInt() != 0)
            {
                for (int trackerCount = 0; trackerCount < splitLine.at(1).toInt(); ++trackerCount)
                {
                    int stringListIndex = 2 + trackerCount * 4;

                    if(splitLine.at(stringListIndex).toInt() == this->trackerID)
                    {
                        QVector<int> position(3);
                        position[0] = splitLine.at(stringListIndex + 2).toInt() + xOffset; //X
                        position[1] = splitLine.at(stringListIndex + 3).toInt() + yOffset; //Y
                        position[2] = splitLine.at(stringListIndex + 1).toInt(); //Theta
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
