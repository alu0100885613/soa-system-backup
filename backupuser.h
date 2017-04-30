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
        getTheSocket();
    }

    int getWhatAmI(){
        return IamID;
    }

};

#endif // BACKUPUSER_H
