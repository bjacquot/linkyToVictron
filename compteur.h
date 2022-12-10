#ifndef COMPTEUR_H
#define COMPTEUR_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QtMqtt/QMqttClient>
#include <QStringList>

#include "qem24energymeter.h"

class Compteur : public QObject
{
    Q_OBJECT
public:
    explicit Compteur(QObject *parent = nullptr);

private:
    QSerialPort serial;
    QTimer retryOpenSerial;
    QTimer timerReadData;

    bool isConnected;
    QMqttClient m_client;
    QTimer retryMqttTimer;
    QMqttSubscription * souscription;


    QStringList listeEtiquette;
    QStringList listeValeur;
    QStringList listeNewValeur;
    QStringList listeTopic;
    QList<int> listeDiv;
    QList<int> listeArrondi;


    QEM24EnergyMeter energyMeter;
    bool isEnergyMeterStarted=false;


    int nbEtiquettes;

    int pSout;
    int pInj;
    int pInst;
    int pInstPrec=-9999;
    int pConso;
    int pConsoPrec=-9999;
    int iInst;
    int iInstPrec=-9999;
    int uInst=230;
    int uInstPrec=0;
    long indexInj;
    long indexInjPrec=-9999;
    int pSolaire=0;
    bool injection;
    int64_t indexHC=0;
    int64_t indexHCprec=0;
    int64_t indexHP=0;
    int64_t indexHPprec=0;
    int64_t indexTotal=0;
    int64_t indexTotalprec=0;



signals:

public slots:
    void serialError();
    void getData();

    void mqttConnected();
    void mqttDisConnected();
    void mqttReconnect();
    void mqttSend(QString topic, QString message,int qos,bool retain);
    void newPuissanceSolaire(QMqttMessage message);
    void addHomeAssistant(QString name, QString type, QString unit);

};

#endif // COMPTEUR_H
