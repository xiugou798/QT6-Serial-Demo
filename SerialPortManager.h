#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QDebug>
#include <functional>

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

    // 设置接收数据的回调函数
    void setDataReceivedCallback(std::function<void(const QByteArray &)> callback);

private slots:
    void onDataReceived();  // 串口数据接收槽函数

private:
    QSerialPort *m_serialPort;
    std::function<void(const QByteArray &)> m_dataReceivedCallback; // 回调函数
};

#endif // SERIALPORTMANAGER_H
