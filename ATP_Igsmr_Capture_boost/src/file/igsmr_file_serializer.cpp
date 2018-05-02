#include "igsmr_file_serializer.h"
#include <arpa/inet.h>
#include <string.h>

std::pair<void *, int> IgsmrFileSerializer::serialize(const CollectionData &data)
{
    memset(&struct_, 0, sizeof (struct_));

    struct_.mt = data.MT;
    struct_.data_src = data.DataSource;
    struct_.data_type = data.DataType;

    struct_.utc_sec = (uint32_t) (data.UTCTime / 1000);
    struct_.utc_millisec = (uint32_t) (data.UTCTime % 1000);
    struct_.time_zone = 0;
    struct_.flag = 1;

    uint16_t data_len = data.Length;
    struct_.data_len = data_len;
    memcpy(struct_.data, data.Data, data_len);

    return std::pair<void *, int>(&struct_, data_len+13);
}

