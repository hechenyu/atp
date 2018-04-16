#ifndef IGSMRMONITORAPP_H
#define IGSMRMONITORAPP_H

#include <QThread>
#include <QtNetwork/QTcpSocket>

#include <igsmrmonitor.h>

class IgsmrMonitorApp : public QThread
{
public:
     IgsmrMonitorApp();
    ~IgsmrMonitorApp();

    void  NewConnect();

private:
    IgsmrMonitor *pMT1;
    IgsmrMonitor *pMT2;
    QTcpSocket *pIgsmrTCPSocket;

   QString  ServerIP;
   quint16 ServerPort;

   QString MT1DTESerial,MT1DCESerial;
   QString MT2DTESerial,MT2DCESerial;

   bool bNetConnected;
   QMutex NetMutex;
protected:
   void run();
   volatile bool stopped;

private slots:
   void disConnect();
};

#endif // IGSMRMONITORAPP_H
