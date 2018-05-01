#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <libebmbus/ebmbus.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_openPort_clicked();

    void slot_gotResponseRaw(quint64 telegramID, quint8 preamble, quint8 commandAndFanaddress, quint8 fanGroup, QByteArray data);

    void slot_transactionFinished();
    void slot_transactionLost(quint64 id);
    void slot_simpleStatus(quint64 telegramID, quint8 fanAddress, quint8 fanGroup, QString status);
    void slot_status(quint64 telegramID, quint8 fanAddress, quint8 fanGroup, quint8 statusAddress, QString status, quint8 rawValue);
    void slot_actualSpeed(quint64 telegramID, quint8 fanAddress, quint8 fanGroup, quint8 actualRawSpeed);
    void slot_setPointHasBeenSet(quint64 telegramID, quint8 fanAddress, quint8 fanGroup);
    void slot_EEPROMhasBeenWritten(quint64 telegramID, quint8 fanAddress, quint8 fanGroup);
    void slot_EEPROMdata(quint64 telegramID, quint8 fanAddress, quint8 fanGroup, EbmBusEEPROM::EEPROMaddress eepromAddress, quint8 dataByte);

    void on_pushButton_getU_clicked();

    void on_pushButton_getI_clicked();

    void on_pushButton_setSpeed_clicked();

    void on_pushButton_status_clicked();

    void on_pushButton_EEPROMread_clicked();

    void on_pushButton_statusRead_clicked();

    void on_pushButton_EEPROMwrite_clicked();

    void on_pushButton_getActualSpeed_clicked();

private:
    Ui::MainWindow *ui;
    EbmBus* m_ebmbus;
};

#endif // MAINWINDOW_H
