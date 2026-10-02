#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "imgdb/JpegMpBaseSaver.h"
#include "imgdb/MpSaver.h"

namespace imgdb {
// 0x005B0920 slot 0x00 | virtual slot, introduced by imgdb::JpegMpBaseSaver
void imgdb::MpSaver::vf_0x00()
{
}

// 0x005B08C4 slot 0x04 | virtual slot, introduced by imgdb::JpegMpBaseSaver
void imgdb::MpSaver::vf_0x04()
{
}

// 0x005B04B8 | nintendogs:bytes-fuzzy [tier B]
void imgdb::MpSaver::Save()
{
}

// 0x005B0808 | nintendogs:bytes [tier B]
void imgdb::MpSaver::SetRaw(const void*, const void*, int, int, nn::jpeg::CTR::PixelFormat)
{
}

// 0x005B0824 | nintendogs:bytes [tier B]
imgdb::MpSaver::MpSaver()
{
}

// 0x0075612C | nintendogs:bytes [tier B]
void imgdb::MpSaver::ValidateParam() const
{
}

} // namespace imgdb
