#include "igsmr_udp_sender.h"

IgsmrUdpSender::IgsmrUdpSender(const std::string &serv_ip, int serv_port):
    serv_ip_(serv_ip), serv_port_(serv_port)
{
    udp_.connect(serv_ip_.c_str(), serv_port_);
}

void IgsmrUdpSender::send(const CollectionData &data)
{
    std::pair<void *, int> serialized_data;
    serialized_data = serializer_.serialize(data);
    udp_.send(static_cast<char *>(serialized_data.first), serialized_data.second);
}

