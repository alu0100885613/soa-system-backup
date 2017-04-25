#include "backupwindow.h"
#include "ui_backupwindow.h"

BackupWindow::BackupWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BackupWindow),
    status_(0),
    timer_(new QTimer),
    timeConnected_(0),
    IamID_(0),
    tcpServer_(new QTcpServer(this)),
    tcpSocket_(new QTcpSocket(this)),
    ClientList_(new QList<QByteArray>)
{
    ui->setupUi(this);

    connect(timer_, SIGNAL(timeout()), this, SLOT(showTime()));
    connect(tcpServer_,SIGNAL(newConnection()),this,SLOT(welcome()));
    connect(tcpSocket_,SIGNAL(readyRead()),this,SLOT(readyRec()));


    initializeElementsGUI();

}

BackupWindow::~BackupWindow()
{
    delete ui;
    delete timer_;
    delete tcpServer_;
    delete tcpSocket_;
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
    ui->directoryLine->setText("");
    ui->ipLine->setText("");
    ui->portLine->setText("");
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    for(int nIter=0; nIter<list.count(); nIter++)
    {
        if(!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ){
                ui->myIpLabel->setText(list[nIter].toString());
                nIter = list.count();
            }

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

}

void BackupWindow::on_modeComboBox_activated(int index)
{
    IamID_ = index;

    if(index == 0)
        initializeElementsGUI();

    if(index == 1){
        ui->browseButton->setEnabled(true);
        ui->connectButton->setEnabled(true);
        ui->sendButton->setEnabled(true);
        ui->directoryLine->setEnabled(false);
        ui->ipLine->setEnabled(true);
        ui->portLine->setEnabled(true);
        ui->directoryLine->setText("");
        ui->ipLine->setText("");
        ui->portLine->setText("");
    }
    if(index == 2){
        ui->connectButton->setEnabled(true);
        ui->browseButton->setEnabled(true);
        ui->sendButton->setEnabled(false);
        ui->directoryLine->setEnabled(false);
        ui->ipLine->setEnabled(true);
        ui->portLine->setEnabled(true);
        ui->directoryLine->setText("");
        ui->ipLine->setText("");
        ui->portLine->setText("");
    }
    if(index == 3){
        ui->connectButton->setEnabled(true);
        ui->browseButton->setEnabled(false);
        ui->sendButton->setEnabled(false);
        ui->directoryLine->setEnabled(false);
        ui->ipLine->setEnabled(false);
        ui->portLine->setEnabled(true);
        ui->directoryLine->setText("");
        ui->ipLine->setText("");
        ui->portLine->setText("");
    }

}

int BackupWindow::whatAmI()
{
    return IamID_;
}

void BackupWindow::on_connectButton_clicked()
{
    const QString disconnectString = "Disconnect";
    const QString connectString = "Connect";

    if(!status_) {

        tryToConnect();

        timer_->start(1000);
        ui->modeComboBox->setEnabled(false);
        ui->myPortLabel->setText(ui->portLine->text());
        ui->connectButton->setText(disconnectString);
        changeStatus();
    } else {

        letsDisconnect();

        timer_->stop();
        timeConnected_ = 0;
        ui->modeComboBox->setEnabled(true);
        ui->browseButton->setEnabled(true);
        ui->portLine->setEnabled(true);
        ui->connectButton->setText(connectString);
        changeStatus();
    }
}

void BackupWindow::on_browseButton_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(
        this,
        tr("Select a Directory"),
        QDir::currentPath());

    ui->directoryLine->setText(dirname);

}

void BackupWindow::tryToConnect()
{
    switch(whatAmI())
    {
        case 1: connectToServer();break;
        case 2: connectToServer();break;
        case 3: connectToWorld();break;
    }
}

void BackupWindow::letsDisconnect()
{
    if(whatAmI() == 3){
        tcpServer_->close();
        qDebug() << "Voy a enviar un mensaje a todos mis clientes actualmente conectados para que terminen, pero solo soy un qDebug :|";
    } else {
        tcpSocket_->abort();
        qDebug() << "He abortado";
    }
}



void BackupWindow::connectToServer()
{

    const QString port = ui->portLine->text();
    const QString ip = ui->ipLine->text();
    ui->portLine->setEnabled(false);
    ui->ipLine->setEnabled(false);
    int portNumber = port.toInt();

    tcpSocket_->connectToHost(ip,portNumber);


}

void BackupWindow::connectToWorld()
{

    const QString port = ui->portLine->text();
    ui->portLine->setEnabled(false);
    int portNumber = port.toInt();

    tcpServer_->listen(QHostAddress::Any,portNumber);

}

void BackupWindow::welcome()
{
    BackupMsg helloMsg;

    helloMsg.set_type_(0);

    std::string output;
    helloMsg.SerializeToString(&output);
    QByteArray byteArray(helloMsg.SerializeAsString().c_str(),helloMsg.ByteSize());
    qDebug() << byteArray;

    while(tcpServer_->hasPendingConnections())
    {
        QTcpSocket* ccSocket = new QTcpSocket(this);
        ccSocket = tcpServer_->nextPendingConnection();
        ccSocket->write(byteArray);
        connect(ccSocket,&QTcpSocket::readyRead,this,[=]{
            QByteArray dataIn = ccSocket->readAll();
            qDebug() << dataIn;
            addClient();
        });
    }


}

void BackupWindow::readyRec()
{
    QByteArray dataIn = tcpSocket_->readAll();
    BackupMsg a;
    a.ParseFromArray(dataIn.data(),dataIn.size());
    qDebug() << a.type_();

    QByteArray dataOut  = "001|10.0.0.0|20.0.0.0|20|323294924dfdfdf|";
    tcpSocket_->write(dataOut);
}

void BackupWindow::addClient(){


}
