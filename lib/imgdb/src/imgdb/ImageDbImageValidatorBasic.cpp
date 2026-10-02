#include "imgdb/ImageDbImageValidatorBasic.h"

namespace imgdb {
// 0x005AB924 | nintendogs:bytes [tier B]
void imgdb::ImageDbImageValidatorBasic::ValidateMp(const void*, unsigned, long long)
{
}

// 0x005ABB04 | nintendogs:bytes [tier B]
void imgdb::ImageDbImageValidatorBasic::ValidateJpeg(const void*, unsigned, long long)
{
}

// 0x005ABCE4 | nintendogs:bytes [tier B]
void imgdb::ImageDbImageValidatorBasic::ValidateImage(imgdb::StorageType, const imgdb::ImageInfo&, bool)
{
}

// 0x005ABDEC | nintendogs:bytes [tier B]
imgdb::ImageDbImageValidatorBasic::ImageDbImageValidatorBasic()
{
}

} // namespace imgdb
