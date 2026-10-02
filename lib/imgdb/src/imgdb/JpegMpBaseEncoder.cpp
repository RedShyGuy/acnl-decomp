#include "imgdb/JpegMpBaseEncoder.h"

namespace imgdb {
// ctor address unknown
imgdb::JpegMpBaseEncoder::JpegMpBaseEncoder()
{
}

// 0x0011C12F slot 0x00 | slot vf_0x00 of ChangeRentalBase
imgdb::JpegMpBaseEncoder::~JpegMpBaseEncoder()
{
}

// 0x0011C12F slot 0x04 | slot vf_0x00 of ChangeRentalBase
void imgdb::JpegMpBaseEncoder::vf_0x04()
{
}

// 0x005AB120 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseEncoder::SetSysMakerNoteBodyId(imgdb::BodyIdType, unsigned)
{
}

// 0x005AB144 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseEncoder::ConvTitleUniqueIdToString(char*, int, unsigned)
{
}

} // namespace imgdb
