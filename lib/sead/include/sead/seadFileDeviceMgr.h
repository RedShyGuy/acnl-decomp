#pragma once

#include "decomp.h"
#include "sead/seadFileDevice.h"
#include "sead/seadTListNode.h"

namespace sead {
class Heap;
class MainFileDevice;

// The mounted file devices by drive name ("<drive>:<path>"; paths without a drive go to the default
// device). A singleton without a vtable whose SingletonDisposer_ lies in its first 16 bytes; the
// constructor mounts the rom of the program and the MainFileDevice. unmount is the name of the
// reference symbols, the other names are ours.
class FileDeviceMgr
{
public:
    class SingletonDisposer_;

    FileDeviceMgr(); // (inline in createInstance)
    ~FileDeviceMgr(); // (inline in deleteInstance and ~SingletonDisposer_)

    static FileDeviceMgr* createInstance(Heap* heap); // 0x0012C540
    static void deleteInstance(); // 0x0012C658

    // adds device under its drive name (driveName: a new one, empty: the one it has)
    void mount(FileDevice* device, const SafeStringBase<char>& driveName = SafeStringBase<char>::s_EmptyString); // 0x00120780
    void unmount(FileDevice* device); // 0x00542E80 | nintendogs:bytes [tier A]
    // the device of a drive name, NULL if none is mounted under it
    FileDevice* findDevice(const SafeStringBase<char>& driveName) const; // 0x00139D80
    // the device of path (and path without the drive)
    FileDevice* findDeviceFromPath(const SafeStringBase<char>& path, BufferedSafeStringBase<char>* noDrivePath) const; // 0x00134C3C
    // loads path ("<drive>:<path>") with its device
    u8* tryLoad(FileDevice::LoadArg& arg); // 0x0012C6D8
    // opens path ("<drive>:<path>") with its device
    FileDevice* tryOpen(FileHandle* handle, const SafeStringBase<char>& path, FileDevice::FileOpenFlag flag,
                        u32 divSize); // 0x00542E20

    // the text before ":" of path; false if there is no ":"
    static bool getDriveName(BufferedSafeStringBase<char>* driveName, const SafeStringBase<char>& path); // 0x00138DE8
    // the text after "://" of path (all of it without "://")
    static void getPathWithoutDrive(BufferedSafeStringBase<char>* noDrivePath, const SafeStringBase<char>& path); // 0x00138EA0

    static FileDeviceMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 mSingletonDisposerBuffer[4];  // 0x00, the SingletonDisposer_
    TList<FileDevice*> mDeviceList;   // 0x10
    FileDevice* mDefaultDevice;       // 0x1C
    MainFileDevice* mMainFileDevice;  // 0x20
    u8* mRomWorkBuffer;               // 0x24, the buffer of nn::fs::MountRom
};
ASSERT_SIZE(FileDeviceMgr, 0x28);
} // namespace sead
