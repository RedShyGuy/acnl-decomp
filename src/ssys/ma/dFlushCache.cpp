#include "ssys/ma/dFlushCache.h"
#include "ssys/ma/dFlushCache_SingletonDisposer_.h"
#include "nn/gx/gx_Api.h"
#include "nn/os/os_Api.h"
#include "sead/seadNew.h"

#include <new>

namespace ssys {
namespace ma {
// 0x00976C28
FlushCache* FlushCache::s_pInstance = NULL;
// 0x00976C2C
FlushCache::SingletonDisposer_* FlushCache::s_pSingletonDisposer = NULL;
// 0x00AE6690
FlushCache::RangeList FlushCache::s_RangeList;
// (constructed and destroyed, but no function of the release build uses it)
// 0x00AE6798
sead::CriticalSection FlushCache::s_CriticalSection;

// the range list and the critical section
// 0x00793040 (static initializer)

// 0x00138890 (name is ours)
void ssys::ma::FlushCache::RangeList::Add(const Range& range)
{
    u32 index = FindIndex(range);
    if (m_Count == index) {
        if (m_Count < MAX_RANGES) {
            m_Count++;
            m_Ranges[index] = range;
        } else {
            Range& last = m_Ranges[index - 1];
            last.start = last.start > range.start ? range.start : last.start;
            last.end = last.end > range.end ? last.end : range.end;
        }
    } else {
        Range& found = m_Ranges[index];
        if (range.end < found.start) {
            if (m_Count < MAX_RANGES) {
                m_Count++;
                for (u32 i = m_Count - 1; index < i; i--) {
                    m_Ranges[i] = m_Ranges[i - 1];
                }
                found = range;
            } else {
                found.start = range.start < found.start ? range.start : found.start;
                found.end = found.end > range.end ? found.end : range.end;
            }
        } else {
            u32 first = FindStartIndex(range);
            u32 last = FindEndIndex(range);
            if (m_Count == first) {
                first = FindStartIndex(range);
            }
            if (m_Count == last) {
                last = FindEndIndex(range);
            }
            Range& joined = m_Ranges[first];
            joined.start = joined.start > range.start ? range.start : joined.start;
            joined.end = joined.end > range.end ? joined.end : range.end;
            if (first < last) {
                Range& lastRange = m_Ranges[last];
                joined.start = joined.start > lastRange.start ? lastRange.start : joined.start;
                joined.end = joined.end > lastRange.end ? joined.end : lastRange.end;
                u32 removed = last - first;
                for (u32 i = last; i < m_Count; i++) {
                    m_Ranges[i - removed] = m_Ranges[i];
                    m_Ranges[i].start = 0;
                    m_Ranges[i].end = 0;
                }
                m_Count -= removed;
            }
        }
    }
    if (m_IsDebug) {
        Dump();
    }
}

// 0x0012D1BC (name after sead::FileDeviceMgr)
void ssys::ma::FlushCache::deleteInstance()
{
    SingletonDisposer_* disposer = s_pSingletonDisposer;
    if (disposer == NULL) {
        return;
    }
    s_pSingletonDisposer = NULL;
    disposer->~SingletonDisposer_();
    if (s_pInstance != NULL) {
        delete s_pInstance;
    }
    s_pInstance = NULL;
}

// 0x001338F4 (name is ours)
void ssys::ma::FlushCache::AddRange(uptr address, u32 size)
{
    m_CriticalSection.lock();
    m_AddCount++;
    uptr memoryStart = nn::os::GetDeviceMemoryAddress();
    uptr memoryEnd = memoryStart + nn::os::GetDeviceMemorySize();
    uptr end = address + size;
    if (address < memoryEnd && memoryStart < end) {
        if (address < memoryStart) {
            address = memoryStart;
        }
        if (end > memoryEnd) {
            end = memoryEnd;
        }
        if (m_UseRangeList) {
            Range range = {address, end};
            s_RangeList.Add(range);
        } else {
            if (m_DirtyStart == 0 || m_DirtyStart > address) {
                m_DirtyStart = address;
            }
            if (m_DirtyEnd == 0 || end > m_DirtyEnd) {
                m_DirtyEnd = end;
            }
        }
    }
    m_CriticalSection.unlock();
}

// 0x00567010 (name is ours)
void ssys::ma::FlushCache::FlushRange(uptr address, u32 size)
{
    m_CriticalSection.lock();
    m_FlushRangeCount++;
    uptr memoryStart = nn::os::GetDeviceMemoryAddress();
    uptr memoryEnd = memoryStart + nn::os::GetDeviceMemorySize();
    uptr end = address + size;
    if (address < memoryEnd && memoryStart < end) {
        if (address < memoryStart) {
            address = memoryStart;
        }
        if (end > memoryEnd) {
            end = memoryEnd;
        }
        if (m_UseRangeList) {
            Range range = {address, end};
            if (s_RangeList.Intersects(range)) {
                uptr start = 0;
                uptr last = 0;
                if (s_RangeList.m_Count != 0) {
                    start = s_RangeList.m_Ranges[0].start;
                    last = s_RangeList.m_Ranges[s_RangeList.m_Count - 1].end;
                }
                nngxUpdateBuffer(reinterpret_cast<const void*>(start), last - start);
                m_FlushCount++;
                s_RangeList.Clear();
            }
        } else if (m_DirtyStart != 0 && m_DirtyEnd > address && end > m_DirtyStart) {
            nngxUpdateBuffer(reinterpret_cast<const void*>(m_DirtyStart), m_DirtyEnd - m_DirtyStart);
            m_FlushCount++;
            m_DirtyStart = 0;
            m_DirtyEnd = 0;
        }
    }
    m_CriticalSection.unlock();
}

// 0x00567138 (name after sead::FileDeviceMgr)
ssys::ma::FlushCache* ssys::ma::FlushCache::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(FlushCache)];
        FlushCache* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) FlushCache();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x00567264 (name is ours)
void ssys::ma::FlushCache::FlushAll()
{
    m_CriticalSection.lock();
    if (m_UseRangeList) {
        if (s_RangeList.m_Count != 0) {
            uptr start = s_RangeList.m_Ranges[0].start;
            uptr last = s_RangeList.m_Ranges[s_RangeList.m_Count - 1].end;
            nngxUpdateBuffer(reinterpret_cast<const void*>(start), last - start);
            m_FlushCount++;
            s_RangeList.Clear();
        }
    } else if (m_DirtyStart != 0) {
        nngxUpdateBuffer(reinterpret_cast<const void*>(m_DirtyStart), m_DirtyEnd - m_DirtyStart);
        m_DirtyStart = 0;
        m_DirtyEnd = 0;
        m_FlushCount++;
    }
    m_CriticalSection.unlock();
}

// 0x00567F08 (name is ours)
void ssys::ma::FlushCache::Flush(uptr address, u32 size)
{
    if (s_pInstance != NULL) {
        s_pInstance->FlushRange(address, size);
        return;
    }
    uptr memoryStart = nn::os::GetDeviceMemoryAddress();
    nngxUpdateBuffer(reinterpret_cast<const void*>(memoryStart), nn::os::GetDeviceMemorySize());
}

} // namespace ma
} // namespace ssys
