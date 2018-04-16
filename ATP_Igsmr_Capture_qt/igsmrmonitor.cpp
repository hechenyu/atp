#include "igsmrmonitor.h"


IgsmrMonitor::IgsmrMonitor( unsigned char _MTIndex,QString DTESerial,QString DCESerial)
{   
  pTCPSocket=NULL;
  MTIndex=_MTIndex;

  QDateTime FileTime;
  FileTime = QDateTime::currentDateTime();
  strFileStartTime=FileTime.toString("yyyy.MM.dd_hh.mm.ss");

  strFileStartName.sprintf("/IgsmrRecord/ATP_Igsmr_MT%d_%s",MTIndex,strFileStartTime.toUtf8().data());
  IgsmrFile.setFileName(strFileStartName);
  if(!IgsmrFile.open(QIODevice::WriteOnly))
  {
      strT.sprintf ("Cannot open %s File",strFileStartTime.toUtf8().data());
      qDebug() <<strT<< endl;
  }

  IgsmrDataStream.setDevice(&IgsmrFile);
  IgsmrDataStream.setVersion(QDataStream::Qt_5_5);

  pIgsmrNetStream=new QDataStream(&IgsmrNetMsg,QIODevice::WriteOnly);
  pIgsmrNetStream->setVersion(QDataStream::Qt_5_5);


  pMT_DTE=new QextSerialPort(DTESerial);
  pMT_DTE->open(QIODevice::ReadOnly);
  strT.sprintf ("MT%d_DTE is open: %d", MTIndex, pMT_DTE->isOpen());
  qDebug() <<strT<< endl;

  pMT_DCE=new QextSerialPort(DCESerial);
  pMT_DCE->open(QIODevice::ReadOnly);
  strT.sprintf ("MT%d_DCE is open: %d",MTIndex, pMT_DCE->isOpen());
  qDebug() <<strT<< endl;

  stopped=false;
}

IgsmrMonitor::~IgsmrMonitor()
{
    pMT_DCE->close();
    strT.sprintf("MT%d_DCE is open: %d",MTIndex,  pMT_DCE->isOpen());
    qDebug() <<strT<< endl;
    delete pMT_DCE;

    pMT_DTE->close();
    strT.sprintf("MT%d_DTE is open: %d",MTIndex, pMT_DTE->isOpen());
    qDebug() <<strT<< endl;
    delete pMT_DTE;

    QDateTime FileTime;
    FileTime = QDateTime::currentDateTime();
    strFileEndTime=FileTime.toString("yyyy.MM.dd_hh.mm.ss");

    IgsmrFile.close();
    strFileEndName.sprintf("/IgsmrRecord/ATP_Igsmr_MT%d_%s-%s",MTIndex,strFileStartTime.toUtf8().data(),strFileEndTime.toUtf8().data());
    IgsmrFile.rename(strFileStartName,strFileEndName);
}

void IgsmrMonitor::run()
{
    unsigned long DTELineStatus,DCELineStatus;
    unsigned long DTELineNewStatus,DCELineNewStatus;

    DTELineStatus=0;
    DCELineStatus=0;

     strT.sprintf("MT%d Capture Running....",MTIndex);

    while(!stopped)
    {
        qDebug() <<strT<< endl;

        DTELineNewStatus=pMT_DTE->lineStatus();

        if(DTELineStatus!=DTELineNewStatus)
        {
            if((DTELineStatus&LS_CTS)!=(DTELineNewStatus&LS_CTS))
                 StoreSignal(0,(DTELineNewStatus&LS_CTS)==0? 1:0);

            if((DTELineStatus&LS_DSR)!=(DTELineNewStatus&LS_DSR))
                 StoreSignal(0,(DTELineNewStatus&LS_DSR)==0? 9:8);

            DTELineStatus=DTELineNewStatus;
        }

        DCELineNewStatus=pMT_DTE->lineStatus();
        if(DCELineStatus!=DCELineNewStatus)
        {
            if((DCELineStatus&LS_CTS)!=(DCELineNewStatus&LS_CTS))
                 StoreSignal(0,(DCELineNewStatus&LS_CTS)==0? 1:0);

            if((DCELineStatus&LS_DSR)!=(DCELineNewStatus&LS_DSR))
                 StoreSignal(0,(DCELineNewStatus&LS_DSR)==0?0x19:0x18);

            if((DCELineStatus&LS_DCD)!=(DCELineNewStatus&LS_DCD))
                 StoreSignal(0,(DCELineNewStatus&LS_DCD)==0?0x9:0x8);

            if((DCELineStatus&LS_RI)!=(DCELineNewStatus&LS_RI))
                 StoreSignal(0,(DCELineNewStatus&LS_RI)==0?0x11:0x10);

            DCELineStatus=DCELineNewStatus;
        }

        ReadSerialData(pMT_DTE);
        ReadSerialData(pMT_DCE);

        usleep(300000);
    }
}

void IgsmrMonitor::ReadSerialData(QextSerialPort* pSerialPort)
{
    int numBytes;
    numBytes=pSerialPort->bytesAvailable();
    if(numBytes>0)
    {
        if(numBytes > MT_BUFFER_LEN)  numBytes=MT_BUFFER_LEN;
        pCollectionData=NULL;
        pCollectionData=new CollectionData(this);
        if(pCollectionData)
        {
            pCollectionData->MT=MTIndex;
            pCollectionData->DataSource=0;
            pCollectionData->DataType=2;
            pCollectionData->Length=numBytes;
            pSerialPort->read(pCollectionData->Data,numBytes);
            pCollectionData->StoreData(IgsmrDataStream,*pIgsmrNetStream);
            delete pCollectionData;
            if(pTCPSocket)
            {
               pNetMutex->lock();
               int WriteSize= pTCPSocket->write(IgsmrNetMsg);
               while(WriteSize<IgsmrNetMsg.size())
               {
                   WriteSize+=pTCPSocket->write(IgsmrNetMsg.right(IgsmrNetMsg.size()-WriteSize));
               }
               IgsmrNetMsg.clear();
               pNetMutex->unlock();
            }
        }
    }
}

void IgsmrMonitor::StopCapture()
{
        stopped=true;
}

 void IgsmrMonitor::StoreSignal(uchar Source,uchar SignalType)
 {
     pCollectionData=NULL;
     pCollectionData=new CollectionData(this);
     if(pCollectionData)
     {
         pCollectionData->MT=MTIndex;
         pCollectionData->DataSource=Source;
         pCollectionData->DataType=0;
         pCollectionData->Length=1;
         pCollectionData->Data[0]=SignalType;
         pCollectionData->StoreData(IgsmrDataStream,*pIgsmrNetStream);

         delete pCollectionData;
        }
 }
