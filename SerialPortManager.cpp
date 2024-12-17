#include "SerialPortManager.h"

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent), m_serialPort(new QSerialPort(this))
{
    // 连接接收数据的信号
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortManager::onDataReceived, Qt::DirectConnection);
}

SerialPortManager::~SerialPortManager()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

bool SerialPortManager::init(const QString &portName,
                             int baudRate,
                             QSerialPort::DataBits dataBits,
                             QSerialPort::Parity parity,
                             QSerialPort::StopBits stopBits,
                             QSerialPort::FlowControl flowControl)
{
    // 检查串口是否存在
    if (!isPortAvailable(portName)) {
        qDebug() << "串口" << portName << "不存在！";
        return false;
    }

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(dataBits);
    m_serialPort->setParity(parity);
    m_serialPort->setStopBits(stopBits);
    m_serialPort->setFlowControl(flowControl);

    // 尝试打开串口
    if (m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "串口连接成功：" << portName;
        return true;
    } else {
        qDebug() << "串口连接失败：" << portName;
        return false;
    }
}

void SerialPortManager::closePort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        qDebug() << "串口关闭";
    }
}

bool SerialPortManager::sendData(const QByteArray &data)
{
    if (m_serialPort->isOpen()) {
        qint64 bytesWritten = m_serialPort->write(data);
        return bytesWritten == data.size();
    } else {
        qDebug() << "串口未打开，无法发送数据";
        return false;
    }
}

QByteArray SerialPortManager::readData()
{
    QByteArray data = m_serialPort->readAll();
    return data;
}

bool SerialPortManager::isPortAvailable(const QString &portName)
{
    // 获取系统中的所有串口信息
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &port : availablePorts) {
        if (port.portName() == portName) {
            return true;  // 找到指定的串口
        }
    }

    return false;  // 没有找到指定的串口
}


// 模板函数的实现
template <typename T>
void SerialPortManager::m_addEnumForList(QList<QString>& list)
{
    // 将传入的枚举值转换为整数并添加到 QList 中
    QMetaEnum metaEnum = QMetaEnum::fromType<T>();
    int count = metaEnum.keyCount();
    for (int i = 0; i < count; ++i) {
        QString key = metaEnum.key(i);
        list.append(key);
        //int value = metaEnum.value(i);
        // 处理key和value
    }
}

// 获取串口
QList<QSerialPortInfo> SerialPortManager::getAllPort()
{
    return QSerialPortInfo::availablePorts();
}


// 获取波特率
QList<QString> SerialPortManager::getAllBaudRates()
{
    QList<QString> baudRates;

    // m_addEnumForList<QSerialPort::BaudRate>(baudRates);

    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::BaudRate>();
    int count = metaEnum.keyCount();
    for (int i = 0; i < count; ++i) {
        //QString key = metaEnum.key(i);
        int value = metaEnum.value(i);
        baudRates.append(QString::number(value));
        // 处理key和value
    }

    return baudRates;
}

// 获取数据位
QList<QString> SerialPortManager::getAllDataBits()
{
    QList<QString> dataBitsList;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();
    int count = metaEnum.keyCount();
    for (int i = 0; i < count; ++i) {
        //QString key = metaEnum.key(i);
        int value = metaEnum.value(i);
        dataBitsList.append(QString::number(value));
        // 处理key和value
    }
    return dataBitsList;
}

// 获取停止位
QList<QString> SerialPortManager::getAllStopBits()
{
    QList<QString> stopBitsList;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
    int count = metaEnum.keyCount();
    for (int i = 0; i < count; ++i) {
        QString key = metaEnum.key(i);
        stopBitsList.append(key);
        //int value = metaEnum.value(i);
        // 处理key和value
    }
    return stopBitsList;
}


// 获取校验位
QList<QString> SerialPortManager::getAllParity()
{
    QList<QString> parityList;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();
    int count = metaEnum.keyCount();
    for (int i = 0; i < count; ++i) {
        QString key = metaEnum.key(i);
        parityList.append(key);
        //int value = metaEnum.value(i);
        // 处理key和value
    }
    return parityList;
}



void SerialPortManager::setDataReceivedCallback(std::function<void(const QByteArray &)> callback)
{
    m_dataReceivedCallback = callback;
}

void SerialPortManager::onDataReceived()
{
    // 从串口读取数据
    QByteArray data = m_serialPort->readAll();

    // 如果回调函数已设置，调用回调函数
    if (m_dataReceivedCallback) {
        m_dataReceivedCallback(data);
    }
}
