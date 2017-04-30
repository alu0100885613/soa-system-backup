#ifndef MAGICOBJECT_H
#define MAGICOBJECT_H
#include <QTcpServer>
#include <QTcpSocket>

class MagicObject {

public:
    virtual int getWhatAmI() = 0;
    virtual QTcpServer* getTheServer() = 0;
    virtual QTcpSocket* getTheSocket() = 0;

};


#endif // MAGICOBJECT_H
