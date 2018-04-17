#include "igsmr_net_serializer.h"
#include <arpa/inet.h>
#include <string.h>

std::pair<void *, int> IgmsrNetSerializer::serialize(const CollectionData &data)
{
    memset(&struct_, 0, sizeof (struct_));

    struct_.frame_head = 0xffffffff;
    uint16_t frame_len = data.Length + 16;
    struct_.frame_len = htons(frame_len);

    uint32_t utc_high = (uint32_t) (data.UTCTime >> 32);
    uint32_t utc_low = (uint32_t) (data.UTCTime);
    struct_.utc_high = htonl(utc_high);
    struct_.utc_low = htonl(utc_low);

    struct_.apt[0] = 0x1; struct_.apt[1] = 0x1; struct_.apt[2] = 0x1;

    struct_.mt = data.MT;
    struct_.data_src = data.DataSource;
    struct_.data_type = data.DataType;
    uint16_t data_len = data.Length;
    struct_.data_len = htons(data_len);
    memcpy(struct_.data, data.Data, data_len);

    return std::pair<void *, int>(&struct_, frame_len+6);
}

