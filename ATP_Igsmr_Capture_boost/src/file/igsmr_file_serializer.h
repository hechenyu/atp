#ifndef __igmsr_file_serializer_h
#define __igmsr_file_serializer_h

#include "collection_data.h"
#include <stdint.h>
#include <utility>

#pragma pack(1)
struct IgsmrFileStruct {
    uint8_t mt;
    uint8_t data_src;
    uint8_t data_type;
    uint32_t utc_sec;
    uint16_t utc_millisec;
    uint16_t time_zone;
    uint16_t flag;
    uint32_t data_len;
    uint8_t data[MT_BUFFER_LEN]; 
};
#pragma pack()

class IgsmrFileSerializer {
private:
    IgsmrFileStruct struct_;

public:
    std::pair<void *, int> serialize(const CollectionData &data);
};

#endif
