#include <QCoreApplication>
#include <QtCore>
#include <vaccleananalyzer.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int trackerID = atoi(argv[1]);

    VacCleanAnalyzer *vacCleanAnalyzer = new VacCleanAnalyzer(trackerID, &a);

    QObject::connect(vacCleanAnalyzer, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, vacCleanAnalyzer, SLOT(start()));

    return a.exec();
}
