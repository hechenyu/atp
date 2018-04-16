#include "collectiondata.h"


CollectionData::CollectionData(QObject *parent) : QObject(parent)
{
        MT=1;
        DataSource=0;
        DataType=0;
        memset(Data,0,sizeof(Data));

        CollectionTime=QDateTime::currentDateTimeUtc();
        UTCTime=CollectionTime.toMSecsSinceEpoch();
}

 void CollectionData::StoreData(QDataStream &MTDataFileStream,QDataStream &MTDataNetStream)
 {
        qint16 i;
        MTDataFileStream <<qint32(UTCTime>>32);
        MTDataFileStream <<qint32(UTCTime);
        MTDataFileStream<<MT<<DataSource<<DataType<<Length;
        for(i=0;i<Length;i++)
            MTDataFileStream<<Data[i];

        qint16 PacketLen=16+Length;
        for(i=0;i<4;i++)
             MTDataNetStream<<0xff;
        MTDataNetStream<<PacketLen;
        MTDataNetStream <<qint32(UTCTime>>32);
        MTDataNetStream <<qint32(UTCTime);
        //ATP ID
        for(i=0;i<3;i++)
             MTDataNetStream<<0x1;
        MTDataNetStream<<MT<<DataSource<<DataType<<Length;
        for(i=0;i<Length;i++)
            MTDataNetStream<<Data[i];
 }
