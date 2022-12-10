#ifndef COMPTEUR_H
#define COMPTEUR_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QtMqtt/QMqttClient>
#include <QStringList>

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

    int nbEtiquettes;

    int pSout;
    int pInj;
    int pInst;
    int pInstPrec=-9999;
    int pConso;
    int pConsoPrec=-9999;
    int iInst;
    int iInstPrec=-9999;
    long indexInj;
    long indexInjPrec=-9999;
    int pSolaire=0;
    bool injection;


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
