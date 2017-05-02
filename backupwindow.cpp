#include "backupwindow.h"
#include "ui_backupwindow.h"

BackupWindow::BackupWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BackupWindow),
    status_(0),
    timer_(new QTimer),
    timeConnected_(0),
    MyMagicObject_(),
    PassiveClientList_(),
    ActiveClientList_(),
    ClientList_(),
    MagicList_()
{
    ui->setupUi(this);

    connect(timer_, SIGNAL(timeout()), this, SLOT(showTime()));

    initializeElementsGUI();
    keepAlive();
}

BackupWindow::~BackupWindow()
{
    delete ui;
    delete timer_;
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

    if(index == 0)
        initializeElementsGUI();

    if(index == 1){
        MyMagicObject_ = new BackupUser(index);

        ui->browseButton->setEnabled(true);
        ui->connectButton->setEnabled(true);
        ui->sendButton->setEnabled(true);
        ui->directoryLine->setEnabled(false);
        ui->ipLine->setEnabled(true);
        ui->portLine->setEnabled(true);
        ui->directoryLine->setText("");
        ui->ipLine->setText("");
        ui->portLine->setText("");

        connect(MyMagicObject_->getTheSocket(),SIGNAL(readyRead()),this,SLOT(readyRec()));
    }
    if(index == 2){
        MyMagicObject_ = new BackupUser(index);

        ui->connectButton->setEnabled(true);
        ui->browseButton->setEnabled(true);
        ui->sendButton->setEnabled(false);
        ui->directoryLine->setEnabled(false);
        ui->ipLine->setEnabled(true);
        ui->portLine->setEnabled(true);
        ui->directoryLine->setText("");
        ui->ipLine->setText("");
        ui->portLine->setText("");

        connect(MyMagicObject_->getTheSocket(),SIGNAL(readyRead()),this,SLOT(readyRec()));
    }
    if(index == 3){
        MyMagicObject_ = new BackupServer(index);

        ui->connectButton->setEnabled(true);
        ui->browseButton->setEnabled(false);
        ui->sendButton->setEnabled(false);
        ui->directoryLine->setEnabled(false);
        ui->ipLine->setEnabled(false);
        ui->portLine->setEnabled(true);
        ui->directoryLine->setText("");
        ui->ipLine->setText("");
        ui->portLine->setText("");

        connect(MyMagicObject_->getTheServer(),SIGNAL(newConnection()),this,SLOT(welcome()));
    }

    if(index != 0)
        ui->modeComboBox->setEnabled(false);

}

int BackupWindow::whatAmI()
{
    return MyMagicObject_->getWhatAmI();
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
        BackupMsg byeMsg;
        byeMsg.set_type_(2);
        byeMsg.set_role_(whatAmI());

        QByteArray byteArray(byeMsg.SerializeAsString().c_str(), byeMsg.ByteSize());
        multicast(byteArray);
        eraseAllSockets();
        on_comboUsers_activated(0);
        MyMagicObject_->getTheServer()->close();
    } else {
        BackupMsg byeMsg;
        byeMsg.set_type_(2);
        byeMsg.set_origin_(ui->myIpLabel->text().toStdString());
        byeMsg.set_role_(whatAmI());

        QByteArray byteArray(byeMsg.SerializeAsString().c_str(), byeMsg.ByteSize());
        MyMagicObject_->getTheSocket()->write(byteArray);
        MyMagicObject_->getTheSocket()->close();
    }
}

void BackupWindow::connectToServer()
{

    const QString port = ui->portLine->text();
    const QString ip = ui->ipLine->text();
    ui->portLine->setEnabled(false);
    ui->ipLine->setEnabled(false);
    int portNumber = port.toInt();

    MyMagicObject_->getTheSocket()->connectToHost(ip,portNumber);


}

void BackupWindow::connectToWorld()
{

    const QString port = ui->portLine->text();
    ui->portLine->setEnabled(false);
    int portNumber = port.toInt();

    MyMagicObject_->getTheServer()->listen(QHostAddress::Any,portNumber);

}

void BackupWindow::welcome()
{
    BackupMsg helloMsg;

    helloMsg.set_type_(0);
    helloMsg.set_role_(whatAmI());

    QByteArray byteArray(helloMsg.SerializeAsString().c_str(),helloMsg.ByteSize());

    while(MyMagicObject_->getTheServer()->hasPendingConnections())
    {
        BackupUser* ccVirtualUser = new BackupUser(MyMagicObject_->getTheServer()->nextPendingConnection());
        ccVirtualUser->getTheSocket()->write(byteArray);
        connect(ccVirtualUser->getTheSocket(),&QTcpSocket::readyRead,this,[=]{
            QByteArray dataIn = ccVirtualUser->getTheSocket()->readAll();
            BackupMsg pack;
            pack.ParseFromArray(dataIn.data(),dataIn.size());
            analyzePack(pack,ccVirtualUser->getTheSocket());
        });
    }


}

void BackupWindow::readyRec()
{
    QByteArray dataIn = MyMagicObject_->getTheSocket()->readAll();
    BackupMsg pack;
    pack.ParseFromArray(dataIn.data(),dataIn.size());

    analyzePack(pack,MyMagicObject_->getTheSocket());

}

void BackupWindow::addClient(std::string c, int r, QTcpSocket* sck)
{
    QString toInsert = QString::fromStdString(c);

    MagicNode node;
    node.pointer_ = sck;
    node.idMode_ = r;
    MagicList_.push_back(node);
    ui->nusers->setText(QString::number(MagicList_.size()));


    if(!know_host(toInsert)){
        ClientList_.push_back(toInsert);

        if(r == 2)
            PassiveClientList_.push_back(toInsert);

        if(r == 1)
            ActiveClientList_.push_back(toInsert);
    }

    qDebug() << MagicList_.size();
    qDebug() << "Total: " << ClientList_;
    qDebug() << "Pasivos :" << PassiveClientList_;
    qDebug() << "Activos :" << ActiveClientList_;

}

void BackupWindow::analyzePack(BackupMsg pack, QTcpSocket* sck)
{
    switch(pack.type_())
    {
        case 0: returnMyIp();break;
        case 1: addClient(pack.origin_(),pack.role_(),sck);break;
        case 2: wannaDisconnect(sck);
    }
}

void BackupWindow::returnMyIp()
{
    BackupMsg myPackage;
    QString myIp = ui->myIpLabel->text();

    myPackage.set_type_(1);
    myPackage.set_origin_(myIp.toStdString());
    myPackage.set_role_(whatAmI());

    QByteArray byteArray(myPackage.SerializeAsString().c_str(), myPackage.ByteSize());
    MyMagicObject_->getTheSocket()->write(byteArray);

}

bool BackupWindow::know_host(QString host)
{
    for(auto entry: ClientList_){
        if(entry == host)
            return true;
    }

    return false;
}

void BackupWindow::keepAlive()
{

}

void BackupWindow::wannaDisconnect(QTcpSocket* sck)
{
    if(whatAmI() == 3){
        for(int i = 0; i < MagicList_.size() ; i++){
            if(MagicList_[i].pointer_ == sck){
                MagicList_[i].pointer_->close();
                MagicList_.removeAt(i);
                ui->nusers->setText(QString::number(MagicList_.size()));
            }
        }
    }
    else{
       sck->close();

       const QString connectString = "Connect";

       timer_->stop();
       timeConnected_ = 0;
       ui->browseButton->setEnabled(true);
       ui->portLine->setEnabled(true);
       ui->connectButton->setText(connectString);
       changeStatus();
    }

}

void BackupWindow::multicast(QByteArray bytearray)
{
    for(int i = 0; i < MagicList_.size() ; i++){
        MagicList_[i].pointer_->write(bytearray);
    }
}

void BackupWindow::on_comboUsers_activated(int index)
{
    if(index == 0){
        ui->nusers->setText(QString::number(MagicList_.size()));
    }

    if(index == 1){
        ui->nusers->setText(QString::number(getPassives()));
    }

    if(index == 2){
        ui->nusers->setText(QString::number(getActives()));
    }
}

int BackupWindow::getPassives()
{
    int count = 0;

    for(auto entry: MagicList_){
        if(entry.idMode_ == 2)
            count++;
    }

    return count;
}

int BackupWindow::getActives()
{
    int count = 0;

    for(auto entry: MagicList_){
        if(entry.idMode_ == 1)
            count++;
    }

    return count;
}

void BackupWindow::eraseAllSockets()
{
    for(int i = 0; i < MagicList_.size() ; i++){
        MagicList_.removeAt(i);
    }
}
