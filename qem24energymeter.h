#ifndef QEM24ENERGYMETER_H
#define QEM24ENERGYMETER_H

#include <QObject>
#include <QModbusTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

class QEM24EnergyMeter : public QObject
{
    Q_OBJECT
public:
    explicit QEM24EnergyMeter(QObject *parent = nullptr);

private:
    QModbusTcpServer modbusDevice;

    QModbusDataUnitMap reg;

    void setRegistre(quint16 address, quint16 value);

private Q_SLOTS:
    void onStateChanged(int state);
    void updateWidgets(QModbusDataUnit::RegisterType table, int address, int size);
    void handleDeviceError(QModbusDevice::Error newError);

    void clientDisconnected(QTcpSocket *modbusClient);

signals:

};

#endif // QEM24ENERGYMETER_H
