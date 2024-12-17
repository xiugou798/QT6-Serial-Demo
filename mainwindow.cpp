#include "mainwindow.h"
#include "ui_mainwindow.h"



SerialPortManager serialManager;

void MainWindow::dataCallback(const QByteArray &data)
{
    // 处理接收到的数据
    QString receivedHex = data.toHex().toUpper(); // 转换为十六进制大写字符串
    QString formattedHex;

    // 添加空格分隔
    for (int i = 0; i < receivedHex.size(); i += 2) {
        formattedHex.append(receivedHex.mid(i, 2)); // 取两位
        if (i + 2 < receivedHex.size()) {
            formattedHex.append(' '); // 每两位后添加空格
        }
    }

    // 将接收到的内容追加到文本框
    QString existingText = ui->textEdit->toPlainText();
    if (!existingText.isEmpty()) {
        existingText.append(' '); // 如果已经有内容，先添加一个空格
    }
    existingText.append(formattedHex); // 追加新接收到的数据

    // 更新文本框内容
    ui->textEdit->setText(existingText);

    // 可选：打印到调试控制台
    qDebug() << "接收到的数据：" << formattedHex;
}


void MainWindow::initSerialPorts()
{
    // 获取所有可用的串口信息
    QList<QSerialPortInfo> serialPortInfoList = serialManager.getAllPort();

    QList baudRatesList =  serialManager.getAllBaudRates();
    QList dataBitsList = serialManager.getAllDataBits();
    QList stopBitsList =  serialManager.getAllStopBits();
    QList parityList =  serialManager.getAllParity();

    // 清空下拉框内容
    ui->comboBox->clear();

    // 将串口名称添加到下拉框
    for (const QSerialPortInfo &info : serialPortInfoList) {
        ui->comboBox->addItem(info.portName());
    }
    // 将波特率添加到下拉框
    for (const QString &info : baudRatesList) {
        ui->comboBox_2->addItem(info);
    }
    // 波特率默认9600
    ui->comboBox_2->setCurrentIndex(3);
    // 将数据位添加到下拉框
    for (const QString &info : dataBitsList) {
        ui->comboBox_3->addItem(info);
    }
    ui->comboBox_3->setCurrentIndex(dataBitsList.length() - 1);

    // 将停止位添加到下拉框
    for (const QString &info : stopBitsList) {
        ui->comboBox_4->addItem(info);
    }

    // 将校验位添加到下拉框
    for (const QString &info : parityList) {
        ui->comboBox_5->addItem(info);
    }

    // 如果没有可用串口，提示用户
    if (serialPortInfoList.isEmpty()) {
        QMessageBox::warning(this, "提示", "未检测到可用串口！");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initSerialPorts();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // 获取输入的十六进制字符串
    QString hexString = ui->textEdit_2->toPlainText().trimmed();

    // 检查输入是否为空
    if (hexString.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入有效的十六进制字符串！");
        return;
    }

    // 去除可能的空格或换行符
    hexString.remove(" ");

    // 检查输入是否是有效的十六进制格式
    bool isValidHex = true;
    for (QChar c : hexString) {
        if (!c.isDigit() && !(c.toLower() >= 'a' && c.toLower() <= 'f')) {
            isValidHex = false;
            break;
        }
    }

    if (!isValidHex) {
        QMessageBox::warning(this, "输入错误", "输入的字符串不是有效的十六进制格式！");
        return;
    }

    // 如果字符串长度为奇数，前面补零
    if (hexString.length() % 2 != 0) {
        hexString.insert(hexString.length() - 1, '0');
    }

    // 将十六进制字符串转换为 QByteArray
    QByteArray tmp = QByteArray::fromHex(hexString.toUtf8());

    // 检查转换后的数据是否有效
    if (tmp.isEmpty()) {
        QMessageBox::warning(this, "转换错误", "无法从十六进制字符串转换为有效的数据！");
        return;
    }

    // 调用 sendData 方法发送数据
    serialManager.sendData(tmp);

    // 可选：提供反馈，例如发送成功提示
    // QMessageBox::information(this, "发送成功", "数据已成功发送！");
}


void MainWindow::on_pushButton_2_clicked(bool checked)
{


    if (checked) {
        // 如果按钮状态为 "关闭"，关闭串口并更新按钮文字
        serialManager.closePort();
        ui->pushButton_2->setText("打开");
        ui->pushButton_2->setChecked(true);
        return;
    }

    // 获取选中的串口名称
    QString portName = ui->comboBox->currentText();
    int baudRates = ui->comboBox_2->currentText().toInt();
    QSerialPort::DataBits dataBits = static_cast<QSerialPort::DataBits>(ui->comboBox_3->currentText().toInt());
    QSerialPort::StopBits stopBits =ui->comboBox_4->currentData().value<QSerialPort::StopBits>();
    QSerialPort::Parity parity = ui->comboBox_5->currentData().value<QSerialPort::Parity>();

    if (portName.isEmpty()) {
        QMessageBox::warning(this, "错误", "请选择一个有效的串口！");
        ui->pushButton_2->setChecked(true);
        return;
    }

    // 检查并尝试打开串口
    if (!serialManager.init(portName, baudRates, dataBits, parity, stopBits)) {
        QMessageBox::critical(this, "错误", "无法打开串口，请检查连接！");
        ui->pushButton_2->setChecked(true);
        return;
    }


    qDebug() << "成功连接到串口：" << portName;

    // 更新按钮文字为 "关闭"
    ui->pushButton_2->setText("关闭");
    ui->pushButton_2->setChecked(false);

    // 设置接收数据的回调函数
    serialManager.setDataReceivedCallback([this](const QByteArray& data) {
        dataCallback(data);
    });
}


// 清空发送区域
void MainWindow::on_pushButton_4_clicked()
{
    ui->textEdit->clear();
}

// 清空接收区
void MainWindow::on_pushButton_3_clicked()
{
    ui->textEdit_2->clear();
}



