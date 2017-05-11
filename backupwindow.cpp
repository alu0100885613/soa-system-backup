#include "backupwindow.h"
#include "ui_backupwindow.h"
#define NUMBER_OF_PASSIVES_REQUIRED 1
#define ACTIVES 1
#define PASSIVES 2
#define SERVER 3

BackupWindow::BackupWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BackupWindow),
    status_(0),
    timer_(new QTimer),
    timer2_(new QTimer),
    ack_(new QTimer),
    timeConnected_(0),
    MyMagicObject_(),
    PassiveClientList_(),
    ActiveClientList_(),
    ClientList_(),
    MagicList_(),
    BlackList_(),
    FileQueue_(),
    PackagesQueue_(),
    TransferList_(),
    checkPack_(0)
{
    ui->setupUi(this);

    connect(timer_, SIGNAL(timeout()), this, SLOT(showTime()));
    connect(timer2_,SIGNAL(timeout()), this, SLOT(keepAlive()));

    connect(ack_,&QTimer::timeout,this,[=]{
        ack_->stop();
        if(!BlackList_.empty()){
            executeBlackList();
        }
    });

    initializeElementsGUI();
}

BackupWindow::~BackupWindow()
{
    delete ui;
    delete timer_;
    delete MyMagicObject_;
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
        ui->connectButton->setEnabled(false);
        ui->sendButton->setEnabled(false);
        ui->directoryLine->setEnabled(false);
        ui->ipLine->setEnabled(true);
        ui->portLine->setEnabled(true);
        ui->directoryLine->setText("");
        ui->ipLine->setText("");
        ui->portLine->setText("");
        ui->comboUsers->setCurrentIndex(1);
        ui->comboUsers->setEnabled(false);

        connect(MyMagicObject_->getTheSocket(),SIGNAL(readyRead()),this,SLOT(readyRec()));
    }
    if(index == 2){
        MyMagicObject_ = new BackupUser(index);

        ui->connectButton->setEnabled(false);
        ui->browseButton->setEnabled(true);
        ui->sendButton->setEnabled(false);
        ui->directoryLine->setEnabled(false);
        ui->ipLine->setEnabled(true);
        ui->portLine->setEnabled(true);
        ui->directoryLine->setText("");
        ui->ipLine->setText("");
        ui->portLine->setText("");
        ui->comboUsers->setCurrentIndex(2);
        ui->comboUsers->setEnabled(false);

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

        if(whatAmI() == 3)
            timer2_->start(5000);

        ui->browseButton->setEnabled(false);
        ui->modeComboBox->setEnabled(false);
        ui->myPortLabel->setText(ui->portLine->text());
        ui->connectButton->setText(disconnectString);
        changeStatus();

    } else {

        letsDisconnect();

        timer_->stop();
        timer2_->stop();
        timeConnected_ = 0;

        if(whatAmI() != SERVER){
            ui->browseButton->setEnabled(true);
        }
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
    if(dirname != "")
        ui->connectButton->setEnabled(true);

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
        MyMagicObject_->getTheSocket()->waitForBytesWritten();
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

    if(TransferList_.size() > 0){
        helloMsg.set_type_(9);
    } else {
        helloMsg.set_type_(0);
    }

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
    BackupMsg infOthers;
    infOthers.set_type_(3);
    infOthers.set_role_(whatAmI());
    infOthers.set_nusersact(getActives());
    infOthers.set_nuserspas(getPassives());

    QByteArray byteArray(infOthers.SerializeAsString().c_str(), infOthers.ByteSize());
    multicast(byteArray);

    if(!know_host(toInsert)){
        ClientList_.push_back(toInsert);

        if(r == 2)
            PassiveClientList_.push_back(toInsert);

        if(r == 1)
            ActiveClientList_.push_back(toInsert);
    }

}

void BackupWindow::analyzePack(BackupMsg pack, QTcpSocket* sck)
{
    switch(pack.type_())
    {
        case 0: returnMyIp();break;
        case 1: addClient(pack.origin_(),pack.role_(),sck);break;
        case 2: wannaDisconnect(sck);break;
        case 3: morePeople(pack.nusersact(),pack.nuserspas());break;
        case 4: imAlive(sck);break;
        case 5: eraseFromBlackList(sck);break;
        case 6: backupStarting(pack,sck);break;
        case 7: mountDirAndFiles(pack);break;
        case 8: checkedPacks(pack);break;
        case 9: channelBusy();break;
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
    BackupMsg kAlive;
    kAlive.set_type_(4);
    kAlive.set_role_(whatAmI());

    QByteArray byteArray(kAlive.SerializeAsString().c_str(), kAlive.ByteSize());
    BlackList_ = MagicList_;

    multicast(byteArray);
    ack_->start(3000);
}

void BackupWindow::wannaDisconnect(QTcpSocket* sck)
{
    if(whatAmI() == 3){
        for(int i = 0; i < MagicList_.size() ; i++){
            if(MagicList_[i].pointer_ == sck){
                MagicList_[i].pointer_->close();
                if(contained(TransferList_,MagicList_[i])){
                    if(MagicList_[i].idMode_ == ACTIVES){
                        qDebug() << "removeall";
                        removeAll(TransferList_);
                        qDebug() << TransferList_.size() << TransferList_.first().pointer_;
                    } else {
                        qDebug() << "removeOne";
                        removeThatItem(TransferList_,MagicList_[i]);
                        BackupMsg checked;
                        checked.set_type_(8);
                        checked.set_role_(whatAmI());
                        checkedPacks(checked);
                    }
                }
                MagicList_.removeAt(i);              
                ui->nusers->setText(QString::number(MagicList_.size()));
                BackupMsg infOthers;
                infOthers.set_type_(3);
                infOthers.set_role_(whatAmI());        
                infOthers.set_nusersact(getActives());
                infOthers.set_nuserspas(getPassives());

                QByteArray byteArray(infOthers.SerializeAsString().c_str(), infOthers.ByteSize());
                multicast(byteArray);
            }
        }
    } else {
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
    if(whatAmI() == 3){
        for(int i = 0; i < MagicList_.size() ; i++){
            MagicList_[i].pointer_->write(bytearray);
        }
    } else {
        MyMagicObject_->getTheSocket()->write(bytearray);
    }
}

void BackupWindow::multicastPassive(QByteArray bytearray)
{
    for(int i = 0; i < MagicList_.size() ; i++){
        if(MagicList_[i].idMode_ == PASSIVES)
            MagicList_[i].pointer_->write(bytearray);
    }
}

void BackupWindow::multicastPassiveTL(QByteArray bytearray)
{
    for(int i = 0; i < TransferList_.size() ; i++){
        if(TransferList_[i].idMode_ == PASSIVES){
            TransferList_[i].pointer_->write(bytearray);
        }
    }
}

void BackupWindow::multicastActive(QByteArray bytearray)
{
    for(int i = 0; i < MagicList_.size() ; i++){
        if(MagicList_[i].idMode_ == ACTIVES)
            MagicList_[i].pointer_->write(bytearray);
    }
}

void BackupWindow::multicastActiveTL(QByteArray bytearray)
{
    for(int i = 0; i < TransferList_.size() ; i++){
        if(TransferList_[i].idMode_ == ACTIVES)
            TransferList_[i].pointer_->write(bytearray);
        return;
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

void BackupWindow::morePeople(int act, int pas)
{
    if(whatAmI() == 1){
        ui->nusers->setText(QString::number(pas));
        if(pas >= NUMBER_OF_PASSIVES_REQUIRED)
            ui->sendButton->setEnabled(true);
        else
            ui->sendButton->setEnabled(false);
    }

    if(whatAmI() == 2)
        ui->nusers->setText(QString::number(act));
}

void BackupWindow::imAlive(QTcpSocket *sck)
{
    BackupMsg ACK;
    ACK.set_type_(5);
    ACK.set_role_(whatAmI());

    QByteArray byteArray(ACK.SerializeAsString().c_str(), ACK.ByteSize());
    sck->write(byteArray);

}

void BackupWindow::eraseFromBlackList(QTcpSocket* sck)
{
    for(int i = 0; i < BlackList_.size() ; i++){
        if(BlackList_[i].pointer_ == sck){
            BlackList_.removeAt(i);
            return;
        }
    }
}

void BackupWindow::executeBlackList()
{
    for(auto entry: BlackList_){
        wannaDisconnect(entry.pointer_);
    }
}

void BackupWindow::on_sendButton_clicked()
{
    BackupMsg sendStart;
    sendStart.set_type_(6);
    sendStart.set_origin_(ui->myIpLabel->text().toStdString());
    sendStart.set_role_(whatAmI());

    QByteArray byteArray(sendStart.SerializeAsString().c_str(), sendStart.ByteSize());
    MyMagicObject_->getTheSocket()->write(byteArray);
    MyMagicObject_->getTheSocket()->waitForBytesWritten();


    ui->browseButton->setEnabled(false);
    QDir sourceDir(ui->directoryLine->text());

    sourceDir.setFilter(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files | QDir::Dirs);
    scanDirectory(sourceDir);
}

void BackupWindow::backupStarting(BackupMsg bm, QTcpSocket* sck)
{
    ui->browseButton->setEnabled(false);

    if(whatAmI() == 3){
        QByteArray byteArray(bm.SerializeAsString().c_str(), bm.ByteSize());
        MagicNode node;
        node.pointer_ = sck;
        node.idMode_ = ACTIVES;
        if(!contained(TransferList_,node))
            TransferList_.push_back(node);
        includePassives();
        busyChannel();
        for(auto entry: TransferList_){
            qDebug() <<"transferlist size: "<< entry.pointer_;
        }
        multicastPassiveTL(byteArray);
    } else {
        QDir dir;
        dir.mkpath(ui->directoryLine->text() + "/" + QString::fromStdString(bm.origin_()));
    }
}

void BackupWindow::scanDirectory(QDir dir)
{

    QDirIterator it(dir.absolutePath(),QDirIterator::Subdirectories);

    while(it.hasNext()){
        QFileInfo fileInfo(it.next());
        if(fileInfo.isDir()){
            BackupMsg content;
            content.set_type_(7);
            content.set_origin_(ui->myIpLabel->text().toStdString());
            content.set_role_(whatAmI());
            content.set_nameofthing(fileInfo.absolutePath().toStdString());
            content.set_fileordir(0);


            qDebug() << QString::fromStdString(content.nameofthing());
            QByteArray byteArray(content.SerializeAsString().c_str(), content.ByteSize());
            PackagesQueue_.enqueue(byteArray);
        }
        if(fileInfo.isFile()){
            qint64 sz = fileInfo.size();
            QString file_name = fileInfo.fileName();
            QString file_path = fileInfo.path();

            MyFiles FileData;
            FileData.size_ = sz;
            FileData.name_ = file_name;
            FileData.path_ = file_path;

            FileQueue_.enqueue(FileData);

        }
    }

    if(!PackagesQueue_.empty())
        MyMagicObject_->getTheSocket()->write(PackagesQueue_.dequeue());

}

void BackupWindow::mountDirAndFiles(BackupMsg bm)
{
    if(whatAmI() == 3){
        QByteArray byteArray(bm.SerializeAsString().c_str(), bm.ByteSize());
        multicastPassiveTL(byteArray);
    } else {
        if(bm.fileordir() == 0){
            QDir dir(ui->directoryLine->text() + "/" + QString::fromStdString(bm.origin_()));
            dir.mkpath(dir.absolutePath() + "/" + QString::fromStdString(bm.nameofthing()));

            BackupMsg checked;
            checked.set_type_(8);
            checked.set_role_(whatAmI());
            QByteArray byteArray(checked.SerializeAsString().c_str(), checked.ByteSize());
            MyMagicObject_->getTheSocket()->write(byteArray);
         }
    }
}

void BackupWindow::checkedPacks(BackupMsg bm)
{
    if(whatAmI() == 3){
        QByteArray byteArray(bm.SerializeAsString().c_str(), bm.ByteSize());
        if(bm.role_() == PASSIVES){
            checkPack_++;
        }

        if(checkPack_ == TransferList_.size()-1){
            multicastActiveTL(byteArray);
            checkPack_ = 0;
        }
    } else {
        if(!PackagesQueue_.empty())
            MyMagicObject_->getTheSocket()->write(PackagesQueue_.dequeue());
    }

}

void BackupWindow::includePassives()
{
    for(int i = 0; i < MagicList_.size() ; i++){
        if(MagicList_[i].idMode_ == PASSIVES){
            if(!contained(TransferList_,MagicList_[i]))
                TransferList_.push_back(MagicList_[i]);
        }
    }
}

void BackupWindow::busyChannel()
{
    BackupMsg bc;
    bc.set_type_(9);
    bc.set_role_(whatAmI());

    QByteArray byteArray(bc.SerializeAsString().c_str(), bc.ByteSize());


    for(int i = 0; i < MagicList_.size() ; i++){
        if(MagicList_[i].idMode_ == ACTIVES){
            if(!contained(TransferList_,MagicList_[i]))
                MagicList_[i].pointer_->write(byteArray);

        }
    }
}

void BackupWindow::channelBusy()
{
    ui->sendButton->setEnabled(false);

    returnMyIp();
}

bool BackupWindow::contained(QVector<MagicNode> tl, MagicNode mn)
{
    for(int i = 0; i < tl.size() ; i++){
        if(tl[i].pointer_ == mn.pointer_)
            return true;
    }

    return false;
}

void BackupWindow::removeThatItem(QVector<MagicNode>& tl, MagicNode mn)
{
    for(int i = 0; i < tl.size() ; i++){
        if(tl[i].pointer_ == mn.pointer_){
            tl.removeAt(i);
            return;
        }
    }
}

void BackupWindow::removeAll(QVector<MagicNode>& tl)
{
    for(int i = 0; i < tl.size() ; i++){
        tl.removeAt(i);
    }
}
