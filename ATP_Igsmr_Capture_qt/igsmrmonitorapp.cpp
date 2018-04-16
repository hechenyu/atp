#include "igsmrmonitorapp.h"
#include<QSettings>

IgsmrMonitorApp::IgsmrMonitorApp()
{
    QSettings *IgsmrCaptureSet = new QSettings("IgsmrCaptureSet.ini",QSettings::IniFormat);

    MT1DTESerial=IgsmrCaptureSet->value("/Serial/MT1DTESerial").toString();
    MT1DCESerial=IgsmrCaptureSet->value("/Serial/MT1DCESerial").toString();
    MT2DTESerial=IgsmrCaptureSet->value("/Serial/MT2DTESerial").toString();
    MT2DCESerial=IgsmrCaptureSet->value("/Serial/MT2DCESerial").toString();

    ServerIP=IgsmrCaptureSet->value("/Net/IPAddress").toString();
    ServerPort=IgsmrCaptureSet->value("/Net/Port").toInt();

    delete IgsmrCaptureSet;

    pMT1=new IgsmrMonitor(1,MT1DTESerial,MT1DCESerial);
    pMT2=new IgsmrMonitor(2,MT2DTESerial,MT2DCESerial);

    pMT1->pNetMutex=&NetMutex;
    pMT2->pNetMutex=&NetMutex;

    pMT1->start();
    pMT2->start();
    start();
}

IgsmrMonitorApp::~IgsmrMonitorApp()
{
   pMT1->StopCapture();
   pMT2->StopCapture();

   pMT1->wait();
   pMT2->wait();

   if(pMT1)delete pMT1;
   if(pMT2)delete pMT2;
}

void IgsmrMonitorApp::NewConnect()
{
    pIgsmrTCPSocket = new QTcpSocket(this);
    bNetConnected=false;
    while(!bNetConnected)
    {
        QString strT;
        strT.sprintf("Net Connect Beging....");
        qDebug() <<strT<< endl;

        int constintTimeout=5*1000;
        pIgsmrTCPSocket->connectToHost(ServerIP,ServerPort);
        bNetConnected = pIgsmrTCPSocket->waitForConnected(constintTimeout);
        if(bNetConnected)
        {
            connect(pIgsmrTCPSocket,SIGNAL(disconnected()),this,SLOT(disConnect()));
            pMT1->pTCPSocket=pIgsmrTCPSocket;
            pMT2->pTCPSocket=pIgsmrTCPSocket;
        }

        sleep(5);
    }
}

void  IgsmrMonitorApp::disConnect()
{
    bNetConnected=false;
    pMT1->pTCPSocket=NULL;
    pMT2->pTCPSocket=NULL;

    if(pIgsmrTCPSocket)
   {
        pIgsmrTCPSocket->abort();
        delete pIgsmrTCPSocket;
    }
   start();
}

void IgsmrMonitorApp::run()
{
    NewConnect();
}
