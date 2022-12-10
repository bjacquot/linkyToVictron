#include "compteur.h"
#include <QDebug>
#include <QString>

/*
PJOURF+1        0000C001 07008002 1700C001 NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE U
  ADSC  812061050715    1
VTIC    02      J
DATE    H211223140802           ;
NGTF    H PLEINE/CREUSE         \
LTARF    HEURE  PLEINE          A
EAST    018063371       ,
EASF01  009242478       F
EASF02  008820893       I
EASF03  000000000       $
EASF04  000000000       %
EASF05  000000000       &
EASF06  000000000       '
EASF07  000000000       (
EASF08  000000000       )
EASF09  000000000       *
EASF10  000000000       "
EASD01  009242478       D
EASD02  008820893       G
EASD03  000000000       "
EASD04  000000000       #
EAIT    002487735       )
ERQ1    000002119       H
ERQ2    000001142       D
ERQ3    000325124       N
ERQ4    002330833       T
IRMS1   008     6
URMS1   235     D
PREF    12      B
PCOUP   12      \
SINSTS  01905   U
SMAXSN  H211223042418   11190   7
SMAXSN-1        H211222054856   14280
SINSTI  00000   <
SMAXIN  H211223111510   01730   "
SMAXIN-1        H211222000000   00000   +
CCASN   H211223140000   00454   8
CCASN-1 H211223133000   00310   O
UMOY1   H211223140000   237     *
STGE    003A4501        C
MSG1         PAS DE          MESSAGE            <
PRM     04520405165913  .
RELAIS  000     B
NTARF   02      O
NJOURF  00      &
NJOURF+1        00      B
PJOURF+1        0000C001 07008002 1700C001 NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE U
  ADSC  812061050715    1
VTIC    02      J
DATE    H211223140803           <
NGTF    H PLEINE/CREUSE         \
LTARF    HEURE  PLEINE          A
EAST    018063373       .
EASF01  009242478       F
EASF02  008820895       K
EASF03  000000000       $
EASF04  000000000       %
EASF05  000000000       &
EASF06  000000000       '
EASF07  000000000       (
EASF08  000000000       )
EASF09  000000000       *
EASF10  000000000       "
EASD01  009242478       D
EASD02  008820895       I
EASD03  000000000       "
EASD04  000000000       #
EAIT    002487735       )
ERQ1    000002119       H
ERQ2    000001142       D
ERQ3    000325124       N
ERQ4    002330833       T
IRMS1   008     6
URMS1   235     D
PREF    12      B
PCOUP   12      \
SINSTS  01929   [
SMAXSN  H211223042418   11190   7
SMAXSN-1        H211222054856   14280
SINSTI  00000   <
SMAXIN  H211223111510   01730   "
SMAXIN-1        H211222000000   00000   +
CCASN   H211223140000   00454   8
CCASN-1 H211223133000   00310   O
UMOY1   H211223140000   237     *
STGE    003A4501        C
MSG1         PAS DE          MESSAGE            <
PRM     04520405165913  .
RELAIS  000     B
NTARF   02      O
NJOURF  00      &
NJOURF+1        00      B
PJOURF+1        0000C001 07008002 1700C001 NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE U
  ADSC  812061050715    1









ADSC  812061050715    1
VTIC    02      J
DATE    E210610220556           <
NGTF    H PLEINE/CREUSE         \
LTARF    HEURE  PLEINE          A
EAST    012060129       $
EASF01  005811879       I
EASF02  006248250       >
EASF03  000000000       $
EASF04  000000000       %
EASF05  000000000       &
EASF06  000000000       '
EASF07  000000000       (
EASF08  000000000       )
EASF09  000000000       *
EASF10  000000000       "
EASD01  005811878       F
EASD02  006248251       =
EASD03  000000000       "
EASD04  000000000       #
IRMS1   003     1
URMS1   236     E
PREF    12      B
PCOUP   12      \
SINSTS  00725   T
SMAXSN  E210610000934   05560   4
SMAXSN-1        E210609233927   05570   %
CCASN   E210610220000   00630   /
CCASN-1 E210610213000   00512   N
UMOY1   E210610220000   236     $
STGE    003A4401        B
MSG1         PAS DE          MESSAGE            <
PRM     04520405165913  .
RELAIS  000     B
NTARF   02      O
NJOURF  00      &
NJOURF+1        00      B
PJOURF+1        0000C001 06368002 1636C001 NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE NONUTILE      %

 */


Compteur::Compteur(QObject *parent)
    : QObject(parent), serial(this),timerReadData(this)
{
    serial.setPortName("domotiqueCompteur");
    serial.setBaudRate(9600);
    serial.setDataBits(QSerialPort::Data7);
    serial.setParity(QSerialPort::EvenParity);
    retryOpenSerial.setInterval(500);
    retryOpenSerial.setSingleShot(true);
    timerReadData.setInterval(400);

    connect(&retryOpenSerial,&QTimer::timeout,this,&Compteur::serialError);
    connect(&timerReadData,&QTimer::timeout,this,&Compteur::getData);
    //connect(&serial,&QSerialPort::readyRead,this,&Compteur::getData);


    connect(&m_client,SIGNAL(connected()),this,SLOT(mqttConnected()));
    connect(&m_client,SIGNAL(disconnected()),this,SLOT(mqttDisConnected()));
    connect(&retryMqttTimer,SIGNAL(timeout()),this,SLOT(mqttReconnect()));

    listeEtiquette.append("EASF01");
    listeTopic.append("fluide/elec/hc");
    listeValeur.append("");
    listeNewValeur.append("");
    listeDiv.append(100);
    listeArrondi.append(0);

    listeEtiquette.append("EASF02");
    listeTopic.append("fluide/elec/hp");
    listeValeur.append("");
    listeNewValeur.append("");
    listeDiv.append(100);
    listeArrondi.append(0);

    listeEtiquette.append("PTEC");
    listeTopic.append("fluide/elec/tarif");
    listeValeur.append("");
    listeNewValeur.append("");
    listeDiv.append(0);
    listeArrondi.append(0);


    nbEtiquettes=listeEtiquette.size();

    m_client.setHostname(QString("iot"));
    m_client.setPort(1883);

    retryMqttTimer.setInterval(100);
    retryMqttTimer.start();
    souscription=nullptr;


    retryOpenSerial.start();
}

void Compteur::serialError()
{
    disconnect(&serial,&QSerialPort::errorOccurred,this,&Compteur::serialError);
    timerReadData.stop();
    qDebug()<<"tentative d'ouverture de la liaison serie";
    serial.close();
    if (serial.open(QIODevice::ReadOnly)==false)
    {

        qDebug()<<"echec";
        retryOpenSerial.start();
        serial.close();
    }
    else
    {
        retryOpenSerial.stop();
        qDebug()<<"ok";
        connect(&serial,&QSerialPort::errorOccurred,this,&Compteur::serialError);
        timerReadData.start();
    }

}

void Compteur::getData()
{
    QString line;
    static bool start=false;
    bool run=true;
    char c='\0',pc;
    char crc=0;
    do
    {
        pc=c;
        if (serial.getChar(&c))
        {
            if (c=='\n')
            {
                line.clear();
                crc=0;
            }
            else if (c=='\r')
            {
                crc-=pc;
                crc=(crc&0x3F)+0x20;
                QStringList liste = line.split(QLatin1Char('\t'));
                //qDebug()<<liste<<" "<<crc;

                if (crc==liste.last().at(0))
                {
                    //qDebug()<<"crc ok";

                    if (liste.at(0)=="DATE")
                    {
                        //qDebug()<<"start";
                        if (start==false) start=true;
                        else
                        {
                            if (injection==true)
                            {
                                if (iInst>0) iInst=-iInst;
                                pInst=-pInj;
                            }
                            else
                            {
                                if (iInst<0) iInst=-iInst;
                                pInst=pSout;
                            }
/*                            if (iInst!=iInstPrec)
                            {
                                iInstPrec=iInst;
                                //qDebug()<<"topic"<<"fluide/elec/iinst"<<iInst;
                                mqttSend("fluide/elec/iinst",QString::number(iInst),1,true);
                            }
*/
                            if (pInst!=pInstPrec)
                            {
                                pInstPrec=pInst;
                                //qDebug()<<"topic"<<"fluide/elec/puissance"<<pInst;
                                mqttSend("homeassistant/sensor/teleinfoPinst/state",QString::number(pInst),1,true);
                            }
                            pConso=pInst+pSolaire;
                            if (pConso!=pConsoPrec)
                            {
                                pConsoPrec=pConso;
                                mqttSend("homeassistant/sensor/teleinfoPconso/state",QString::number(pConso),1,true);
                            }
                            if (indexInj!=indexInjPrec)
                            {
                                indexInjPrec=indexInj;
                                mqttSend("homeassistant/sensor/teleinfoIndexInjection/state",QString::number(indexInj/10)+'.'+QString::number(indexInj%10),1,true);
                            }

                            for(int i=0;i<nbEtiquettes;i++)
                            {
                                if (listeValeur.at(i)!=listeNewValeur.at(i))
                                {
                                    listeValeur[i]=listeNewValeur.at(i);
                                    //qDebug()<<"topic"<<listeTopic.at(i)<<listeValeur[i];
                                    mqttSend(listeTopic[i],listeValeur[i],1,true);
                                }
                            }
                        }
                    }
                    else if (liste.at(0)=="IRMS1")
                    {
                        iInst=liste.at(1).toInt();
                    }
                    else if (liste.at(0)=="SINSTS")
                    {
                        pSout=liste.at(1).toInt();
                        //if (pInst==0) injection=true; else injection=false;
                        pSout=(pSout/50)*50;
                    }
                    else if (liste.at(0)=="SINSTI")
                    {
                        pInj=liste.at(1).toInt();
                        if (pInj!=0) injection=true; else injection=false;
                        pInj=(pInj/50)*50;
                    }
                    else if (liste.at(0)=="EAIT")
                    {
                        indexInj=liste.at(1).toLong()/100;
                    }
                    else
                    {
                        int i=listeEtiquette.indexOf(liste.at(0));
                        if (i>=0)
                        {
                            listeNewValeur[i] = liste.at(1);
                            int div=listeDiv.at(i);
                            if (div!=0) listeNewValeur[i]=QString::number((listeNewValeur[i].toInt()/div));
                            int arrondi=listeArrondi.at(i);
                            if (arrondi!=0) listeNewValeur[i]=QString::number(( ((listeNewValeur[i].toInt())/arrondi)*arrondi));
                        }
                    }
                }
            }

            else
            {
                line.append(c);
                crc+=c;
            }

            if (serial.bytesAvailable()<20) run=false;
        }
        else
        {
            run=false;
        }
    }while(run);

}

void Compteur::addHomeAssistant(QString name, QString type, QString unit)
{
    QString payload;
    payload.append("{");
    if (type!="")   payload.append("\"device_class\": \""+type+"\",");
    payload.append("\"state_topic\": \"homeassistant/sensor/teleinfo"+name+"/state\",");
    payload.append("\"name\": \"teleinfo"+name+"\",");
    payload.append("\"unique_id\": \"teleinfo"+name+"\",");
    if (unit!="") payload.append("\"unit_of_measurement\": \""+unit+"\"}");
    emit mqttSend("homeassistant/sensor/teleinfo"+name+"/config",
                  payload,
                  1,1);
}


void Compteur::mqttConnected()
{
    isConnected = true;
    retryMqttTimer.stop();
    souscription=m_client.subscribe(QMqttTopicFilter("fluide/elec/pSolaire"),1);
    connect(souscription,&QMqttSubscription::messageReceived,
            this,&Compteur::newPuissanceSolaire);

    addHomeAssistant("Pinst","power","W");
    addHomeAssistant("Pconso","power","W");
    addHomeAssistant("IndexHC","energy","kWh");
    addHomeAssistant("IndexHP","energy","kWh");
    addHomeAssistant("IndexInjection","energy","kWh");

    qDebug()<<"connect to Mosquitto !";
}

void Compteur::mqttDisConnected()
{
    isConnected = false;
    if (souscription!=nullptr)
    {
        disconnect(souscription,&QMqttSubscription::messageReceived,
                   this,&Compteur::newPuissanceSolaire);
        delete souscription;
        souscription=nullptr;
    }
    qDebug()<<"connection au serveur perdue !!!";
    retryMqttTimer.start();
}

void Compteur::mqttReconnect()
{
    qDebug()<<"try to (re)connect !";
    m_client.connectToHost();
}


void Compteur::mqttSend(QString topic, QString message, int qos, bool retain)
{
    if (isConnected)
    {
        m_client.publish(topic, message.toLatin1(),quint8(qos),retain);
    }
}

void Compteur::newPuissanceSolaire(QMqttMessage message)
{
    //qDebug()<<"new message : "<<message.payload();
    bool isInt;
    int val=message.payload().toInt(&isInt);
    if (isInt)
    {
        //qDebug()<<val;
        pSolaire=val;
    }

}

