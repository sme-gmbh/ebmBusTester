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

    void slot_gotResponse(quint8 preamble, quint8 commandAndFanaddress, quint8 fanGroup, QByteArray data);

    void on_pushButton_getU_clicked();

    void on_pushButton_getI_clicked();

    void on_pushButton_setSpeed_clicked();

    void on_pushButton_status_clicked();

    void on_pushButton_EEPROMread_clicked();

    void on_pushButton_statusRead_clicked();

    void on_pushButton_EEPROMwrite_clicked();

private:
    Ui::MainWindow *ui;
    EbmBus* m_ebmbus;
};

#endif // MAINWINDOW_H
