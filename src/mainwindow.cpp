#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ebmbus = new EbmBus(this, "/dev/ttyUSB0");
    connect (m_ebmbus, SIGNAL(signal_response(quint8,quint8,quint8,QByteArray)), this, SLOT(slot_gotResponse(quint8,quint8,quint8,QByteArray)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_openPort_clicked()
{
    if (m_ebmbus->open())
        QMessageBox::information(this, "ebmBus", "Open!");
    else
        QMessageBox::warning(this, "ebmBus", "Open failed!");
}

void MainWindow::slot_gotResponse(quint8 preamble, quint8 commandAndFanaddress, quint8 fanGroup, QByteArray data)
{
    QString r;

    r.sprintf("PRE: %02X  commandAndFanaddress: %02X  fanGroup: %02X  data: ", preamble, commandAndFanaddress, fanGroup);
    foreach (quint8 byte, data)
    {
        r.append(QString().sprintf("%02X ", byte));
    }

    ui->plainTextEdit_response->appendPlainText(r);
}

void MainWindow::on_pushButton_getU_clicked()
{
    m_ebmbus->getStatus(1, 1, 0x03);
    //m_ebmbus->writeTelegram(EbmBus::GetStatus, 1, 1, QByteArray(1, 0x03));
}

void MainWindow::on_pushButton_getI_clicked()
{
    m_ebmbus->getStatus(1, 1, 0x04);
    //m_ebmbus->writeTelegram(EbmBus::GetStatus, 1, 1, QByteArray(1, 0x04));
}

void MainWindow::on_pushButton_setSpeed_clicked()
{
    quint8 speed = ui->spinBox_speed->value();

    m_ebmbus->setSpeedSetpoint(1, 1, speed);
    //m_ebmbus->writeTelegram(EbmBus::SetSetpoint, 1, 1, QByteArray(1, speed));
}

void MainWindow::on_pushButton_status_clicked()
{
    m_ebmbus->getSimpleStatus(1, 1);
    //m_ebmbus->writeTelegram(EbmBus::GetStatus, 1, 1, QByteArray());
}

void MainWindow::on_pushButton_EEPROMread_clicked()
{
    QString adrText = ui->comboBox_EEPROMadr->currentText().split(" ").first();
    quint8 eepromRegister = adrText.toInt(nullptr, 16);

    m_ebmbus->readEEPROM(1, 1, eepromRegister);
    //m_ebmbus->writeTelegram(EbmBus::EEPROMread, 1, 1, QByteArray(1, EEPROMregister));
}

void MainWindow::on_pushButton_statusRead_clicked()
{
    QString adrText = ui->comboBox_StatusAdr->currentText().split(" ").first();
    quint8 statusRegister = adrText.toInt(nullptr, 16);

    m_ebmbus->getStatus(1, 1, statusRegister);
    //m_ebmbus->writeTelegram(EbmBus::GetStatus, 1, 1, QByteArray(1, StatusRegister));
}

void MainWindow::on_pushButton_EEPROMwrite_clicked()
{
    QString adrText = ui->comboBox_EEPROMadr->currentText().split(" ").first();
    quint8 eepromRegister = adrText.toInt(nullptr, 16);

    m_ebmbus->writeEEPROM(1, 1, eepromRegister, ui->spinBox_EEPROMvalue->value());
}
