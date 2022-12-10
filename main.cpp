#include <QCoreApplication>
#include "compteur.h"
#include "qem24energymeter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QEM24EnergyMeter energyMeter(&a);
    //Compteur cpt;
    return a.exec();
}
