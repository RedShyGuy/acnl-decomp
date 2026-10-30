#pragma once

#include "imgdb/ImageDbRecordLink.h"
#include "imgdb/PictureDatabase.h"
#include "imgdb/imgdb_Types.h"

namespace imgdb {

class ImageDbRecord;
class ImageDbRecordParam;

// the databases of the CTR NAND and the SD card and a list of all their images sorted by date (member names are
// ours)
// RTTI N5imgdb13ImageDatabaseE @ 0x008D2AF0
// vtable 0x00908F0C (vptr 0x00908F14), offset_to_top 0, 2 entries
class ImageDatabase : public Singleton<ImageDatabase>
{
public:
    // records per database
    static const int CTR_NAND_RECORD_NUM = 500;
    static const int SD_RECORD_NUM = 3000;

    ImageDatabase();
    virtual ~ImageDatabase();

    void Initialize(bool isMountNand, bool isMountSd);
    // the record-link list and the buffers of both databases are freed
    void Finalize();
    Result RegisterMp(StorageType storage, const void* data, u32 size, const ImageDbRecordParam& param,
                      ImageDbRecordLink* link);
    // the records of a storage are removed and its tables made new
    void ResetTable(StorageType storage);
    Result LoadDatabase(StorageType storage);
    Result RegisterJpeg(StorageType storage, const void* data, u32 size, const ImageDbRecordParam& param,
                        ImageDbRecordLink* link);
    Result SaveDatabase(StorageType storage);
    // a record for an image and the image file (with data); at the next free index or at the given one
    Result RegisterImage(StorageType storage, ImageKind kind, const void* data, u32 size, const void* subData,
                         u32 subSize, const ImageDbRecordParam& param, bool isSave, const IndexInfo* index,
                         ImageDbRecordLink* link);
    Result CreateDatabase(StorageType storage);
    // the tables are read again; tables that cannot be read are rebuilt from the image files
    Result RestoreDatabase(StorageType storage);
    // the image file and its record
    Result UnregisterImage(const ImageDbRecordLink& link);
    Result CreateRecordLink(ImageDbRecordLink& link, StorageType storage, const DateTimeSeconds& dateTime,
                            ImageKind kind, const ImageDbRecordParam& param, s64 size);
    Result UnregisterRecord(StorageType storage, int index, ImageDbRecord& record);
    // the picture table follows the table of the DSi camera
    Result SyncPictureDatabaseCore(StorageType storage);
    Result SyncPictureDatabaseWithTwl(StorageType storage);
    // the records of the DSi table move to the records of the picture table with the same index
    Result SortPictureDatabaseCtrAndTwl(StorageType storage);
    static void StorePictureDbRecordFromParam(PictureDbRecord& record, const ImageDbRecordParam& param,
                                              ImageKind kind);
    Result WritePictureDatabase(StorageType storage, bool isLegacy, bool isBreakCrc);
    // NULL for an empty record
    ImageDbRecord* FindRecord(const ImageDbRecordLink& link) const;
    int GetEmptyImageNum(StorageType storage) const;
    void InsertRecordLinkList(DynamicArray<ImageDbRecordLink, XAllocator<ImageDbRecordLink> >& list,
                              const ImageDbRecordLink& link) const;
    u32 FindRecordLinkListInsertPos(const DynamicArray<ImageDbRecordLink, XAllocator<ImageDbRecordLink> >& list,
                                    const ImageDbRecordLink& link) const;

private:
    // the order of the list: date, time of the image, time of saving, kind, storage, record
    bool IsLessRecordLink(const ImageDbRecordLink& a, const ImageDbRecordLink& b) const;
    void AddRecordLinks(StorageType storage);

    bool m_IsInitialized;                                                    // +0x04
    bool m_IsCancelRequested;                                                // +0x05
    bool m_IsRestoreRequired[2];                                             // +0x06, per storage
    PictureDatabase m_Databases[2];                                          // +0x08
    DynamicArray<ImageDbRecordLink, XAllocator<ImageDbRecordLink> > m_RecordLinks;  // +0x98
};
ASSERT_SIZE(ImageDatabase, 0xB0);

} // namespace imgdb
