#include <cstring>

#include "mw/qre/QREncoder.h"
#include "mw/qre/qre_Encoder.h"

namespace mw {
namespace qre {

// 0x00345424
mw::qre::QREncoder::QREncoder()
{
    // empty (in the original too; only the vptr is set)
}

// 0x0034547C
// 0x00345450 (deleting dtor)
mw::qre::QREncoder::~QREncoder()
{
    detail::s_Encoder.ClearImages();
}

// 0x00345378
bool mw::qre::QREncoder::Initialize(void* work, u32 workSize)
{
    if (work == NULL || workSize == 0)
    {
        return false;
    }
    detail::s_WorkState.work = static_cast<u8*>(work);
    detail::s_WorkState.workSize = workSize;
    detail::s_WorkState.workUsed = 0;
    detail::s_WorkState.blockNum = 0;
    std::memset(detail::s_WorkBlocks, 0, sizeof(detail::s_WorkBlocks));
    return true;
}

// the work memory: 94019 bytes per pixel of a symbol module square plus 2.5 MB
// 0x003453B8
u32 mw::qre::QREncoder::GetWorkSize(u32 dataSize, u32 cellSize, u32 divideNum)
{
    if (divideNum != 0)
    {
        return cellSize * cellSize * divideNum * 94019 + 0x280000;
    }
    return (dataSize / 1200 + 1) * (cellSize * cellSize) * 94019 + 0x280000;
}

// 0x00345400
bool mw::qre::QREncoder::Encode(EncodeParam* param)
{
    return detail::s_Encoder.Encode(param);
}

// 0x0034540C
bool mw::qre::QREncoder::GetImage(ImageInfo* image, s32 index)
{
    return detail::s_Encoder.GetImage(image, index);
}

// 0x00345418
u32 mw::qre::QREncoder::GetImageSize(s32 index, bool isBmp)
{
    return detail::s_Encoder.GetImageSize(index, isBmp);
}

} // namespace qre
} // namespace mw
