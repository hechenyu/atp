#ifndef __igsmr_file_wirter_h
#define __igsmr_file_wirter_h

#include <string>
#include "boost/shared_ptr.hpp"
#include "igsmr_file_serializer.h"
#include "auto_timestamp_ofile.h"

class IgsmrFileWriter {
private:
    std::string file_prefix_;
    std::string file_suffix_;
    int file_slice_size_;
    boost::shared_ptr<AutoTimestampOFile> ofile_; 
    IgsmrFileSerializer serializer_;

public:
    IgsmrFileWriter(const std::string &prefix, const std::string suffix, int slice_size);

    void write(const CollectionData &data);
};

#endif
