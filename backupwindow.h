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
#include <QProcess>
#include "protocolbuffer.pb.h"
#include "backupserver.h"
#include "backupuser.h"

namespace Ui {
class BackupWindow;
}

struct MagicNode{
    QTcpSocket* pointer_;
    int idMode_;
};

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

    void addClient(std::string c, int r,QTcpSocket* sck);

    void analyzePack(BackupMsg pack,QTcpSocket* sck);

    void returnMyIp();

    bool know_host(QString host);

    void keepAlive();

    void wannaDisconnect(QTcpSocket* sck);

    void multicast(QByteArray bytearray);

    void multicastPassive(QByteArray bytearray);

    void on_comboUsers_activated(int index);

    int getPassives();

    int getActives();

    void eraseAllSockets();

    void morePeople(int act, int pas);

    void imAlive(QTcpSocket* sck);

    void eraseFromBlackList(QTcpSocket* sck);

    void executeBlackList();

    void on_sendButton_clicked();

    void backupStarting(BackupMsg c);

private:
    Ui::BackupWindow *ui;
    int status_;
    QTimer* timer_;
    QTimer* timer2_;
    QTimer* ack_;
    long timeConnected_;
    MagicObject* MyMagicObject_;
    QStringList PassiveClientList_;
    QStringList ActiveClientList_;
    QStringList ClientList_;
    QVector<MagicNode> MagicList_;
    QVector<MagicNode> BlackList_;


};

#endif // BACKUPWINDOW_H
