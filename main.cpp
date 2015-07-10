#include <QCoreApplication>
#include <QtCore>
#include <vaccleananalyzer.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc != 3) {
        std::cout << "Usage:\n        ./vacCleanAnalyzer <TrackerID> <Diameter of Robot in cm>" << std::endl;
        return -1;
    }

    int trackerID = atoi(argv[1]);
    int diameterInCM = atoi(argv[2]);

    VacCleanAnalyzer *vacCleanAnalyzer = new VacCleanAnalyzer(trackerID, diameterInCM, &a);

    QObject::connect(vacCleanAnalyzer, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, vacCleanAnalyzer, SLOT(start()));

    return a.exec();
}
