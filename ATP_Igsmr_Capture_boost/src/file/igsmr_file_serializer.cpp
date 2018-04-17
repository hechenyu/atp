#include "igsmr_file_serializer.h"
#include <arpa/inet.h>
#include <string.h>

std::pair<void *, int> IgmsrFileSerializer::serialize(const CollectionData &data)
{
    memset(&struct_, 0, sizeof (struct_));

    uint32_t utc_high = (uint32_t) (data.UTCTime >> 32);
    uint32_t utc_low = (uint32_t) (data.UTCTime);
    struct_.utc_high = htonl(utc_high);
    struct_.utc_low = htonl(utc_low);

    struct_.mt = data.MT;
    struct_.data_src = data.DataSource;
    struct_.data_type = data.DataType;
    uint16_t data_len = data.Length;
    struct_.data_len = htons(data_len);
    memcpy(struct_.data, data.Data, data_len);

    return std::pair<void *, int>(&struct_, data_len+13);
}

