#ifdef DEBUG
#include <iostream>
#endif

#include "glog/logging.h"
#include "wraptermios.h"
#include "igsmr_serial_port.h"

namespace {

void dump_data(const std::string &dev_name, const char *data, int length) {
    std::ostringstream os;

    os << "read data from '" << dev_name << "' " << length << " bytes: " << std::string(data, length) << " hex: ";
    os << std::hex;
    for (int i = 0; i < length; i++)
        os << (int) data[i] << " ";

    LOG(INFO) << os.str();
}

}   // namespace

IgsmrSerialPort::IgsmrSerialPort(const std::string &dev_name, 
        int mt_index, SourceType data_source):
    dev_name_(dev_name), mt_index_(mt_index), data_source_(data_source)
{
    this->open(dev_name_.c_str());
    this->setRawMode();
    this->setSpeed(9600);
    this->setParity(8, 1, 'N');
    this->setIcanon(0, 0);
    this->setTimeout(1, 0, 500);
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
    dump_data(dev_name_, pdata->Data, pdata->Length);
#endif

    return pdata;
}

IgsmrSerialPort::Status IgsmrSerialPort::readStatus()
{
    Status status;
    int serial = 0;
    try {
        serial = this->getModemStatus();
    } catch (...) {
        LOG(ERROR) << "getModemStatus fail";
    }
    status.CTS = TTY_MODEM_STATUS_CTS(serial);
    status.DCD = TTY_MODEM_STATUS_DCD(serial);
    status.RI = TTY_MODEM_STATUS_RI(serial);
    status.DSR = TTY_MODEM_STATUS_DSR(serial);

    return status;
}

boost::shared_ptr<CollectionData> IgsmrSerialPort::makeStatusCommon(uint8_t signalType)
{
    boost::shared_ptr<CollectionData> pdata(new CollectionData);

    pdata->MT = mt_index_;
    pdata->DataSource = data_source_;
    pdata->DataType = 0;
    pdata->Length = 1;
    pdata->Data[0] = signalType;

    return pdata;
}

boost::shared_ptr<CollectionData> IgsmrSerialPort::makeStatusCTS(Status status)
{
    int signalType = status.CTS ? 0x0 : 0x1;
    return makeStatusCommon(signalType);
}

boost::shared_ptr<CollectionData> IgsmrSerialPort::makeStatusDCD(Status status)
{
    int signalType = status.DCD ? 0x8 : 0x9;
    return makeStatusCommon(signalType);
}

boost::shared_ptr<CollectionData> IgsmrSerialPort::makeStatusRI(Status status)
{
    int signalType = status.RI ? 0x10 : 0x11;
    return makeStatusCommon(signalType);
}

boost::shared_ptr<CollectionData> IgsmrSerialPort::makeStatusDSR(Status status)
{
    int signalType = status.DSR ? 0x18 : 0x19;
    return makeStatusCommon(signalType);
}
