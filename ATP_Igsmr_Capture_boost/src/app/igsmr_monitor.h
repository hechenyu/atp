#ifndef __igsmr_monitor_h
#define __igsmr_monitor_h

#include "boost/shared_ptr.hpp"
#include "collection_data.h"
#include "igsmr_serial_port.h"
#include "igsmr_udp_sender.h"
#include "igsmr_file_writer.h"

class IgsmrMonitor {
private:
    boost::shared_ptr<IgsmrSerialPort> pMT_DTE;
    IgsmrSerialPort::Status MT_DTE_Status;
    boost::shared_ptr<IgsmrSerialPort> pMT_DCE;
    IgsmrSerialPort::Status MT_DCE_Status;
    boost::shared_ptr<IgsmrUdpSender>  pUdp;
    boost::shared_ptr<IgsmrFileWriter> pFile; 
    int poll_timeout_;

    void process_data(boost::shared_ptr<CollectionData> pdata);
    void process_signal();
    void process_dte_signal();
    void process_dce_signal();

public:
    IgsmrMonitor(int MTIndex, const std::string &DTESerial, const std::string &DCESerial);

    void run();
};

#endif
