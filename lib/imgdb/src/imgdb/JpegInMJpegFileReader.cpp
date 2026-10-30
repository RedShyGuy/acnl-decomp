#include "imgdb/JpegInMJpegFileReader.h"

namespace imgdb {

// 0x009826CA
bool imgdb::JpegInMJpegFileReader::s_IsDefaultLimitFileSizeSet;
// 0x009826D4
u32 imgdb::JpegInMJpegFileReader::s_DefaultLimitFileSize;

// 0x005AB628 | nintendogs:bytes [tier B]
void imgdb::JpegInMJpegFileReader::SetDefaultLimitFileSize(u32 size)
{
    s_DefaultLimitFileSize = size;
    s_IsDefaultLimitFileSizeSet = true;
}

} // namespace imgdb
