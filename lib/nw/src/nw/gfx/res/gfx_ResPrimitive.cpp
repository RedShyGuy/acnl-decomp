#include "nn/gx/detail/gx_Context.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/nstd/nstd_String.h"
#include "nw/gfx/internal/internal_Api.h"
#include "nw/gfx/res/gfx_ResPrimitive.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/os/os_IAllocator.h"

namespace nw {
namespace gfx {
namespace res {
// 0x004A67E8 | nintendogs:bytes [tier A]
void nw::gfx::res::ResPrimitive::SetupDrawCommand(bool isFixedPrimitive)
{
    const ut::Offset* pBegin = ptr()->indexStreams.array.To<ut::Offset>();
    const ut::Offset* pEnd = pBegin + ptr()->indexStreams.count;
    for (const ut::Offset* pIt = pBegin; pIt != pEnd; ++pIt) {
        ResIndexStreamData* pStream = const_cast<ResIndexStreamData*>(pIt->To<ResIndexStreamData>());
        internal::CommandBufferInfo info(pStream->pCommand, pStream->commandSize);
        internal::SetupDrawIndexStreamCommand(info, ResIndexStream(pStream), isFixedPrimitive);
    }
    ptr()->flags |= ResPrimitiveData::FLAG_DRAW_COMMAND_DONE;
}

// 0x004A6880 | nintendogs:bytes [tier A]
void nw::gfx::res::ResPrimitive::Setup(os::IAllocator* pAllocator)
{
    ptr()->pAllocator = pAllocator;
    const u32 count = ptr()->indexStreams.count;
    const void** pBufferObjects = ptr()->bufferObjects.To<const void*>();
    for (u32 i = 0; i < count; ++i) {
        ResIndexStreamData* pStream = static_cast<ResIndexStreamData*>(ptr()->indexStreams.GetData(i));
        pBufferObjects[i] = pStream;
        pStream->pCommand = static_cast<u32*>(pAllocator->Alloc(DRAW_COMMAND_SIZE, 4));
        pStream->commandSize = DRAW_COMMAND_SIZE;

        // the indices uploaded once (a stream that already has its copy keeps it)
        if (pStream->pLocationAddress != nullptr) {
            pStream->locationArea = 0;
            continue;
        }
        u32 location = pStream->locationFlag;
        if (location == 0) {
            location = ResTexture::LOCATION_NO_COPY;
        }
        const u32 size = pStream->size;
        const u32 area = location & 0xFFFF0000;
        switch (area) {
        case ResTexture::LOCATION_NO_COPY:
            nngxlowStoreDataCache(reinterpret_cast<unsigned>(pStream->data.ToPtr()), size);
            break;
        case ResTexture::LOCATION_VRAM_A:
        case ResTexture::LOCATION_VRAM_B: {
            const u32 vramArea = area & 0x30000;
            void* pCopy = nn::gx::detail::s_Allocator.m_pAllocate(vramArea, PURPOSE_VERTEX_BUFFER,
                                                                  reinterpret_cast<GLuint>(pBufferObjects[i]), size);
            g_pAddVramDmaCommand(pStream->data.ToPtr(), pCopy, size);
            pStream->pLocationAddress = pCopy;
            pStream->locationArea = vramArea;
            break;
        }
        case ResTexture::LOCATION_FCRAM_COPY: {
            void* pCopy = nn::gx::detail::s_Allocator.m_pAllocate(0x10000, PURPOSE_VERTEX_BUFFER,
                                                                  reinterpret_cast<GLuint>(pBufferObjects[i]), size);
            nnnstdMemCpy(pCopy, pStream->data.ToPtr(), size);
            nngxlowStoreDataCache(reinterpret_cast<unsigned>(pCopy), size);
            pStream->pLocationAddress = pCopy;
            pStream->locationArea = 0x10000;
            break;
        }
        default:
            break;
        }
    }
}

} // namespace res
} // namespace gfx
} // namespace nw
