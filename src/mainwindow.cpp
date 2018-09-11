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
    connect (m_ebmbus, SIGNAL(signal_responseRaw(quint64,quint8,quint8,quint8,QByteArray)), this, SLOT(slot_gotResponseRaw(quint64,quint8,quint8,quint8,QByteArray)));
    connect (m_ebmbus, SIGNAL(signal_actualSpeed(quint64,quint8,quint8,quint8)), this, SLOT(slot_actualSpeed(quint64,quint8,quint8,quint8)));
    connect (m_ebmbus, SIGNAL(signal_EEPROMdata(quint64,quint8,quint8,EbmBusEEPROM::EEPROMaddress,quint8)), this, SLOT(slot_EEPROMdata(quint64,quint8,quint8,EbmBusEEPROM::EEPROMaddress,quint8)));
    connect (m_ebmbus, SIGNAL(signal_EEPROMhasBeenWritten(quint64,quint8,quint8)), this, SLOT(slot_EEPROMhasBeenWritten(quint64,quint8,quint8)));
    connect (m_ebmbus, SIGNAL(signal_setPointHasBeenSet(quint64,quint8,quint8)), this, SLOT(slot_setPointHasBeenSet(quint64,quint8,quint8)));
    connect (m_ebmbus, SIGNAL(signal_simpleStatus(quint64,quint8,quint8,QString)), this, SLOT(slot_simpleStatus(quint64,quint8,quint8,QString)));
    connect (m_ebmbus, SIGNAL(signal_status(quint64,quint8,quint8,quint8,QString,quint8)), this, SLOT(slot_status(quint64,quint8,quint8,quint8,QString,quint8)));
    connect (m_ebmbus, SIGNAL(signal_transactionFinished()), this, SLOT(slot_transactionFinished()));
    connect (m_ebmbus, SIGNAL(signal_transactionLost(quint64)), this, SLOT(slot_transactionLost(quint64)));
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

void MainWindow::slot_gotResponseRaw(quint64 telegramID, quint8 preamble, quint8 commandAndFanaddress, quint8 fanGroup, QByteArray data)
{
    QString r;

    r.sprintf("ID: %llu PRE: %02X  commandAndFanaddress: %02X  fanGroup: %02X  data: ", telegramID, preamble, commandAndFanaddress, fanGroup);
    foreach (quint8 byte, data)
    {
        r.append(QString().sprintf("%02X ", byte));
    }

    ui->plainTextEdit_response->appendPlainText(r);
}

void MainWindow::slot_transactionFinished()
{
    QString r;
    r = "Transaction finished";
    ui->plainTextEdit_response->appendPlainText(r);
}

void MainWindow::slot_transactionLost(quint64 id)
{
    QString r;
    r = QString().sprintf("Transaction id %llu lost", id);
    ui->plainTextEdit_response->appendPlainText(r);

}

void MainWindow::slot_simpleStatus(quint64 telegramID, quint8 fanAddress, quint8 fanGroup, QString status)
{
    QString r;
    r = QString().sprintf("Transaction id %llu: Received simple status from fanAddress %i, fanGroup %i: ", telegramID, fanAddress, fanGroup);
    r += status;
    ui->plainTextEdit_response->appendPlainText(r);
}

void MainWindow::slot_status(quint64 telegramID, quint8 fanAddress, quint8 fanGroup, quint8 statusAddress, QString status, quint8 rawValue)
{
    QString r;
    r = QString().sprintf("Transaction id %llu: Received status[%i]=%i from fanAddress %i, fanGroup %i: ", telegramID, statusAddress, rawValue, fanAddress, fanGroup);
    r += status;
    ui->plainTextEdit_response->appendPlainText(r);
}

void MainWindow::slot_actualSpeed(quint64 telegramID, quint8 fanAddress, quint8 fanGroup, quint8 actualRawSpeed)
{
    QString r;
    r = QString().sprintf("Transaction id %llu: Received actual speed from fanAddress %i, fanGroup %i: %i", telegramID, fanAddress, fanGroup, actualRawSpeed);
    ui->plainTextEdit_response->appendPlainText(r);
}

void MainWindow::slot_setPointHasBeenSet(quint64 telegramID, quint8 fanAddress, quint8 fanGroup)
{
    QString r;
    r = QString().sprintf("Transaction id %llu: Setpoint has been set on fanAddress %i, fanGroup %i", telegramID, fanAddress, fanGroup);
    ui->plainTextEdit_response->appendPlainText(r);
}

void MainWindow::slot_EEPROMhasBeenWritten(quint64 telegramID, quint8 fanAddress, quint8 fanGroup)
{
    QString r;
    r = QString().sprintf("Transaction id %llu: EEPROM has been written on fanAddress %i, fanGroup %i", telegramID, fanAddress, fanGroup);
    ui->plainTextEdit_response->appendPlainText(r);
}

void MainWindow::slot_EEPROMdata(quint64 telegramID, quint8 fanAddress, quint8 fanGroup, EbmBusEEPROM::EEPROMaddress eepromAddress, quint8 dataByte)
{
    QString r;
    r = QString().sprintf("Transaction id %llu: EEPROM data read at %i on fanAddress %i, fanGroup %i: %i", telegramID, (int)eepromAddress, fanAddress, fanGroup, dataByte);
    ui->plainTextEdit_response->appendPlainText(r);
}

void MainWindow::on_pushButton_getU_clicked()
{
    m_ebmbus->getStatus(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), EbmBusStatus::DCvoltage);
}

void MainWindow::on_pushButton_getI_clicked()
{
    m_ebmbus->getStatus(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), EbmBusStatus::DCcurrent);
}

void MainWindow::on_pushButton_setSpeed_clicked()
{
    quint8 speed = ui->spinBox_speed->value();

    m_ebmbus->setSpeedSetpoint(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), speed);
}

void MainWindow::on_pushButton_status_clicked()
{
    m_ebmbus->getSimpleStatus(ui->spinBox_fadr->value(), ui->spinBox_gadr->value());
}

void MainWindow::on_pushButton_EEPROMread_clicked()
{
    QString adrText = ui->comboBox_EEPROMadr->currentText().split(" ").first();
    bool ok;
    EbmBusEEPROM::EEPROMaddress eepromRegister = (EbmBusEEPROM::EEPROMaddress) adrText.toInt(&ok, 16);

    m_ebmbus->readEEPROM(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), eepromRegister);
}

void MainWindow::on_pushButton_statusRead_clicked()
{
    QString adrText = ui->comboBox_StatusAdr->currentText().split(" ").first();
    bool ok;
    EbmBusStatus::StatusAddress statusRegister = (EbmBusStatus::StatusAddress) adrText.toInt(&ok, 16);

    m_ebmbus->getStatus(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), statusRegister);
}

void MainWindow::on_pushButton_EEPROMwrite_clicked()
{
    QString adrText = ui->comboBox_EEPROMadr->currentText().split(" ").first();
    bool ok;
    EbmBusEEPROM::EEPROMaddress eepromRegister = (EbmBusEEPROM::EEPROMaddress) adrText.toInt(&ok, 16);

    m_ebmbus->writeEEPROM(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), eepromRegister, ui->spinBox_EEPROMvalue->value());
}

void MainWindow::on_pushButton_getActualSpeed_clicked()
{
    m_ebmbus->getActualSpeed(ui->spinBox_fadr->value(), ui->spinBox_gadr->value());
}

void MainWindow::on_pushButton_softwareReset_clicked()
{
    m_ebmbus->softwareReset(ui->spinBox_fadr->value(), ui->spinBox_gadr->value());
}
