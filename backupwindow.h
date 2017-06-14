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
#include <QQueue>
#include <QDirIterator>
#include <QMessageBox>
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

struct MyFiles{
    qint64 size_;
    QString name_;
    QString path_;
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

    void multicastPassiveTL(QByteArray bytearray);

    void multicastActive(QByteArray bytearray);

    void multicastActiveTL(QByteArray bytearray);

    void on_comboUsers_activated(int index);

    int getPassives();

    int getActives();

    void eraseAllSockets();

    void morePeople(int act, int pas);

    void imAlive(QTcpSocket* sck);

    void eraseFromBlackList(QTcpSocket* sck);

    void executeBlackList();

    void on_sendButton_clicked();

    void backupStarting(BackupMsg c, QTcpSocket* sck);

    void scanDirectory(QDir dir);

    void mountDirAndFiles(BackupMsg bm);

    void checkedPacks(BackupMsg bm);

    void includePassives();

    void busyChannel();

    void channelBusy();

    bool contained(QVector<MagicNode> tl, MagicNode mn);

    void removeThatItem(QVector<MagicNode>& tl, MagicNode mn);

    void removeAll(QVector<MagicNode>& tl);

    void infoAboutTotalSize(qint64 tb);

    void understandSize(BackupMsg bm);

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
    QQueue<MyFiles> FileQueue_;
    QQueue<QByteArray> PackagesQueue_;
    QVector<MagicNode> TransferList_;
    int checkPack_;
    qint64 totalBytes_;
    bool jump_;

};

#endif // BACKUPWINDOW_H
