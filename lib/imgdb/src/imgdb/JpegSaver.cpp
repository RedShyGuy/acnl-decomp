#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "imgdb/JpegMpBaseSaver.h"
#include "imgdb/JpegSaver.h"

namespace imgdb {
// 0x005B15FC slot 0x00 | virtual slot, introduced by imgdb::JpegMpBaseSaver
void imgdb::JpegSaver::vf_0x00()
{
}

// 0x005B15A0 slot 0x04 | virtual slot, introduced by imgdb::JpegMpBaseSaver
void imgdb::JpegSaver::vf_0x04()
{
}

// 0x005B11AC | nintendogs:bytes-fuzzy [tier B]
void imgdb::JpegSaver::Save()
{
}

// 0x005B14FC | nintendogs:bytes [tier B]
void imgdb::JpegSaver::SetRaw(const void*, int, int, nn::jpeg::CTR::PixelFormat)
{
}

// 0x005B1510 | nintendogs:bytes [tier B]
imgdb::JpegSaver::JpegSaver()
{
}

// 0x007567A4 | nintendogs:bytes [tier B]
void imgdb::JpegSaver::ValidateParam() const
{
}

} // namespace imgdb
