#include "igsmr_monitor.h"

IgsmrMonitor::IgsmrMonitor(unsigned char MTIndex, 
        const std::string &DTESerial, const std::string &DCESerial)
{
    pMT_DTE.reset(new IgsmrSerialPort(DTESerial, MTIndex, IgsmrSerialPort::DTE));
    pMT_DCE.reset(new IgsmrSerialPort(DCESerial, MTIndex, IgsmrSerialPort::DCE));
}
