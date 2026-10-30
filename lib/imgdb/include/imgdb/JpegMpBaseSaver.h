#pragma once

#include "imgdb/ArchiveMounter.h"
#include "imgdb/ImageDbRecordParam.h"
#include "imgdb/SysMakerNote.h"
#include "imgdb/imgdb_Types.h"

namespace imgdb {

// the maker notes of JpegSaver and MpSaver (member names are ours)
// RTTI N5imgdb15JpegMpBaseSaverE @ 0x008D2B24
// vtable 0x00908F30 (vptr 0x00908F38), offset_to_top 0, 2 entries
class JpegMpBaseSaver
{
public:
    JpegMpBaseSaver();
    virtual ~JpegMpBaseSaver();

    void SetScreenshotFlag(bool isScreenshot);
    void SetTitleUniqueId(u32 titleUniqueId);
    // bit 4 of the handle type (such images must be at least 400x240)
    void SetHandleTypeBit4(bool isSet);
    bool GetHandleTypeBit4() const;

protected:
    // the length of the path the savers return
    static const int PATH_SIZE = 44;

    // the SD card must be in the slot, mounted and writable
    static Result CheckSdCard()
    {
        SdArchiveMounter* mounter = SdArchiveMounter::GetInstance();
        if (!SdArchiveMounter::IsInsert())
        {
            return RESULT_FS_NOT_FOUND;
        }
        if (SdArchiveMounter::IsInsert() && !mounter->IsMount())
        {
            return RESULT_FS_ARCHIVE_ERROR;
        }
        if (!SdArchiveMounter::IsWritable())
        {
            return RESULT_FS_ACCESS_ERROR;
        }
        return RESULT_SUCCESS;
    }

    // a failed save while the SD card is gone is a file system error
    static Result ConvertSaveResult(Result result)
    {
        if (!SdArchiveMounter::GetInstance()->IsMount() || !SdArchiveMounter::IsInsert() ||
            result == RESULT_FS_NOT_FOUND || result == RESULT_FS_ARCHIVE_ERROR)
        {
            return RESULT_FS_ERROR;
        }
        return result;
    }

    // the database values of the maker note
    void StoreRecordParam(ImageDbRecordParam& param, const DateTimeSeconds& dateTime) const
    {
        const SysMakerNote* note = m_pSysMakerNote;
        param.Initialize();
        param.m_DateTime = dateTime;
        param.m_TitleUniqueId = note->m_TitleUniqueId;
        param.m_SaveProcessType = note->m_SaveProcessType;
        param.m_HandleTypeBit = note->m_HandleTypeBit;
        param.m_DistinctionTypeBit = note->GetDistinctionTypeBit();
        param.m_ShootingType = 0;
        for (int i = 0; i < BODY_ID_TYPE_NUM; i++)
        {
            BodyIdType type = static_cast<BodyIdType>(i);
            if (note->m_BodyIdValidBits & (1 << type))
            {
                param.SetBodyId(type, note->m_BodyIds[type]);
            }
            else
            {
                param.ResetBodyId(type);
            }
        }
    }

    const void* m_pUserMakerNote;   // +0x04
    u32 m_UserMakerNoteSize;        // +0x08
    SysMakerNote* m_pSysMakerNote;  // +0x0C
    bool m_HasTitleUniqueId;        // +0x10
};
ASSERT_SIZE(JpegMpBaseSaver, 0x14);

} // namespace imgdb
