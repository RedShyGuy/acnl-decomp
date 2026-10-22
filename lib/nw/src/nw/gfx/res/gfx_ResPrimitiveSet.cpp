#include <cstring>

#include "nn/gx/detail/gx_Context.h"
#include "nw/gfx/res/gfx_ResPrimitiveSet.h"
#include "nw/os/os_IAllocator.h"

namespace nw {
namespace gfx {
namespace res {
// 0x004A75CC | nintendogs:bytes [tier A]
void nw::gfx::res::ResPrimitiveSet::Cleanup()
{
    const ut::Offset* pBegin = ptr()->primitives.array.To<ut::Offset>();
    const ut::Offset* pEnd = pBegin + ptr()->primitives.count;
    for (const ut::Offset* pIt = pBegin; pIt != pEnd; ++pIt) {
        ResPrimitiveData* pPrimitive = const_cast<ResPrimitiveData*>(pIt->To<ResPrimitiveData>());
        const void** pBufferObjects = pPrimitive->bufferObjects.To<const void*>();
        const u32 count = pPrimitive->indexStreams.count;
        for (u32 i = 0; i < count; ++i) {
            ResIndexStreamData* pStream = static_cast<ResIndexStreamData*>(pPrimitive->indexStreams.GetData(i));
            if (pStream != nullptr && pStream->pCommand != nullptr) {
                pPrimitive->pAllocator->Free(pStream->pCommand);
                pStream->pCommand = nullptr;
                pStream->commandSize = 0;
            }
            if (pStream->locationArea != 0) {
                nn::gx::detail::s_Allocator.m_pFree(pStream->locationArea, PURPOSE_VERTEX_BUFFER,
                                                    reinterpret_cast<GLuint>(pBufferObjects[i]), pStream->pLocationAddress);
                pStream->pLocationAddress = nullptr;
            }
        }
        std::memset(pBufferObjects, 0, count * sizeof(const void*));
        pPrimitive->pAllocator = nullptr;
        pPrimitive->flags &= ~ResPrimitiveData::FLAG_DRAW_COMMAND_DONE;
    }
}

} // namespace res
} // namespace gfx
} // namespace nw
