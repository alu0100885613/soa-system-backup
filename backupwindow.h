#ifndef BACKUPWINDOW_H
#define BACKUPWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QFileDialog>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class BackupWindow;
}

class BackupWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BackupWindow(QWidget *parent = 0);
    ~BackupWindow();

private slots:
    void changeStatus(void);

    void initializeElementsGUI(void);

    void showTime(void);

    void on_modeComboBox_activated(int index);

    void on_connectButton_clicked();

    void on_browseButton_clicked();

    int whatAmI();

    void tryToConnect();

    void letsDisconnect();

    void connectToServer();

    void connectToWorld();

    void welcome();

    void readyRec();

private:
    Ui::BackupWindow *ui;
    int status_;
    QTimer* timer_;
    long timeConnected_;
    int IamID_;
    QTcpServer* tcpServer_;
    QTcpSocket* tcpSocket_;

};

#endif // BACKUPWINDOW_H
