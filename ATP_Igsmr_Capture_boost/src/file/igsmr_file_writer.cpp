#include "igsmr_file_writer.h"

IgsmrFileWriter::IgsmrFileWriter(
        const std::string &prefix, const std::string suffix, int slice_size)
    : file_prefix_(prefix), file_suffix_(suffix), file_slice_size_(slice_size)
{
    ofile_.reset(new AutoTimestampOFile(file_prefix_, file_suffix_));
}

void IgsmrFileWriter::write(const CollectionData &data)
{
    std::pair<void *, int> serialized_data;
    serialized_data = serializer_.serialize(data);
    ofile_->write(static_cast<char *>(serialized_data.first), serialized_data.second);
    ofile_->flush();
    if (ofile_->size() >= file_slice_size_) {
        ofile_.reset(new AutoTimestampOFile(file_prefix_, file_suffix_));
    }
}
