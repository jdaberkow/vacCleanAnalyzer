#include <QCoreApplication>
#include <QtCore>
#include <vaccleananalyzer.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    VacCleanAnalyzer *vacCleanAnalyzer = new VacCleanAnalyzer(&a);

    QObject::connect(vacCleanAnalyzer, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, vacCleanAnalyzer, SLOT(start()));

    return a.exec();
}
