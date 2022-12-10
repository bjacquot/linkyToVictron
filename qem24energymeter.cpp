#include "qem24energymeter.h"
#include <QDebug>


QEM24EnergyMeter::QEM24EnergyMeter(QObject *parent)
    : QObject{parent}
{
//    reg.insert(QModbusDataUnit::Coils, { QModbusDataUnit::Coils, 0, 10 });
//    reg.insert(QModbusDataUnit::DiscreteInputs, { QModbusDataUnit::DiscreteInputs, 0, 10 });
    reg.insert(QModbusDataUnit::InputRegisters, { QModbusDataUnit::InputRegisters, 0, 0xA101 });
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0, 0xA101 });


    modbusDevice.setMap(reg);
    // 000Bh Carlo Gavazzi identification code 1648

    setRegistre(0xB,1648);
    setRegistre( 0x0302, 0x101E ); // Version and revision code of measurement module
    setRegistre( 0x0304, 0x101E ); // Version and revision code of communication module
    setRegistre( 0x1002, 3 ); // Measuring system  (3="1Ph", 4=“3P”)
    //setRegistre( 0x1003, 10 ); // current transformer ratio)
    //setRegistre( 0x1005, 10 ); // tension transformer ratio
    //setRegistre( 0x1010, 15 ); // interval Time

    //setRegistre( 0x1109, 1 ); // filter coeff

    setRegistre(0x2100,192);
    setRegistre(0x2101,168);
    setRegistre(0x2102,0);
    setRegistre(0x2103,7);

    setRegistre(0x2104,255);
    setRegistre(0x2105,255);
    setRegistre(0x2106,255);
    setRegistre(0x2107,0);

    setRegistre(0x2108,192);
    setRegistre(0x2109,168);
    setRegistre(0x210A,0);
    setRegistre(0x210B,1);

    setRegistre(0x210C,502);

    setRegistre(0x2120,192);
    setRegistre(0x2121,168);
    setRegistre(0x2122,0);
    setRegistre(0x2123,7);

    setRegistre(0x2124,255);
    setRegistre(0x2125,255);
    setRegistre(0x2126,255);
    setRegistre(0x2127,0);

    setRegistre(0x2128,192);
    setRegistre(0x2129,168);
    setRegistre(0x212A,0);
    setRegistre(0x212B,1);


    setRegistre( 0x0032, 0 ); // Phase sequence
    setRegistre( 0x0033, 500 ); // Frequence
    setRegistre( 0xA100, 3 ); //Front selector status lock
    setRegistre( 0x5000, 0x4c69 ); //Serial Digit  1 & 2
    setRegistre( 0x5001, 0x6e6b ); //Serial Digit  3 & 4
    setRegistre( 0x5002, 0x7930 ); //Serial Digit  5 & 6
    setRegistre( 0x5003, 0x3030 ); //Serial Digit  7 & 8
    setRegistre( 0x5004, 0x3030 ); //Serial Digit  9 & 10
    setRegistre( 0x5005, 0x3030 ); //Serial Digit  11 & 12
    setRegistre( 0x5006, 0x3100 ); //Serial Digit  13
    setRegistre( 0xA000, 7 ); //Application : Doit être égal à 7 pour Victron
    //Infos triphasées non utilisées:
    setRegistre( 0x0000, 2300 ); // V-L1
    setRegistre( 0x0001, 0 );
    setRegistre( 0x0002, 0 ); // V-L3
    setRegistre( 0x0003, 0 );
    setRegistre( 0x0004, 0 ); // V-L2
    setRegistre( 0x0005, 0 );
    setRegistre( 0x000C, 100 ); // A-L1
    setRegistre( 0x000D, 0 );
    setRegistre( 0x000E, 0 ); // A-L1
    setRegistre( 0x000F, 0 );
    setRegistre( 0x0010, 0 ); // A-L2
    setRegistre( 0x0011, 0 );
    setRegistre( 0x0012, 1000 ); // W-L1
    setRegistre( 0x0013, 0 );
    setRegistre( 0x0016, 0 ); // W-L2
    setRegistre( 0x0017, 0 );
    setRegistre( 0x0018, 1000 ); // VA-L1
    setRegistre( 0x0019, 0 );
    setRegistre( 0x0042, 0 ); // kWh-L2
    setRegistre( 0x0043, 0 );
    setRegistre( 0x0028, 1000 ); // W-Total
    setRegistre( 0x0029, 0 );
    setRegistre( 0x002A, 1000 ); // VA-Total
    setRegistre( 0x002B, 0 );
    setRegistre( 0x0034, 100); // kWh +
    setRegistre( 0x0035, 0 );
    //Initialisation des valeurs pertinentes pour le monophasé
    setRegistre( 0x0040, 1 ); // kWh-L1
    setRegistre( 0x0041, 0 );
    setRegistre( 0x0044, 0 ); // kWh-L2
    setRegistre( 0x0045, 0 );
    setRegistre( 0x004E, 0 ); // kWh-
    setRegistre( 0x004F, 0 );
    setRegistre( 0x0046, 0 ); // kWh+ T1
    setRegistre( 0x0047, 0 );
    setRegistre( 0x0048, 0 ); // kWh+ T2
    setRegistre( 0x0049, 0 );



    modbusDevice.setConnectionParameter(QModbusDevice::NetworkPortParameter, 502);
    modbusDevice.setConnectionParameter(QModbusDevice::NetworkAddressParameter, "192.168.0.7");
    modbusDevice.setServerAddress(1);
    modbusDevice.setValue(QModbusServer::ListenOnlyMode, 0);
    //modbusDevice.setValue(QModbusServer::DeviceBusy, 0xfff);


    connect(&modbusDevice, &QModbusTcpServer::dataWritten,
            this, &QEM24EnergyMeter::updateWidgets);
    connect(&modbusDevice, &QModbusTcpServer::stateChanged,
            this, &QEM24EnergyMeter::onStateChanged);
    connect(&modbusDevice, &QModbusTcpServer::errorOccurred,
            this, &QEM24EnergyMeter::handleDeviceError);

    connect(&modbusDevice, &QModbusTcpServer::modbusClientDisconnected,
            this, &QEM24EnergyMeter::clientDisconnected);

    modbusDevice.setData(QModbusDataUnit::Coils, 0, 1);
    modbusDevice.setData(QModbusDataUnit::Coils, 1, 2);

    modbusDevice.connectDevice();

}

void QEM24EnergyMeter::setRegistre(quint16 address, quint16 value)
{
    modbusDevice.setData(QModbusDataUnit::HoldingRegisters,address,value);
    modbusDevice.setData(QModbusDataUnit::InputRegisters,address,value);
}

void QEM24EnergyMeter::onStateChanged(int state)
{
    qDebug()<<"state :"<<state;
}

void QEM24EnergyMeter::updateWidgets(QModbusDataUnit::RegisterType table, int address, int size)
{
    quint16 value;
    modbusDevice.data(QModbusDataUnit::HoldingRegisters,address,&value);
    qDebug()<<"updateWidgets :"<<address<<" "<<size<<" "<<value;

}

void QEM24EnergyMeter::handleDeviceError(QModbusDevice::Error newError)
{
    qDebug()<<"error :"<<newError;

}

void QEM24EnergyMeter::clientDisconnected(QTcpSocket *modbusClient)
{
    qDebug()<<"deconnexion : "<<modbusClient->peerAddress() ;
}
