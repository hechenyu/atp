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

private:
    std::string dev_name_;
    int mt_index_;              // MTIndex
    SourceType data_source_;    // DataSource

public:
    IgsmrSerialPort(const std::string &dev_name, int mt_index, SourceType data_source);

    const std::string &getDeviceName() const;

    boost::shared_ptr<CollectionData> readData(); 
};

#endif
