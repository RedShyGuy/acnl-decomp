#pragma once

#include "decomp.h"
#include "imgdb/Singleton.h"

namespace imgdb {
// RTTI N5imgdb13ImageDatabaseE @ 0x008D2AF0
// vtable 0x00908F0C (vptr 0x00908F14), offset_to_top 0, 2 entries
class ImageDatabase : public ::imgdb::Singleton<imgdb::ImageDatabase>
{
public:
    virtual void vf_0x00(); // 0x005A72B8 slot 0x00 | virtual slot, introduced by imgdb::Singleton<imgdb::ImageDatabase>
    virtual void vf_0x04(); // 0x005A7218 slot 0x04 | virtual slot, introduced by imgdb::Singleton<imgdb::ImageDatabase>
    void Initialize(bool, bool); // 0x005A5C30 | nintendogs:bytes [tier B]
    void RegisterMp(imgdb::StorageType, const void*, unsigned, const imgdb::ImageDbRecordParam&, imgdb::ImageDbRecordLink*); // 0x005A5CDC | nintendogs:bytes [tier B]
    void ResetTable(imgdb::StorageType); // 0x005A5D28 | nintendogs:bytes [tier B]
    void RegisterJpeg(imgdb::StorageType, const void*, unsigned, const imgdb::ImageDbRecordParam&, imgdb::ImageDbRecordLink*); // 0x005A60B4 | nintendogs:bytes [tier B]
    void SaveDatabase(imgdb::StorageType); // 0x005A6100 | nintendogs:bytes [tier B]
    void RegisterImage(imgdb::StorageType, imgdb::ImageKind, const void*, unsigned, const void*, unsigned, const imgdb::ImageDbRecordParam&, bool, const imgdb::IndexInfo*, imgdb::ImageDbRecordLink*); // 0x005A6154 | nintendogs:bytes [tier B]
    void CreateDatabase(imgdb::StorageType); // 0x005A63B4 | nintendogs:bytes [tier B]
    void RestoreDatabase(imgdb::StorageType); // 0x005A6424 | nintendogs:bytes [tier B]
    void UnregisterImage(const imgdb::ImageDbRecordLink&); // 0x005A6664 | nintendogs:bytes [tier B]
    void CreateRecordLink(imgdb::ImageDbRecordLink&, imgdb::StorageType, const imgdb::DateTimeSeconds&, imgdb::ImageKind, const imgdb::ImageDbRecordParam&, long long); // 0x005A6754 | nintendogs:bytes [tier B]
    void UnregisterRecord(imgdb::StorageType, int, imgdb::ImageDbRecord&); // 0x005A6870 | nintendogs:bytes [tier B]
    void SyncPictureDatabaseCore(imgdb::StorageType); // 0x005A6984 | nintendogs:bytes [tier B]
    void SyncPictureDatabaseWithTwl(imgdb::StorageType); // 0x005A6D00 | nintendogs:bytes [tier B]
    void SortPictureDatabaseCtrAndTwl(imgdb::StorageType); // 0x005A6E38 | nintendogs:bytes [tier B]
    ImageDatabase(); // 0x005A71BC | nintendogs:bytes-fuzzy [tier B]
    void WritePictureDatabase(imgdb::StorageType, bool, bool); // 0x005AA904 | nintendogs:bytes [tier B]
    void FindRecord(const imgdb::ImageDbRecordLink&) const; // 0x007550BC | nintendogs:bytes [tier B]
    void GetEmptyImageNum(imgdb::StorageType) const; // 0x0075511C | nintendogs:bytes [tier B]
    void InsertRecordLinkList(imgdb::DynamicArray<imgdb::ImageDbRecordLink, imgdb::XAllocator<imgdb::ImageDbRecordLink>>&, const imgdb::ImageDbRecordLink&) const; // 0x00755148 | nintendogs:bytes [tier B]
    void FindRecordLinkListInsertPos(const imgdb::DynamicArray<imgdb::ImageDbRecordLink, imgdb::XAllocator<imgdb::ImageDbRecordLink>>&, const imgdb::ImageDbRecordLink&) const; // 0x00755204 | nintendogs:bytes-fuzzy [tier B]
};
} // namespace imgdb
