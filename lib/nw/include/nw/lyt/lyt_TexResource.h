#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_ResourceFormat.h"

namespace nw {
namespace lyt {
// A BCLIM image in memory: the image data and the 'imag' block of the file header at its end
// (see 3dbrew "BCLIM"). Member names are ours.
class TexResource
{
public:
    // (inline)
    TexResource() : m_pData(nullptr), m_pImage(nullptr), m_pFileSize(nullptr) {}

    // the image of size bytes; false (all NULL) if it is no valid BCLIM
    bool Set(void* pData, u32 size); // 0x004B4EA4 | nintendogs:bytes [tier B]

    void* m_pData;                  // 0x0
    const res::Image* m_pImage;     // 0x4 'imag'
    const u32* m_pFileSize;         // 0x8 the last word: the size of the image data
};
ASSERT_SIZE(TexResource, 0xC);
} // namespace lyt
} // namespace nw
