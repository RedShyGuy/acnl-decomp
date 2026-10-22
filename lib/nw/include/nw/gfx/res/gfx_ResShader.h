#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
class ResShader
{
public:
    // (inline)
    explicit ResShader(const void* pData = nullptr) : m_pData(pData) {}
    const void* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }

    void Cleanup(); // 0x001378BC | nintendogs:bytes-fuzzy [tier A]
    void Dereference(); // 0x004AA6C8 | nintendogs:bytes [tier A]
    u32 Setup(nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004AA708 | nintendogs:bytes [tier B]

    const void* m_pData; // 0x0 (the layout of the data is not known yet)
};
} // namespace res
} // namespace gfx
} // namespace nw
