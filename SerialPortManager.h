#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QDebug>
#include <QMetaEnum>
#include <functional>
#include "EnumToString.h"
#include "StringToEnum.h"

class SerialPortManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    // 初始化串口连接，检查串口是否存在并打开串口
    bool init(const QString &portName,
              int baudRate = QSerialPort::Baud9600,
              QSerialPort::DataBits dataBits = QSerialPort::Data8,
              QSerialPort::Parity parity = QSerialPort::NoParity,
              QSerialPort::StopBits stopBits = QSerialPort::OneStop,
              QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);

    // 关闭串口
    void closePort();

    // 发送数据
    bool sendData(const QByteArray &data);

    // 接收数据
    QByteArray readData();

    // 检查串口是否存在
    bool isPortAvailable(const QString &portName);

    // 获取串口列表
    QList<QSerialPortInfo> getAllPort();

    // 获取波特率
    QList<QString> getAllBaudRates();

    // 获取数据位
    QList<QString> getAllDataBits();

    // 获取停止位
    QList<QString> getAllStopBits();

    // 获取校验位
    QList<QString> getAllParity();

    // 设置接收数据的回调函数
    void setDataReceivedCallback(std::function<void(const QByteArray &)> callback);

private slots:
    void onDataReceived();  // 串口数据接收槽函数

private:
    QSerialPort *m_serialPort;
    std::function<void(const QByteArray &)> m_dataReceivedCallback; // 回调函数

    template <typename T>
    void m_addEnumForList(QList<QString>& list);

    EnumParser<QSerialPort::DataBits> dataBitsParser = {
        {"5", QSerialPort::DataBits::Data5},
        {"6", QSerialPort::DataBits::Data6},
        {"7", QSerialPort::DataBits::Data7},
        {"8", QSerialPort::DataBits::Data8},
        };
    EnumParser<QSerialPort::Parity> parityParser = {
        {"无校验", QSerialPort::Parity::NoParity},
        {"偶校验", QSerialPort::Parity::EvenParity},
        {"奇校验", QSerialPort::Parity::OddParity},
        {"SpaceParity", QSerialPort::Parity::SpaceParity },
        { "MarkParity", QSerialPort::Parity::MarkParity},
        };
    EnumParser<QSerialPort::BaudRate> baudRateParser = {
        {"1200", QSerialPort::BaudRate::Baud1200},
        {"2400", QSerialPort::BaudRate::Baud2400},
        {"4800", QSerialPort::BaudRate::Baud4800},
        {"9600", QSerialPort::BaudRate::Baud9600},
        {"19200", QSerialPort::BaudRate::Baud19200},
        {"38400", QSerialPort::BaudRate::Baud38400},
        {"57600", QSerialPort::BaudRate::Baud57600},
        {"115200", QSerialPort::BaudRate::Baud115200},
           };
    EnumParser<QSerialPort::StopBits> stopBitsParser = {
        {"1", QSerialPort::StopBits::OneStop},
        {"1.5", QSerialPort::StopBits::OneAndHalfStop},
        {"2", QSerialPort::StopBits::TwoStop},
        };
};


DEFINE_ENUM_TO_STRING(QSerialPort::DataBits,
                      {
                          {QSerialPort::DataBits::Data5, "5"},
                          {QSerialPort::DataBits::Data6, "6"},
                          {QSerialPort::DataBits::Data7, "7"},
                          {QSerialPort::DataBits::Data8, "8"},
                      })

DEFINE_ENUM_TO_STRING(QSerialPort::Parity,
                      {
                          {QSerialPort::Parity::NoParity, "无校验"},
                          {QSerialPort::Parity::EvenParity, "偶校验"},
                          {QSerialPort::Parity::OddParity, "奇校验"},
                          {QSerialPort::Parity::SpaceParity, "SpaceParity"},
                          {QSerialPort::Parity::MarkParity, "MarkParity"},
                      })

DEFINE_ENUM_TO_STRING(QSerialPort::BaudRate,
                      {
                          {QSerialPort::BaudRate::Baud1200, "1200"},
                          {QSerialPort::BaudRate::Baud2400, "2400"},
                          {QSerialPort::BaudRate::Baud4800, "4800"},
                          {QSerialPort::BaudRate::Baud9600, "9600"},
                          {QSerialPort::BaudRate::Baud19200, "19200"},
                          {QSerialPort::BaudRate::Baud38400, "38400"},
                          {QSerialPort::BaudRate::Baud57600, "57600"},
                          {QSerialPort::BaudRate::Baud115200, "115200"},
                       })

DEFINE_ENUM_TO_STRING(QSerialPort::StopBits,
                      {
                          {QSerialPort::StopBits::OneStop, "1"},
                          {QSerialPort::StopBits::OneAndHalfStop, "1.5"},
                          {QSerialPort::StopBits::TwoStop, "2"},
                      })




#endif // SERIALPORTMANAGER_H
