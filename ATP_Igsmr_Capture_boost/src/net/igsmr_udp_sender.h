#ifndef __igsmr_udp_sender_h
#define __igsmr_udp_sender_h

#include "collection_data.h"
#include "igsmr_net_serializer.h"
#include "udp_sender.h"

class IgsmrUdpSender {
private:
    std::string serv_ip_;
    int serv_port_;
    UdpSender udp_;
    IgsmrNetSerializer serializer_;

public:
    IgsmrUdpSender(const std::string &serv_ip, int serv_port); 

    void send(const CollectionData &data);
};

#endif
