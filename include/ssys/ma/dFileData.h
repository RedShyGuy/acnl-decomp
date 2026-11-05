#pragma once

#include "decomp.h"
#include "sead/seadFileDeviceMgr.h"
#include "sead/seadHeap.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadSafeStringBase.h"

namespace ssys {
namespace ma {
class Allocator;

// a whole file loaded with sead::FileDeviceMgr::tryLoad into a buffer of a heap (no RTTI in the binary: class, function
// and member names are ours)
class FileData
{
public:
    DECOMP_EMIT_COPY DECOMP_NOINLINE FileData() : m_pHeap(NULL), m_pData(NULL), m_Size(0), m_IsLoaded(false) {} // 0x0012D3A0
    DECOMP_EMIT_COPY DECOMP_NOIPA ~FileData() {} // 0x0013C988
    // forgets the buffer (does not free it)
    void Clear(); // 0x0056AF74

    // false if it fails or another file is loaded
    bool Load(const char* path, sead::Heap* heap, s32 alignment); // 0x00126250
    bool Load(const char* path, Allocator* allocator, s32 alignment); // 0x0056B01C
    // (a copy of an inline function outside the code of this file in the original)
    bool Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, s32 alignment);
    // frees the buffer
    DECOMP_EMIT_COPY void Unload() // 0x00139490
    {
        if (m_IsLoaded) {
            if (m_pHeap != NULL) {
                sead::ScopedCurrentHeapSetter setter(m_pHeap);
                sead::FileDeviceMgr::s_pInstance->unload(m_pData);
            }
        } else if (m_pHeap != NULL && m_Size != 0) {
            m_pHeap->free(m_pData);
        }
        m_pHeap = NULL;
        m_pData = NULL;
        m_Size = 0;
        m_IsLoaded = false;
    }

    sead::Heap* m_pHeap;  // 0x0
    u8* m_pData;          // 0x4
    u32 m_Size;           // 0x8
    bool m_IsLoaded;      // 0xC the buffer is from tryLoad (else it is freed with the heap)
};
ASSERT_SIZE(FileData, 0x10);
} // namespace ma
} // namespace ssys
