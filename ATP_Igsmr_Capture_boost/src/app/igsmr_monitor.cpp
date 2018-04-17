#include "igsmr_monitor.h"

IgsmrMonitor::IgsmrMonitor(unsigned char MTIndex, 
        const std::string &DTESerial, const std::string &DCESerial)
{
    pMT_DTE.reset(new IgsmrSerialPort(DTESerial, MTIndex, IgsmrSerialPort::DTE));
    pMT_DCE.reset(new IgsmrSerialPort(DCESerial, MTIndex, IgsmrSerialPort::DCE));
}


void IgsmrMonitor::run()
{
    std::vector<TtyReader *> readers;
    readers.push_back(pMT_DTE.get());
    readers.push_back(pMT_DCE.get());

    TtyReader::Poller poller;
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

        }
    }
}
