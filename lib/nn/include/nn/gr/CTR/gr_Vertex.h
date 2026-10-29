#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_Types.h"

#include <string.h>

namespace nn {
namespace gr {
namespace CTR {
class BindSymbolVSInput;

namespace detail {
// vertex buffer base address (in units of 8 bytes) that array addresses are relative to (name is ours)
extern u32 s_VertexBufferBase;
} // namespace detail

// The vertex attribute setup: up to 12 attributes, each either read from an array or fixed.
// Member names are ours.
class Vertex
{
public:
    static const s32 ATTRIBUTE_COUNT = 12;

    // one vertex array (load array) with up to 12 interleaved components
    class LoadArray
    {
    public:
        LoadArray(); // 0x0034B0E8 | fefates:bytes [tier B]
        void DisableAll(); // 0x0034B0AC | mk7dlp:callgraph [confirmed by fefates] [tier A]

        u32 m_PhysicalAddr;                                 // 0x00, 0 = unused
        PicaDataVertexAttrType m_Types[ATTRIBUTE_COUNT];    // 0x04
        u32 m_ByteSizes[ATTRIBUTE_COUNT];                   // 0x10, 0 ends the array
        s32 m_AttrIndex[ATTRIBUTE_COUNT];                   // 0x40, -1 = padding
    };

    struct IndexStream
    {
        IndexStream(); // 0x0034AC1C (not in the symbols)

        u32 m_PhysicalAddr;     // 0x00
        u32 m_DrawVertexCount;  // 0x04
        bool m_IsUnsignedByte;  // 0x08, else 16 bit indices
    };

    // a fixed attribute (name is ours)
    struct ConstAttr
    {
        ConstAttr(); // 0x0034B0A0 (name is ours)

        u8 m_Dimension; // 0x00, 0 = not fixed
        f32 m_Value[4]; // 0x04
    };

    // the layout of an interleaved array (name is ours)
    struct InterleaveInfo
    {
        u8 m_Count;                                         // 0x00
        PicaDataVertexAttrType m_Types[ATTRIBUTE_COUNT];    // 0x04
        const BindSymbolVSInput* m_Symbols[ATTRIBUTE_COUNT]; // 0x10, NULL = padding
    };

    Vertex(); // 0x0034B110 (not in the symbols)

    void EnableAttrAsArray(const BindSymbolVSInput& symbol, u32 physicalAddr, PicaDataVertexAttrType type); // 0x0034AD70 | tier C
    void EnableAttrAsConst(const BindSymbolVSInput& symbol, u8 dimension, const f32* values); // 0x0034AEA0 | nintendogs:bytes [confirmed by fefates] [tier A]
    void EnableInterleavedArray(const InterleaveInfo& info, u32 physicalAddr); // 0x0034AF38 (name is ours)
    u32* MakeDrawCommand(u32* command, const IndexStream& stream) const; // 0x00727B4C | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    // the commands of the enabled attributes, from the cache (made the first time; inline, e.g. in
    // sead::ptcl::GpuPtcle::Draw; the name is ours)
    u32* MakeEnableAttrCommand(u32* command) const
    {
        if (m_CommandCacheSize == 0) {
            m_CommandCacheSize = MakeEnableAttrCommand_(m_CommandCache) - m_CommandCache;
        }
        memcpy(command, m_CommandCache, m_CommandCacheSize * sizeof(u32));
        return command + m_CommandCacheSize;
    }
    // no attribute enabled, like a new one (inline, e.g. in sead::ptcl::PtclRenderer::beginStripeCalc; the
    // name is ours)
    void Reset()
    {
        m_CommandCacheSize = 0;
        memset(m_CommandCache, 0, sizeof(m_CommandCache));
        for (s32 i = 0; i < ATTRIBUTE_COUNT; i++) {
            m_IsEnabled[i] = false;
            m_LoadArrays[i].DisableAll();
            m_ConstAttrs[i].m_Dimension = 0;
            m_ConstAttrs[i].m_Value[0] = 0.0f;
            m_ConstAttrs[i].m_Value[1] = 0.0f;
            m_ConstAttrs[i].m_Value[2] = 0.0f;
            m_ConstAttrs[i].m_Value[3] = 0.0f;
        }
    }
    u32* MakeEnableAttrCommand_(u32* command) const; // 0x00727BEC | mk7dlp:callgraph [confirmed by fefates] [tier A]
    DECOMP_NOINLINE void DisableAttr_(u32 index); // 0x0034AC30 | nintendogs:bytes [confirmed by fefates] [tier A]

    mutable u32 m_CommandCacheSize;             // 0x000, 0 when the cache must be rebuilt
    mutable u32 m_CommandCache[84];             // 0x004
    bool m_IsEnabled[ATTRIBUTE_COUNT];          // 0x154
    LoadArray m_LoadArrays[ATTRIBUTE_COUNT];    // 0x160
    ConstAttr m_ConstAttrs[ATTRIBUTE_COUNT];    // 0x6A0
};
ASSERT_SIZE(Vertex::LoadArray, 0x70);
ASSERT_SIZE(Vertex::IndexStream, 0xC);
ASSERT_SIZE(Vertex::ConstAttr, 0x14);
ASSERT_SIZE(Vertex::InterleaveInfo, 0x40);
ASSERT_SIZE(Vertex, 0x790);
} // namespace CTR
} // namespace gr
} // namespace nn
