#ifndef __igsmr_monitor_app_h
#define __igsmr_monitor_app_h

#include "boost/shared_ptr.hpp"
#include "igsmr_monitor.h"

class IgsmrMonitorApp {
private:
    boost::shared_ptr<IgsmrMonitor> pMT1; 
    boost::shared_ptr<IgsmrMonitor> pMT2; 
    bool sigint_flag_;

public:
    IgsmrMonitorApp();

    void run();
};


#endif
