#include "igsmr_serial_port.h"

IgsmrSerialPort::IgsmrSerialPort(int mt_index, SourceType data_source):
    mt_index_(mt_index), data_source_(data_source)
{
}

boost::shared_ptr<CollectionData> IgsmrSerialPort::readData()
{
    boost::shared_ptr<CollectionData> pdata(new CollectionData);

    pdata->MT = mt_index_;
    pdata->DataSource = data_source_;
    pdata->DataType = 1;
    pdata->Length = this->read(pdata->Data, MT_BUFFER_LEN);

    return pdata;
}
