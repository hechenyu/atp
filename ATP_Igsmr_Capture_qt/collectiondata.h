#ifndef COLLECTIONDATA_H
#define COLLECTIONDATA_H

#include <QObject>
#include<QDateTime>

#define MT_BUFFER_LEN 2048

class CollectionData : public QObject
{
    Q_OBJECT
public:
    explicit CollectionData(QObject *parent = 0);

   QDateTime CollectionTime;

    unsigned char  MT;
    qint64 UTCTime;
    unsigned char DataSource;
    unsigned char DataType;
    unsigned short  Length;
    char Data[MT_BUFFER_LEN];
    CollectionData();

    void StoreData(QDataStream &MTDataFileStream,QDataStream &MTDataNetStream);

signals:

public slots:
};

#endif // COLLECTIONDATA_H
