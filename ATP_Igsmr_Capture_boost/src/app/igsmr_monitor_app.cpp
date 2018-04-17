#include <string>
#include "boost/thread.hpp"
#include "igsmr_config.h"
#include "igsmr_monitor_app.h"

using namespace std;

IgsmrMonitorApp::IgsmrMonitorApp()
{
    IgsmrConfig &config = IgsmrConfig::getInstance();
    string MT1DTESerial = config.getMT1DTESerial();
    string MT1DCESerial = config.getMT1DCESerial();
    string MT2DTESerial = config.getMT2DTESerial();
    string MT2DCESerial = config.getMT2DCESerial();

    pMT1.reset(new IgsmrMonitor(1, MT1DTESerial, MT1DCESerial));
    pMT2.reset(new IgsmrMonitor(2, MT2DTESerial, MT2DCESerial));
}

void IgsmrMonitorApp::run()
{
    boost::thread thr1(&IgsmrMonitor::run, pMT1);
    boost::thread thr2(&IgsmrMonitor::run, pMT2);

    thr1.join();
    thr2.join();
}
