#pragma once

#include "decomp.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadFileDevice.h"
#include "sead/seadFileHandle.h"
#include "sead/seadSafeStringBase.h"

namespace sead {
class FileDevice;
} // namespace sead

namespace ssys {
namespace ma {
// a file opened through sead::FileDeviceMgr; every access holds one critical section (no RTTI in the binary: class,
// function and member names are ours)
class File
{
public:
    DECOMP_EMIT_COPY DECOMP_NOINLINE File() : m_pDevice(NULL) {} // 0x00133A88
    DECOMP_EMIT_COPY DECOMP_NOINLINE ~File() {} // 0x0013C9D8

    // opens path for reading (false if a file is open already or it fails)
    bool Open(const sead::SafeStringBase<char>& path); // 0x0056B074
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool Close() // 0x0013C990
    {
        if (m_pDevice == NULL) {
            return false;
        }
        s_CriticalSection.lock();
        m_pDevice->tryClose(&m_Handle);
        m_pDevice = NULL;
        s_CriticalSection.unlock();
        return true;
    }
    u32 GetSize(); // 0x0056B024
    // the number of bytes read
    u32 Read(u8* buffer, u32 size); // 0x0056B0E4
    bool IsOpen() const { return m_pDevice != NULL; }

    static bool IsExist(const sead::SafeStringBase<char>& path); // 0x0056AF8C

    static sead::CriticalSection s_CriticalSection;

    sead::FileHandle m_Handle;     // 0x00
    sead::FileDevice* m_pDevice;   // 0x30
};
ASSERT_SIZE(File, 0x34);
} // namespace ma
} // namespace ssys
