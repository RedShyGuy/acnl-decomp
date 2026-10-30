#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// only its limit is in the binary (no RTTI, no other function)
class JpegInMJpegFileReader
{
public:
    static void SetDefaultLimitFileSize(u32 size);

private:
    static bool s_IsDefaultLimitFileSizeSet;
    static u32 s_DefaultLimitFileSize;
};

} // namespace imgdb
