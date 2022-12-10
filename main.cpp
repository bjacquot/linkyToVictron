#include <QCoreApplication>
#include "compteur.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Compteur cpt(&a);
    return a.exec();
}
