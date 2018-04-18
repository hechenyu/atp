#ifdef DEBUG
#include <iostream>
#endif

#include "boost/lexical_cast.hpp"
#include "igsmr_config.h"
#include "igsmr_monitor.h"

using namespace std;

IgsmrMonitor::IgsmrMonitor(int MTIndex, 
        const std::string &DTESerial, const std::string &DCESerial)
{
    pMT_DTE.reset(new IgsmrSerialPort(DTESerial, MTIndex, IgsmrSerialPort::DTE));
    pMT_DCE.reset(new IgsmrSerialPort(DCESerial, MTIndex, IgsmrSerialPort::DCE));

    IgsmrConfig &config = IgsmrConfig::getInstance();
    string serv_ip = config.getIPAddress();
    int serv_port = config.getPort();
    pUdp.reset(new IgsmrUdpSender(serv_ip, serv_port));

    string prefix = config.getFilePrefix()+boost::lexical_cast<string>((int)MTIndex)+"_";
    int slice_size = config.getFileSliceSize();
    pFile.reset(new IgsmrFileWriter(prefix, "", slice_size)); 
}

void IgsmrMonitor::run()
{
    std::vector<TtyReader *> readers;
    readers.push_back(pMT_DTE.get());
    readers.push_back(pMT_DCE.get());

    TtyReader::Poller poller;
    poller.watch(readers);

    for ( ; ; ) {
        int nready = poller.poll(-1);

        if (nready == 0)
            continue;

        const std::vector<struct pollfd> &client = poller.getEvents();
        const std::vector<TtyReader *> &ttys = poller.getReaders();

        for (int i = 0; i < client.size(); i++) {
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
#ifdef DEBUG
                cout << "event error of '" << tty->getDeviceName() << "' " << endl;
#endif
                // DOTO: 做相关的错误处理
                poller.unwatch(tty);
                tty->close();
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
