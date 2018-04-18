#ifndef __igmsr_file_serializer_h
#define __igmsr_file_serializer_h

#include "collection_data.h"
#include <stdint.h>
#include <utility>

#pragma pack(1)
struct IgsmrFileStruct {
    uint32_t utc_high;
    uint32_t utc_low;
    uint8_t mt;
    uint8_t data_src;
    uint8_t data_type;
    uint16_t data_len;
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
