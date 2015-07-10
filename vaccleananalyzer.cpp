#include "vaccleananalyzer.h"
#include <QDebug>
#include <QFile>
#include <iostream>

#define IMAGE_WIDTH 2000
#define IMAGE_HEIGHT 2000

VacCleanAnalyzer::VacCleanAnalyzer(int trackerID, int diameterInCM, QObject *parent) : QObject(parent)
{
    this->trackerID = trackerID;
    this->diameterInCM = diameterInCM;
}

void VacCleanAnalyzer::start()
{
    std::cout << "starting vacCleanAnalyzer..." << std::endl;

    this->coverageWorker = new Coverage(IMAGE_WIDTH, IMAGE_HEIGHT, this->diameterInCM);
    this->distanceWorker = new Distance();
    this->durationWorker = new Duration();

    /* import tracking data
     * the QMap is ordered by the timestamps from the file,
     * the QVector holds 3 Values, the x, y and theta value
    */
    QMap<qulonglong, QVector<int> > trackingData = this->preprocessTrackingData();

    //Iterate over every line and calculate needed data successively

    QMapIterator<qulonglong, QVector<int> > trackingDataIterator(trackingData);

    int counter = 0;
    int goal = 300;

    while (trackingDataIterator.hasNext()) {
        counter++;
        trackingDataIterator.next();

        this->durationWorker->updateDuration(&(trackingDataIterator.key()));
        this->coverageWorker->updateCoverage(&(trackingDataIterator.value()));
        this->distanceWorker->updateDistance(&(trackingDataIterator.value()));

        //print messages and export image every 5 minutes
        if (this->durationWorker->getCurrentDuration() >= goal) {
            std::cout << "\n########################\nAfter " << goal/60 << " minutes:\n" << std::endl;
            std::cout << "Trackpoints: " << counter << std::endl;
            std::cout << "Coverage:    " << this->coverageWorker->getCurrentCoveragePercent() << " %" << std::endl;
            std::cout << "Distance:    " << this->distanceWorker->getCurrentDistance() << " px" << std::endl;
            std::cout << "             " << this->distanceWorker->getDistanceInM() << " m" << std::endl;
            std::cout << "Duration:    " << this->durationWorker->getCurrentDuration() << " sec" << std::endl;
            std::cout << "             " << this->durationWorker->getFormattedDuration().toStdString() << " (hh:mm:ss)" << std::endl;

            QString minuteString = QString::number(this->durationWorker->getCurrentDuration() / 60);
            if (minuteString.size() == 1) {
                minuteString.push_front(QString("00"));
            } else if (minuteString.size() == 2) {
                minuteString.push_front(QString("0"));
            }

            this->coverageWorker->exportScenarioAndCoverageImage(QString("scenarioAndCoverage_") + minuteString + QString("m.png"));

            goal += 300;
        }
    }

    //TODO: calculate average distance to walls
    //use wallDistanceWorker->function(...)

    std::cout << "\n########################\nFinished calculations.\n" << std::endl;
    std::cout << "Total Trackpoints: " << counter << std::endl;
    std::cout << "Total Coverage:    " << this->coverageWorker->getCurrentCoveragePercent() << " %" << std::endl;
    std::cout << "Total Distance:    " << this->distanceWorker->getCurrentDistance() << " px" << std::endl;
    std::cout << "                   " << this->distanceWorker->getDistanceInM() << " m" << std::endl;
    std::cout << "Total Duration:    " << this->durationWorker->getCurrentDuration() << " sec" << std::endl;
    std::cout << "                   " << this->durationWorker->getFormattedDuration().toStdString() << " (hh:mm:ss)" << std::endl;

    std::cout << "\nExporting Coverage Image..." << std::endl;
    this->coverageWorker->exportCurrentCoverageImage();

    std::cout << "Exporting combined Scenario&Coverage Images in 5 minute steps..." << std::endl;
    this->coverageWorker->exportScenarioAndCoverageImage(QString("scenarioAndCoverage_final.png"));

    std::cout << "\nInsert your data into the following spreadsheet:" << std::endl;
    std::cout << "    https://docs.google.com/spreadsheets/d/1Hql7qDrgEm0oKqIBd6pix0cMSshNFj__7QqPNpNRtXE/edit?usp=sharing\n" << std::endl;

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
                        position[0] = splitLine.at(stringListIndex + 2).toInt() * 1.03 + xOffset; //X
                        position[1] = splitLine.at(stringListIndex + 3).toInt() * 1.03 + yOffset; //Y
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
    extractData(&trackingData, "CAMERA_1_tracking_data.txt", 975, 0);
    extractData(&trackingData, "CAMERA_2_tracking_data.txt", 25, 0);
    extractData(&trackingData, "CAMERA_3_tracking_data.txt", 25, 960);
    extractData(&trackingData, "CAMERA_4_tracking_data.txt", 975, 960);
    return trackingData;
}
