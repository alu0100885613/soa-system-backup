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
        Q_ASSERT_X(false,"Server like a User","You are using the Server like a User");
        return NULL;
    }

    int getWhatAmI(){
        return IamID;
    }

};
#endif // BACKUPSERVER_H
