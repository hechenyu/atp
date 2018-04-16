#ifndef IGSMRMONITOR_H
#define IGSMRMONITOR_H

#include <QThread>
#include <QMutex>
#include <QFile>
#include <QDataStream>
#include<QtDebug>
#include<QString>
#include <QtNetwork/QTcpSocket>

#include <qextserialport.h>
#include <collectiondata.h>

class IgsmrMonitor :public QThread
{
public:
    IgsmrMonitor( unsigned char _MTIndex,QString DTESerial,QString DCESerial);
    ~IgsmrMonitor();

    void StopCapture();
    void StoreSignal(uchar Source,uchar SignalType);
    void ReadSerialData(QextSerialPort* pSerialPort);

    QString FileName;
    QString strT;
    QString strFileStartTime,strFileEndTime;
    QString strFileStartName,strFileEndName;

    unsigned char MTIndex;
    QFile IgsmrFile;
    QDataStream IgsmrDataStream;

    QByteArray IgsmrNetMsg;
    QDataStream *pIgsmrNetStream;

    QextSerialPort *pMT_DCE;
    QextSerialPort *pMT_DTE;
    CollectionData *pCollectionData;
    QTcpSocket *pTCPSocket;
    QMutex *pNetMutex;

 protected:
    void run();
    volatile bool stopped;
};

#endif // IGSMRMONITOR_H
