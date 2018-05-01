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
    m_ebmbus->getStatus(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), 0x03);
}

void MainWindow::on_pushButton_getI_clicked()
{
    m_ebmbus->getStatus(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), 0x04);
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
    quint8 eepromRegister = adrText.toInt(&ok, 16);

    m_ebmbus->readEEPROM(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), eepromRegister);
}

void MainWindow::on_pushButton_statusRead_clicked()
{
    QString adrText = ui->comboBox_StatusAdr->currentText().split(" ").first();
    bool ok;
    quint8 statusRegister = adrText.toInt(&ok, 16);

    m_ebmbus->getStatus(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), statusRegister);
}

void MainWindow::on_pushButton_EEPROMwrite_clicked()
{
    QString adrText = ui->comboBox_EEPROMadr->currentText().split(" ").first();
    bool ok;
    quint8 eepromRegister = adrText.toInt(&ok, 16);

    m_ebmbus->writeEEPROM(ui->spinBox_fadr->value(), ui->spinBox_gadr->value(), eepromRegister, ui->spinBox_EEPROMvalue->value());
}

void MainWindow::on_pushButton_getActualSpeed_clicked()
{
    m_ebmbus->getActualSpeed(ui->spinBox_fadr->value(), ui->spinBox_gadr->value());
}
