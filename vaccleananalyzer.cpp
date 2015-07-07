#include "vaccleananalyzer.h"
#include <QDebug>
#include <QFile>

VacCleanAnalyzer::VacCleanAnalyzer(int trackerID, QObject *parent) : QObject(parent)
{
    this->trackerID = trackerID;
}

void VacCleanAnalyzer::start()
{
    qDebug() << "starting vacCleanAnalyzer...";
    //qDebug() << "Duration in Seconds: " << this->durationWorker.getCurrentDuration();
    /* import tracking data
     * the QMap is ordered by the timestamps from the file,
     * the QVector holds 3 Values, the x, y and theta value
    */
    QMap<qulonglong, QVector<int> > trackingData = this->preprocessTrackingData();

    //TODO: calculate model (calculate pixels in image which are static objects?)
    //use scenarioWorker.function(...)

    //Iterate over every line and calculate needed data successively
    QMapIterator<qulonglong, QVector<int> > trackingDataIterator(trackingData);
    while (trackingDataIterator.hasNext()) {
        trackingDataIterator.next();

        //this->coverageWorker.updateCoverage(&(trackingDataIterator.value()));
        this->distanceWorker.updateDistance(&(trackingDataIterator.value()));
        qDebug() << "Distance: " << this->distanceWorker.getCurrentDistance();
        //this->durationWorker.updateDuration(trackingDataIterator.key());
        //this->durationWorker.updateDistance(&(trackingDataIterator.value()));

        //if coverage is at 50%, 60%, 70%, 80%, 90%....
        //then print distance and duration
    }
    //end while

    qDebug() << "Duration in Seconds: " << this->durationWorker.getCurrentDuration();
    //TODO: calculate average distance to walls
    //use wallDistanceWorker.function(...)

    emit finished();
}

void VacCleanAnalyzer::extractData(QMap<qulonglong, QVector<int> > *trackingData, QString filename, int xOffset, int yOffset)
{
    QFile dataCam(filename);
    if (dataCam.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!dataCam.atEnd())
        {
            QString line = dataCam.readLine();
            QStringList splitLine = line.simplified().split(' ', QString::SkipEmptyParts);

            if(splitLine.length() > 5 && splitLine.at(1).toInt() != 0)
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
                        QString tmp = splitLine.at(0);
                        trackingData->insert((tmp.remove(QChar('.'), Qt::CaseSensitive)).toULongLong(), position);
                    }
                }
            }
        }
        dataCam.close();
    }
}

QMap<qulonglong, QVector<int> > VacCleanAnalyzer::preprocessTrackingData(){
    QMap<qulonglong, QVector<int> > trackingData;
    extractData(&trackingData, "CAMERA_1_tracking_data.txt", 1000, 0);
    extractData(&trackingData, "CAMERA_2_tracking_data.txt", 0, 0);
    extractData(&trackingData, "CAMERA_3_tracking_data.txt", 0, 1000);
    extractData(&trackingData, "CAMERA_4_tracking_data.txt", 1000, 1000);
    return trackingData;
}
