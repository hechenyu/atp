#include "igsmr_file_serializer.h"
#include <arpa/inet.h>
#include <string.h>

namespace {

void dump_data(const char *data, int length) {
    std::ostringstream os;

    os << "write data '" << "' " << length << " bytes: " << std::string(data, length) << " hex: ";
    os << std::hex;
    for (int i = 0; i < length; i++)
        os << (int) data[i] << " ";

    LOG(INFO) << os.str();
}

}   // namespace

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

    uint32_t data_len = data.Length;
    struct_.data_len = data_len;
    memcpy(struct_.data, data.Data, data_len);

#ifdef DEBUG
    dump_data(struct_.data, data_len);
#endif

    return std::pair<void *, int>(&struct_, data_len+17);
}

