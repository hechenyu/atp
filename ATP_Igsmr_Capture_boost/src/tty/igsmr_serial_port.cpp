#ifdef DEBUG
#include <iostream>
#endif

#include "igsmr_serial_port.h"

IgsmrSerialPort::IgsmrSerialPort(const std::string &dev_name, 
        int mt_index, SourceType data_source):
    dev_name_(dev_name), mt_index_(mt_index), data_source_(data_source)
{
    this->open(dev_name_.c_str());
}

const std::string &IgsmrSerialPort::getDeviceName() const
{
    return dev_name_;
}

boost::shared_ptr<CollectionData> IgsmrSerialPort::readData()
{
    boost::shared_ptr<CollectionData> pdata(new CollectionData);

    pdata->MT = mt_index_;
    pdata->DataSource = data_source_;
    pdata->DataType = 1;
    pdata->Length = this->read(pdata->Data, MT_BUFFER_LEN);

#ifdef DEBUG
    std::cout << "read data from '" << dev_name_ << "' " << pdata->Length << " bytes: " << std::string(pdata->Data, pdata->Length) << std::endl;
#endif

    return pdata;
}
