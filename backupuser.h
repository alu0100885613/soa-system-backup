#ifndef BACKUPUSER_H
#define BACKUPUSER_H

#include "magicobject.h"

class BackupUser: public MagicObject{
private:
    QTcpSocket* MySocket_;
    int IamID;

public:
    BackupUser(int id):
        MySocket_(new QTcpSocket),
        IamID(id)
    {}

    BackupUser(QTcpSocket* sck):
        MySocket_(sck),
        IamID(0)
    {}

    ~BackupUser(){
        MySocket_->abort();
        if(MySocket_ != NULL)
            delete MySocket_;
        MySocket_ = NULL;
    }

    QTcpSocket* getTheSocket(){
        return MySocket_;
    }

    QTcpServer* getTheServer(){
        Q_ASSERT_X(false,"User like a Server","You are using the User like a Server");
        return NULL;
    }

    int getWhatAmI(){
        return IamID;
    }

};

#endif // BACKUPUSER_H
