#include "sead/seadFileDeviceMgr.h"
#include "nn/fs/fs_Api.h"
#include "sead/seadFileDeviceMgr_SingletonDisposer_.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadMainFileDevice.h"
#include "sead/seadNew.h"

#include <new>

namespace sead {
// 0x00976300
FileDeviceMgr* FileDeviceMgr::s_pInstance = nullptr;
// 0x00976304
FileDeviceMgr::SingletonDisposer_* FileDeviceMgr::s_pSingletonDisposer = nullptr;

sead::FileDeviceMgr::FileDeviceMgr() : mDeviceList(), mDefaultDevice(nullptr), mMainFileDevice(nullptr)
{
    if (HeapMgr::s_pInstance == nullptr) {
        return;
    }
    Heap* heap = HeapMgr::s_pInstance->findContainHeap(this);
    nn::fs::Initialize();
    s32 size = nn::fs::GetRomRequiredMemorySize(16, 16, true);
    mRomWorkBuffer = new (heap, 4) u8[size];
    nn::fs::MountRom(16, 16, mRomWorkBuffer, size, true);
    mMainFileDevice = new (heap, 4) MainFileDevice(heap);
    mount(mMainFileDevice);
    mDefaultDevice = mMainFileDevice;
}

sead::FileDeviceMgr::~FileDeviceMgr()
{
    if (mMainFileDevice != nullptr) {
        delete mMainFileDevice;
        mMainFileDevice = nullptr;
    }
    nn::fs::Unmount("rom");
    delete[] mRomWorkBuffer;
}

// 0x0012C540 (name is ours)
sead::FileDeviceMgr* sead::FileDeviceMgr::createInstance(Heap* heap)
{
    if (s_pInstance == nullptr) {
        u8* buffer = new (heap, 4) u8[sizeof(FileDeviceMgr)];
        FileDeviceMgr* instance = nullptr;
        if (buffer == nullptr) {
            s_pSingletonDisposer = nullptr;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) FileDeviceMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x0012C658 (name is ours)
void sead::FileDeviceMgr::deleteInstance()
{
    SingletonDisposer_* disposer = s_pSingletonDisposer;
    if (disposer == nullptr) {
        return;
    }
    s_pSingletonDisposer = nullptr;
    disposer->~SingletonDisposer_();
    if (s_pInstance != nullptr) {
        delete s_pInstance;
    }
    s_pInstance = nullptr;
}

// 0x00120780 (name is ours)
void sead::FileDeviceMgr::mount(FileDevice* device, const SafeStringBase<char>& driveName)
{
    if (!driveName.isEqual(SafeStringBase<char>::s_EmptyString)) {
        device->mDriveName.copy(driveName);
    }
    mDeviceList.pushBack(device);
}

// 0x00542E80 | nintendogs:bytes [tier A]
void sead::FileDeviceMgr::unmount(FileDevice* device)
{
    TListNode<FileDevice*>* node = device;
    if (node->mList != nullptr) {
        node->mList = nullptr;
        node->erase_();
        mDeviceList.mCount--;
    }
    if (mDefaultDevice == device) {
        mDefaultDevice = nullptr;
    }
}

// 0x00139D80 (name is ours)
sead::FileDevice* sead::FileDeviceMgr::findDevice(const SafeStringBase<char>& driveName) const
{
    for (ListNode* node = mDeviceList.mStartEnd.mNext; node != &mDeviceList.mStartEnd; node = node->mNext) {
        FileDevice* device = static_cast<TListNode<FileDevice*>*>(node)->mData;
        if (device->mDriveName.isEqual(driveName)) {
            return device;
        }
    }
    return nullptr;
}

// 0x00134C3C (name is ours)
sead::FileDevice* sead::FileDeviceMgr::findDeviceFromPath(const SafeStringBase<char>& path,
                                                          BufferedSafeStringBase<char>* noDrivePath) const
{
    FixedSafeString<32> driveName;
    FileDevice* device;
    if (getDriveName(&driveName, path)) {
        device = findDevice(driveName);
    } else {
        device = mDefaultDevice;
        if (device == nullptr) {
            return nullptr;
        }
    }
    if (noDrivePath != nullptr) {
        getPathWithoutDrive(noDrivePath, path);
    }
    return device;
}

// 0x0012C6D8 (name is ours)
u8* sead::FileDeviceMgr::tryLoad(FileDevice::LoadArg& arg)
{
    FixedSafeString<256> noDrivePath;
    FileDevice* device = findDeviceFromPath(arg.path, &noDrivePath);
    if (device == nullptr) {
        return nullptr;
    }
    FileDevice::LoadArg deviceArg(arg);
    deviceArg.path = SafeStringBase<char>(noDrivePath.getCString());
    u8* result = device->tryLoad(deviceArg);
    arg.readSize = deviceArg.readSize;
    arg.roundupBufferSize = deviceArg.roundupBufferSize;
    arg.needUnload = deviceArg.needUnload;
    return result;
}

// 0x00542E20 (name is ours)
sead::FileDevice* sead::FileDeviceMgr::tryOpen(FileHandle* handle, const SafeStringBase<char>& path,
                                               FileDevice::FileOpenFlag flag, u32 divSize)
{
    FixedSafeString<256> noDrivePath;
    FileDevice* device = findDeviceFromPath(path, &noDrivePath);
    if (device == nullptr) {
        return nullptr;
    }
    return device->tryOpen(handle, noDrivePath, flag, divSize);
}

// 0x00138DE8 (name is ours)
bool sead::FileDeviceMgr::getDriveName(BufferedSafeStringBase<char>* driveName, const SafeStringBase<char>& path)
{
    driveName->trim(0);
    s32 index = path.findIndex(SafeStringBase<char>(":"));
    if (index == -1) {
        return false;
    }
    driveName->copy(path, index);
    return true;
}

// 0x00138EA0 (name is ours)
void sead::FileDeviceMgr::getPathWithoutDrive(BufferedSafeStringBase<char>* noDrivePath, const SafeStringBase<char>& path)
{
    noDrivePath->trim(0);
    s32 index = path.findIndex(SafeStringBase<char>("://"));
    if (index == -1) {
        noDrivePath->overwriteAt(0, path, -1);
    } else {
        noDrivePath->overwriteAt(0, path.getPart(index + 3), -1);
    }
}

} // namespace sead
