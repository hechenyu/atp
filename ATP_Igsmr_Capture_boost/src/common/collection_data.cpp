#include "collection_data.h"
#include <string.h>

using namespace boost::chrono;

CollectionData::CollectionData()
{
    MT=0;
    DataSource=0;
    DataType=0;
    memset(Data,0,sizeof(Data));

    CollectionTime=system_clock::now();
    system_clock::duration dtn = CollectionTime.time_since_epoch();
    UTCTime = dtn.count() * system_clock::period::num / (system_clock::period::den / 1000);
}

