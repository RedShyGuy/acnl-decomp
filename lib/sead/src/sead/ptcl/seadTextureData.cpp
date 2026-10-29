#include "sead/ptcl/seadTextureData.h"

namespace sead {
namespace ptcl {
// 0x00555AA4 | mk7dlp:bytes [tier B]
sead::ptcl::TextureData::TextureData() : mTexture()
{
    mImage = nullptr;
    mIsReady = false;
}

// 0x00555B0C slot 0x00
// 0x00555ACC (deleting dtor)
sead::ptcl::TextureData::~TextureData()
{
    if (mImage != nullptr) {
        delete mImage;
    }
    mImage = nullptr;
    mIsReady = false;
}
} // namespace ptcl
} // namespace sead
