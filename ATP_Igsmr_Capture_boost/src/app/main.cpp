#include <iostream>
#include "igsmr_config.h"
#include "igsmr_monitor_app.h"

using namespace std;

int main(int argc, char *argv[])
{
    IgsmrConfig::initialize(argc, argv); 
    IgsmrConfig::getInstance().print(cout);

    IgsmrMonitorApp app;
    app.run();

    return 0;
}

