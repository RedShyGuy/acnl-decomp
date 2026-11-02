#pragma once

#include "decomp.h"
#include "ssys/ma/dLoadSplit.h"
#include "ssys/ma/lyt/dArcResAcc.h"

namespace sead {
class Heap;
} // namespace sead

namespace ssys {
namespace ma {
namespace lyt {
// an ArcResAcc that loads its archive with a LoadSplit (async: Read*, at once: Load*; names after libgarden except
// the overloads, LoadArc and ShareArc)
// RTTI N4ssys2ma3lyt15ArcResAccReaderE @ 0x008D2470
// vtable 0x009072DC (vptr 0x009072E4), offset_to_top 0, 2 entries
class ArcResAccReader : public ::ssys::ma::lyt::ArcResAcc
{
public:
    DECOMP_EMIT_COPY ArcResAccReader() : m_pArc(NULL) {} // 0x00120D68 | libgarden [tier A]
    virtual ~ArcResAccReader(); // 0x005689D8 slot 0x00 | libgarden

    // async; true when the archive is there (on the device heap; defaults 0x80 and 512 KB)
    bool ReadArc(const char* path); // 0x0056890C | libgarden [tier A]
    bool ReadArc(const char* path, u32 alignment, u32 chunkSize); // 0x0056891C
    bool ReadArc(const char* path, sead::Heap* heap); // 0x005687B0
    bool ReadArc(const char* path, sead::Heap* heap, u32 alignment, u32 chunkSize); // 0x005687CC
    // at once
    bool LoadArc(const char* path); // 0x00568884
    bool LoadArc(const char* path, u32 alignment); // 0x00568894
    // uses the archive other has loaded
    bool ShareArc(const ArcResAccReader* other); // 0x00568848
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Clear(); // 0x00133A20 | libgarden [tier A]

    // (inline)
    bool SetArc(const void* data)
    {
        m_pArc = data;
        return SetData(data, ".");
    }
    const void* GetArc() const { return m_pArc; }

    static bool s_UseFlushCache;      // (name is ours) the images in device memory go to FlushCache
    static u32 s_DefaultAlignment;    // (name is ours)
    static u32 s_DefaultChunkSize;    // (name is ours)

    const void* m_pArc;       // 0x158
    LoadSplit m_LoadSplit;    // 0x15C
};
ASSERT_SIZE(ArcResAccReader, 0x248);
} // namespace lyt
} // namespace ma
} // namespace ssys
