#ifndef __igsmr_serial_port_h
#define __igsmr_serial_port_h

#include <string>
#include "boost/shared_ptr.hpp"
#include "tty_reader.h"
#include "collection_data.h"

class IgsmrSerialPort: public TtyReader {
public:
    enum SourceType {
        DTE = 0,
        DCE = 1,
    };

    struct Status {
        bool CTS;
        bool DCD;
        bool RI;
        bool DSR;
    };

private:
    std::string dev_name_;
    int mt_index_;              // MTIndex
    SourceType data_source_;    // DataSource

public:
    IgsmrSerialPort(const std::string &dev_name, int mt_index, SourceType data_source);

    const std::string &getDeviceName() const;

    boost::shared_ptr<CollectionData> readData(); 
    Status readStatus();

    boost::shared_ptr<CollectionData> makeStatusCommon(uint8_t signalType);
    boost::shared_ptr<CollectionData> makeStatusCTS(Status status);
    boost::shared_ptr<CollectionData> makeStatusDCD(Status status);
    boost::shared_ptr<CollectionData> makeStatusRI(Status status);
    boost::shared_ptr<CollectionData> makeStatusDSR(Status status);
};

#endif
