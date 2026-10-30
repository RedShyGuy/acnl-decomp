#pragma once

#include "imgdb/imgdb_Types.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {

// RTTI N5imgdb14ArchiveMounterE
// an archive the images are on (slot names are ours)
class ArchiveMounter
{
public:
    virtual bool Mount() = 0;               // slot 0x00
    virtual void Unmount() = 0;             // slot 0x04
    virtual bool IsMount() const = 0;       // slot 0x08
    virtual s64 GetFreeSize() const = 0;    // slot 0x0C
    virtual ~ArchiveMounter() {}            // slots 0x10, 0x14
};

// the mount names (char) of the three archives (0x0097F0C4; names are ours)
extern const char* s_MountNames[3];

// the SD card ("$phtsd:")
// RTTI N5imgdb16SdArchiveMounterE
// vtables 0x00908F70 (vptr 0x00908F78) and 0x00908F90 (ArchiveMounter at +4)
class SdArchiveMounter : public Singleton<SdArchiveMounter>,
                         public ArchiveMounter,
                         private nn::util::ADLFireWall::NonCopyable<SdArchiveMounter>
{
public:
    SdArchiveMounter();
    virtual ~SdArchiveMounter();

    virtual bool Mount();
    virtual void Unmount();
    virtual bool IsMount() const;
    virtual s64 GetFreeSize() const;

    static bool IsInsert();
    static bool IsWritable();

private:
    bool m_IsMounted;   // +0x08
    bool m_IsInserted;  // +0x09
};
ASSERT_SIZE(SdArchiveMounter, 0xC);

// the shared extra save data of the images on the NAND ("$_phtsh:")
// RTTI N5imgdb21CtrNandArchiveMounterE
// vtables 0x00908FD8 (vptr 0x00908FE0) and 0x00908FF8 (ArchiveMounter at +4)
class CtrNandArchiveMounter : public Singleton<CtrNandArchiveMounter>,
                              public ArchiveMounter,
                              private nn::util::ADLFireWall::NonCopyable<CtrNandArchiveMounter>
{
public:
    static const u32 SHARED_EXT_SAVE_DATA_ID = 0xF0000001;

    CtrNandArchiveMounter();
    virtual ~CtrNandArchiveMounter();

    virtual bool Mount();
    virtual void Unmount();
    virtual bool IsMount() const;
    virtual s64 GetFreeSize() const;

    bool GetSharedExtSaveDataBlockSize(s64& totalBlocks, s64& freeBlocks, int& blockSize) const;

private:
    bool m_IsMounted;  // +0x08
};
ASSERT_SIZE(CtrNandArchiveMounter, 0xC);

// the photo archive of the DSi ("twlp:")
// RTTI N5imgdb21TwlNandArchiveMounterE
// vtables 0x00909030 (vptr 0x00909038) and 0x00909050 (ArchiveMounter at +4)
class TwlNandArchiveMounter : public Singleton<TwlNandArchiveMounter>,
                              public ArchiveMounter,
                              private nn::util::ADLFireWall::NonCopyable<TwlNandArchiveMounter>
{
public:
    static const u32 TWL_PHOTO_ARCHIVE_ID = 0x567890AC;

    TwlNandArchiveMounter();
    virtual ~TwlNandArchiveMounter();

    virtual bool Mount();
    virtual void Unmount();
    virtual bool IsMount() const;
    virtual s64 GetFreeSize() const;

private:
    bool m_IsMounted;  // +0x08
};
ASSERT_SIZE(TwlNandArchiveMounter, 0xC);

} // namespace imgdb
