#ifndef __collection_data_h
#define __collection_data_h

#include <stdint.h>
#include "boost/chrono.hpp"

#define MT_BUFFER_LEN 2048

class CollectionData {
public:
    CollectionData();

    boost::chrono::system_clock::time_point CollectionTime;

    unsigned char MT;
    uint64_t UTCTime;
    unsigned char DataSource;
    unsigned char DataType;
    unsigned short  Length;
    char Data[MT_BUFFER_LEN];
};

#endif // __collection_data_h 
