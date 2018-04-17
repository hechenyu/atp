#ifndef __igsmr_monitor_h
#define __igsmr_monitor_h

#include "boost/shared_ptr.hpp"
#include "igsmr_serial_port.h"

class IgsmrMonitor {
private:
    boost::shared_ptr<IgsmrSerialPort> pMT_DTE;
    boost::shared_ptr<IgsmrSerialPort> pMT_DCE;

public:
    IgsmrMonitor(unsigned char MTIndex, const std::string &DTESerial, const std::string &DCESerial);

    void run();
};

#endif
