#include "ssys/ma/dFile.h"
#include "sead/seadFileDeviceMgr.h"
#include "sead/seadFixedSafeString.h"

namespace ssys {
namespace ma {
// 0x00AE6848
sead::CriticalSection File::s_CriticalSection;

// the critical section
// 0x007857CC (static initializer)

// (two equal copies in the original)
// 0x0056AF8C (name is ours)
// 0x0056AEE4 (copy)
bool ssys::ma::File::IsExist(const sead::SafeStringBase<char>& path)
{
    sead::FixedSafeString<256> noDrivePath;
    bool exists = false;
    sead::FileDevice* device = sead::FileDeviceMgr::s_pInstance->findDeviceFromPath(path, &noDrivePath);
    if (device != NULL) {
        bool isExist = false;
        device->tryIsExistFile(&isExist, noDrivePath);
        exists = isExist;
    }
    return exists;
}

// 0x0056B024 (name is ours)
u32 ssys::ma::File::GetSize()
{
    if (m_pDevice == NULL) {
        return 0;
    }
    s_CriticalSection.lock();
    u32 size = 0;
    m_pDevice->tryGetFileSize(&size, &m_Handle);
    s_CriticalSection.unlock();
    return size;
}

// 0x0056B074 (name is ours)
bool ssys::ma::File::Open(const sead::SafeStringBase<char>& path)
{
    if (m_pDevice != NULL) {
        return false;
    }
    s_CriticalSection.lock();
    m_pDevice = sead::FileDeviceMgr::s_pInstance->tryOpen(&m_Handle, path, sead::FileDevice::FILE_OPEN_FLAG_READ_ONLY, 0);
    bool opened = m_pDevice != NULL;
    s_CriticalSection.unlock();
    return opened;
}

// 0x0056B0E4 (name is ours)
u32 ssys::ma::File::Read(u8* buffer, u32 size)
{
    if (m_pDevice == NULL) {
        return 0;
    }
    s_CriticalSection.lock();
    u32 readSize = 0;
    m_pDevice->tryRead(&readSize, &m_Handle, buffer, size);
    s_CriticalSection.unlock();
    return readSize;
}

} // namespace ma
} // namespace ssys
