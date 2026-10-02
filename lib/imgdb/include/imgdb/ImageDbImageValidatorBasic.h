#pragma once

#include "decomp.h"

namespace imgdb {
class ImageDbImageValidatorBasic
{
public:
    void ValidateMp(const void*, unsigned, long long); // 0x005AB924 | nintendogs:bytes [tier B]
    void ValidateJpeg(const void*, unsigned, long long); // 0x005ABB04 | nintendogs:bytes [tier B]
    void ValidateImage(imgdb::StorageType, const imgdb::ImageInfo&, bool); // 0x005ABCE4 | nintendogs:bytes [tier B]
    ImageDbImageValidatorBasic(); // 0x005ABDEC | nintendogs:bytes [tier B]
};
} // namespace imgdb
