#include <string>
#include "glog/logging.h"
#include "boost/thread.hpp"
#include "wrapsignal.h"
#include "igsmr_config.h"
#include "igsmr_monitor_app.h"

using namespace std;

namespace {

volatile int g_sigint_flag = 0;

void sigint_process(int signo)
{
    LOG(WARNING) << "catch SIGINT";
    g_sigint_flag = 1;
}

}   // namespace

IgsmrMonitorApp::IgsmrMonitorApp()
{
    IgsmrConfig &config = IgsmrConfig::getInstance();
    string MT1DTESerial = config.getMT1DTESerial();
    string MT1DCESerial = config.getMT1DCESerial();
    string MT2DTESerial = config.getMT2DTESerial();
    string MT2DCESerial = config.getMT2DCESerial();

    pMT1.reset(new IgsmrMonitor(1, MT1DTESerial, MT1DCESerial));
    pMT2.reset(new IgsmrMonitor(2, MT2DTESerial, MT2DCESerial));

    Signal(SIGINT, &sigint_process);
}

void IgsmrMonitorApp::run()
{
    boost::thread thr1(&IgsmrMonitor::run, pMT1);
    boost::thread thr2(&IgsmrMonitor::run, pMT2);

    while (!g_sigint_flag) {
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }

    thr1.interrupt();
    thr2.interrupt();

    thr1.join();
    thr2.join();
}
