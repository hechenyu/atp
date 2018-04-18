#include <iostream>
#include "wrapglog.h"
#include "glog/logging.h"
#include "igsmr_config.h"
#include "igsmr_monitor_app.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    IgsmrConfig::initialize(argc, argv); 
    IgsmrConfig::getInstance().print(cout);

    Glog_init_logging(IgsmrConfig::getInstance().getLogDir().c_str(), argv[0]);
    Glog_set_max_log_size(1);

    ostringstream os;
    IgsmrConfig::getInstance().print(os);
    LOG(INFO) << "config info:\n" << os.str();

    IgsmrMonitorApp app;
    app.run();

    return 0;
} 

