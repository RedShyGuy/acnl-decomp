#pragma once

#include "decomp.h"
#include "sead/seadCriticalSection.h"

#include <cstring>

namespace sead {
class Heap;
} // namespace sead

namespace ssys {
namespace ma {
// collects the parts of device memory the CPU wrote and flushes them from the data cache before the GPU reads them:
// one range around all writes, or (m_UseRangeList) a sorted list of ranges (a sead singleton without a vtable; all
// names except the class and SingletonDisposer_ are ours)
class FlushCache
{
public:
    class SingletonDisposer_;

    struct Range
    {
        uptr start;
        uptr end;
    };

    // up to 32 ranges sorted by address; a range that overlaps others is joined with them
    class RangeList
    {
    public:
        static const u32 MAX_RANGES = 32;

        // (inline in the static initializer)
        RangeList() : m_Count(0), m_IsDebug(false) { Clear(); }

        void Clear()
        {
            m_Count = 0;
            std::memset(m_Ranges, 0, sizeof(m_Ranges));
        }

        // the index of the first range that range overlaps or that lies behind it
        DECOMP_EMIT_COPY DECOMP_NOINLINE u32 FindIndex(const Range& range) const // 0x0013D7C0
        {
            if (m_Count == 0) {
                return 0;
            }
            u32 low = 0;
            u32 high = m_Count - 1;
            for (;;) {
                u32 mid = (low + high) >> 1;
                if (range.end < m_Ranges[mid].start) {
                    if (mid == low) {
                        return mid;
                    }
                    high = mid - 1;
                } else {
                    if (m_Ranges[mid].end >= range.start) {
                        return mid;
                    }
                    if (mid == high) {
                        return mid + 1;
                    }
                    low = mid + 1;
                }
            }
        }
        // the index of the range that contains address, or of the first range behind it
        DECOMP_EMIT_COPY DECOMP_NOINLINE u32 FindStartIndex(const Range& range) const // 0x0013D6D0
        {
            if (m_Count == 0) {
                return 0;
            }
            uptr address = range.start;
            u32 low = 0;
            u32 high = m_Count - 1;
            for (;;) {
                u32 mid = (low + high) >> 1;
                if (address < m_Ranges[mid].start) {
                    if (mid == low) {
                        return mid;
                    }
                    high = mid - 1;
                } else {
                    if (m_Ranges[mid].end >= address) {
                        return mid;
                    }
                    if (mid == high) {
                        return mid + 1;
                    }
                    low = mid + 1;
                }
            }
        }
        // the index of the range that contains the end of range, or of the last range in front of it
        DECOMP_EMIT_COPY DECOMP_NOINLINE u32 FindEndIndex(const Range& range) const // 0x0013D740
        {
            u32 count = m_Count;
            if (count == 0) {
                return 0;
            }
            uptr address = range.end;
            u32 low = 0;
            u32 high = count - 1;
            for (;;) {
                u32 mid = (low + high) >> 1;
                if (address < m_Ranges[mid].start) {
                    if (mid == low) {
                        return mid == 0 ? count : mid - 1;
                    }
                    high = mid - 1;
                } else {
                    if (address <= m_Ranges[mid].end) {
                        return mid;
                    }
                    if (mid == high) {
                        return mid;
                    }
                    low = mid + 1;
                }
            }
        }
        DECOMP_EMIT_COPY DECOMP_NOINLINE bool Intersects(const Range& range) const // 0x00748D28
        {
            const Range& found = m_Ranges[FindIndex(range)];
            bool result = found.end >= range.start && found.start <= range.end;
            if (m_IsDebug) {
                Dump();
            }
            return result;
        }
        DECOMP_EMIT_COPY void Add(const Range& range); // 0x00138890
        // (empty in the release build)
        DECOMP_EMIT_COPY DECOMP_NOINLINE DECOMP_NOIPA void Dump() const {} // 0x0013D7BC

        u32 m_Count;                  // 0x00
        bool m_IsDebug;               // 0x04
        Range m_Ranges[MAX_RANGES];   // 0x08
    };

    static FlushCache* createInstance(sead::Heap* heap); // 0x00567138
    static void deleteInstance(); // 0x0012D1BC

    // (inline in createInstance)
    FlushCache()
        : m_DirtyStart(0), m_DirtyEnd(0), m_FlushCount(0), m_AddCount(0), m_FlushRangeCount(0), _24(0), _28(0),
          m_UseRangeList(false)
    {
    }

    // notes that the CPU wrote [address, address + size)
    void AddRange(uptr address, u32 size); // 0x001338F4
    // flushes the written parts if they overlap [address, address + size)
    void FlushRange(uptr address, u32 size); // 0x00567010
    void FlushAll(); // 0x00567264

    DECOMP_EMIT_COPY static void Add(uptr address, u32 size) // 0x0012D2A8
    {
        if (s_pInstance != NULL) {
            s_pInstance->AddRange(address, size);
        }
    }
    // without an instance all of device memory is flushed
    static void Flush(uptr address, u32 size); // 0x00567F08

    static FlushCache* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;
    static RangeList s_RangeList;
    static sead::CriticalSection s_CriticalSection;

    u32 m_SingletonDisposerBuffer[4]; // 0x00, the SingletonDisposer_
    uptr m_DirtyStart;                // 0x10
    uptr m_DirtyEnd;                  // 0x14
    u32 m_FlushCount;                 // 0x18
    u32 m_AddCount;                   // 0x1C
    u32 m_FlushRangeCount;            // 0x20
    u32 _24;                          // 0x24
    u32 _28;                          // 0x28
    bool m_UseRangeList;              // 0x2C
    sead::CriticalSection m_CriticalSection; // 0x30
};
ASSERT_SIZE(FlushCache, 0x4C);
} // namespace ma
} // namespace ssys
