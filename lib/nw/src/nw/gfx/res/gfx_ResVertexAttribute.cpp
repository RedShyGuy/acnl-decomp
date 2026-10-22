#include "nn/gx/detail/gx_Context.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/nstd/nstd_String.h"
#include "nw/gfx/res/gfx_ResPrimitive.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/gfx/res/gfx_ResVertexAttribute.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
// 0x004A5990 (name is ours)
// the stream copied to the area of the location flag (or flushed from the cache if it stays in place)
DECOMP_NOINLINE void UploadVertexStream(const void* pOwner, ResVertexAttributeData* pAttribute, u32 location)
{
    const u32 size = pAttribute->size;
    const u32 area = location & 0xFFFF0000;
    switch (area) {
    case ResTexture::LOCATION_NO_COPY:
        nngxlowStoreDataCache(reinterpret_cast<unsigned>(pAttribute->data.ToPtr()), size);
        break;
    case ResTexture::LOCATION_FCRAM_COPY: {
        void* pCopy = nn::gx::detail::s_Allocator.m_pAllocate(0x10000, PURPOSE_VERTEX_BUFFER, reinterpret_cast<GLuint>(pOwner), size);
        nnnstdMemCpy(pCopy, pAttribute->data.ToPtr(), size);
        nngxlowStoreDataCache(reinterpret_cast<unsigned>(pCopy), size);
        pAttribute->pLocationAddress = pCopy;
        pAttribute->locationArea = 0x10000;
        break;
    }
    case ResTexture::LOCATION_VRAM_A:
    case ResTexture::LOCATION_VRAM_B: {
        const u32 vramArea = area & 0x30000;
        void* pCopy = nn::gx::detail::s_Allocator.m_pAllocate(vramArea, PURPOSE_VERTEX_BUFFER, reinterpret_cast<GLuint>(pOwner), size);
        g_pAddVramDmaCommand(pAttribute->data.ToPtr(), pCopy, size);
        pAttribute->pLocationAddress = pCopy;
        pAttribute->locationArea = vramArea;
        break;
    }
    default:
        break;
    }
}
} // namespace

// 0x004A7E20 | nintendogs:bytes [tier A]
void nw::gfx::res::ResVertexAttribute::Setup()
{
    ResVertexAttributeData* pData = ptr();
    if ((pData->flags & ResVertexAttributeData::FLAG_CONSTANT) || pData->pLoaded != nullptr) {
        return;
    }
    pData->pLoaded = pData;
    // a stream that already has its copy keeps it
    if (pData->pLocationAddress != nullptr) {
        pData->locationArea = 0;
        return;
    }
    u32 location = pData->locationFlag;
    if (location == 0) {
        location = ResTexture::LOCATION_NO_COPY;
    }
    UploadVertexStream(pData, pData, location);
}

// 0x004A7EC0 (name is ours)
void nw::gfx::res::ResVertexAttribute::Cleanup()
{
    ResVertexAttributeData* pData = ptr();
    if ((pData->flags & ResVertexAttributeData::FLAG_CONSTANT) || pData->pLoaded == nullptr) {
        return;
    }
    if (pData->locationArea != 0) {
        nn::gx::detail::s_Allocator.m_pFree(pData->locationArea, PURPOSE_VERTEX_BUFFER, reinterpret_cast<GLuint>(pData->pLoaded),
                                            pData->pLocationAddress);
        pData->pLocationAddress = nullptr;
    }
    pData->pLoaded = nullptr;
}

} // namespace res
} // namespace gfx
} // namespace nw
