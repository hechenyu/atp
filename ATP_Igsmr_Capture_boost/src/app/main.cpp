#include <iostream>
#include "wrapglog.h"
#include "daemon_init.h"
#include "glog/logging.h"
#include "igsmr_config.h"
#include "igsmr_monitor_app.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    IgsmrConfig::initialize(argc, argv); 
    IgsmrConfig &config = IgsmrConfig::getInstance();
    config.print(cout);

    if (config.getDaemonMode()) {
        daemon_init();
    }

    Glog_init_logging(config.getLogDir().c_str(), argv[0]);
    Glog_set_max_log_size(1);

    ostringstream os;
    config.print(os);
    LOG(INFO) << "config info:\n" << os.str();

    IgsmrMonitorApp app;
    app.run();

    return 0;
} 

