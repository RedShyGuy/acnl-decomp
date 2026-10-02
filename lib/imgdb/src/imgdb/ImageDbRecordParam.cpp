#include "imgdb/ImageDbRecordParam.h"

namespace imgdb {
// 0x005AB260 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordParam::Initialize(imgdb::ImageKind)
{
}

// 0x005AB2A8 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordParam::Initialize()
{
}

// 0x005AB2D4 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordParam::ResetBodyId(imgdb::BodyIdType)
{
}

// 0x005AB2FC | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordParam::SetBodyId(imgdb::BodyIdType, unsigned)
{
}

// 0x00755CDC | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordParam::IsValidBodyId(imgdb::BodyIdType) const
{
}

} // namespace imgdb
