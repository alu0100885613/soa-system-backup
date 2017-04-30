#ifndef BACKUPSERVER_H
#define BACKUPSERVER_H

#include "magicobject.h"

class BackupServer: public MagicObject{
private:
    QTcpServer* MyServer_;
    int IamID;

public:
    BackupServer(int id):
        MyServer_(new QTcpServer),
        IamID(id)
    {}

    ~BackupServer(){
        MyServer_->close();
        if(MyServer_ != NULL)
            delete MyServer_;
        MyServer_ = NULL;
    }

    QTcpServer* getTheServer(){
        return MyServer_;
    }

    QTcpSocket* getTheSocket(){
        getTheServer();
    }

    int getWhatAmI(){
        return IamID;
    }

};
#endif // BACKUPSERVER_H
