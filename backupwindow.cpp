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

    initializeElementsGUI();

}

BackupWindow::~BackupWindow()
{
    delete ui;
}

void BackupWindow::changeStatus()
{
    status_ = !status_;
}

void BackupWindow::initializeElementsGUI()
{
    ui->browseButton->setEnabled(false);
    ui->connectButton->setEnabled(false);
    ui->sendButton->setEnabled(false);
    ui->directoryLine->setEnabled(false);
    ui->ipLine->setEnabled(false);
    ui->portLine->setEnabled(false);
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    for(int nIter=0; nIter<list.count(); nIter++)
    {
        if(!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
                ui->myIpLabel->setText(list[nIter].toString());

    }
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
    if(index == 0)
        initializeElementsGUI();

    if(index == 1){
        ui->browseButton->setEnabled(true);
        ui->connectButton->setEnabled(true);
        ui->sendButton->setEnabled(true);
        ui->directoryLine->setEnabled(true);
        ui->ipLine->setEnabled(true);
        ui->portLine->setEnabled(true);
    }
    if(index == 2){
        ui->connectButton->setEnabled(true);
        ui->browseButton->setEnabled(true);
        ui->sendButton->setEnabled(false);
        ui->directoryLine->setEnabled(true);
        ui->ipLine->setEnabled(true);
        ui->portLine->setEnabled(true);
    }
    if(index == 3){
        ui->connectButton->setEnabled(true);
        ui->browseButton->setEnabled(false);
        ui->sendButton->setEnabled(false);
        ui->directoryLine->setEnabled(false);
        ui->ipLine->setEnabled(false);
        ui->portLine->setEnabled(true);
    }

}

void BackupWindow::on_connectButton_clicked()
{
    const QString disconnectString = "Disconnect";
    const QString connectString = "Connect";

    if(!status_) {
        //toda la pesca que envía al conectar

        timer_->start(1000);
        ui->modeComboBox->setEnabled(false);
        ui->myPortLabel->setText(ui->portLine->text());
        ui->connectButton->setText(disconnectString);
        changeStatus();
    } else {
        //toda la pesca que envía al desconectar

        timer_->stop();
        timeConnected_ = 0;
        ui->modeComboBox->setEnabled(true);
        ui->connectButton->setText(connectString);
        changeStatus();
    }
}
