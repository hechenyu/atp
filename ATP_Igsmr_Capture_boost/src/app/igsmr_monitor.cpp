#ifdef DEBUG
#include <iostream>
#endif

#include <signal.h>
#include "glog/logging.h"
#include "boost/thread.hpp"
#include "boost/lexical_cast.hpp"
#include "igsmr_config.h"
#include "igsmr_monitor.h"

using namespace std;

IgsmrMonitor::IgsmrMonitor(int MTIndex, 
        const std::string &DTESerial, const std::string &DCESerial):
    poll_timeout_(-1)
{
    pMT_DTE.reset(new IgsmrSerialPort(DTESerial, MTIndex, IgsmrSerialPort::DTE));
    MT_DTE_Status = pMT_DTE->readStatus();
    pMT_DCE.reset(new IgsmrSerialPort(DCESerial, MTIndex, IgsmrSerialPort::DCE));
    MT_DCE_Status = pMT_DCE->readStatus();

    IgsmrConfig &config = IgsmrConfig::getInstance();
    string serv_ip = config.getIPAddress();
    int serv_port = config.getPort();
    pUdp.reset(new IgsmrUdpSender(serv_ip, serv_port));

    string prefix = config.getFilePrefix()+boost::lexical_cast<string>((int)MTIndex)+"_";
    int slice_size = config.getFileSliceSize();
    pFile.reset(new IgsmrFileWriter(prefix, "", slice_size)); 

    poll_timeout_ = config.getPollTimeout();
}

void IgsmrMonitor::run()
{
    std::vector<TtyReader *> readers;
    readers.push_back(pMT_DTE.get());
    readers.push_back(pMT_DCE.get());

    TtyReader::Poller poller;
    poller.watch(readers);

    int count = 0;
    int check_status_per_count = 10;
    for ( ; ; ) {
        boost::this_thread::interruption_point();

        if (count % check_status_per_count == 0) {
            process_signal();
        }

        int nready = poller.poll(poll_timeout_);

        if (nready == 0) {
            count = 0;
            continue;
        }

        count++;
        const std::vector<struct pollfd> &client = poller.getEvents();
        const std::vector<TtyReader *> &ttys = poller.getReaders();

        for (size_t i = 0; i < client.size(); i++) {
            int tty_fd = client[i].fd;
            if (tty_fd < 0)
                continue;

            IgsmrSerialPort *tty = static_cast<IgsmrSerialPort *>(ttys[i]);
            assert(tty_fd == tty->fileno());

#ifdef DEBUG
            printf(" fd=%d; tty=%s, events: %s%s%s%s%s\n", 
                    tty_fd,
                    tty->getDeviceName().c_str(),
                    (client[i].revents & POLLIN) ? "POLLIN " : "",
                    (client[i].revents & POLLOUT) ? "POLLOUT " : "",
                    (client[i].revents & POLLRDHUP) ? "POLLRDHUP " : "",
                    (client[i].revents & POLLHUP) ? "POLLHUP " : "",
                    (client[i].revents & POLLERR) ? "POLLERR " : "");
#endif

            if (client[i].revents & POLLERR) {
                LOG(ERROR) << "event error of '" << tty->getDeviceName() << "' " << endl;

                // DOTO: 做相关的错误处理
                poller.unwatch(tty);
                tty->close();
                kill(getpid(), SIGINT);
                continue;
            }

            if (client[i].revents & POLLIN) {
                boost::shared_ptr<CollectionData> pdata = tty->readData();
                process_data(pdata);
            }

        }
    }
}

void IgsmrMonitor::process_data(boost::shared_ptr<CollectionData> pdata)
{
    pUdp->send(*pdata);
    pFile->write(*pdata);
}

void IgsmrMonitor::process_signal()
{
    process_dte_signal();
    process_dce_signal();
}

void IgsmrMonitor::process_dte_signal()
{
    IgsmrSerialPort::Status newStatus = pMT_DTE->readStatus();

    if (MT_DTE_Status.CTS != newStatus.CTS)
        process_data(pMT_DTE->makeStatusCTS(newStatus));

    if (MT_DTE_Status.DSR != newStatus.DSR)
        process_data(pMT_DTE->makeStatusDSR(newStatus));

    MT_DTE_Status = newStatus;
}

void IgsmrMonitor::process_dce_signal()
{
    IgsmrSerialPort::Status newStatus = pMT_DCE->readStatus();

    if (MT_DCE_Status.CTS != newStatus.CTS)
        process_data(pMT_DCE->makeStatusCTS(newStatus));

    if (MT_DCE_Status.DSR != newStatus.DSR)
        process_data(pMT_DCE->makeStatusDSR(newStatus));

    if (MT_DCE_Status.DCD != newStatus.DCD)
        process_data(pMT_DCE->makeStatusDCD(newStatus));

    if (MT_DCE_Status.RI != newStatus.RI)
        process_data(pMT_DCE->makeStatusRI(newStatus));

    MT_DCE_Status = newStatus;
}
