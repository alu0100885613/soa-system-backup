#include "backupwindow.h"
#include "ui_backupwindow.h"

BackupWindow::BackupWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BackupWindow),
    status_(0),
    timer_(new QTimer),
    timeConnected_(0)
{
    ui->setupUi(this);

    connect(timer_, SIGNAL(timeout()), this, SLOT(showTime()));

}

BackupWindow::~BackupWindow()
{
    delete ui;
}

void BackupWindow::changeStatus()
{
    status_ = !status_;
}

void BackupWindow::showTime()
{
    long timeCopy = timeConnected_++;
    long hours = timeCopy/3600;
    timeCopy -= hours*3600;
    long minutes = timeCopy/60;
    timeCopy -= minutes*60;
    long seconds = timeCopy;

    if(seconds % 2 == 0) {
        QString text = QString::number(hours) + ":" + QString::number(minutes) + ":" + QString::number(seconds);
        ui->lcdNumber->display(text);
    } else {
        QString text = QString::number(hours) + " " + QString::number(minutes) + " " + QString::number(seconds);
        ui->lcdNumber->display(text);
    }
    qDebug() << hours << minutes << seconds;
}

void BackupWindow::on_modeComboBox_activated(int index)
{
    if(index == 1)
        qDebug() <<  "Soy el active user";

    if(index == 2)
        qDebug() << "Soy el passive user";

    if(index == 3)
        qDebug() << "Soy el server";

}

void BackupWindow::on_connectButton_clicked()
{
    const QString disconnectString = "Disconnect";
    const QString connectString = "Connect";

    if(!status_) {
        //toda la pesca que envía al conectar
        timer_->start(1000);
        ui->connectButton->setText(disconnectString);
        changeStatus();
    } else {
        //toda la pesca que envía al desconectar
        timer_->stop();
        timeConnected_ = 0;
        ui->connectButton->setText(connectString);
        changeStatus();
    }
}
