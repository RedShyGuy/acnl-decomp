#include "imgdb/JpegMpBaseSaver.h"
#include "imgdb/SysMakerNote.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

namespace {

// SaveProcessType of a saved photo and of a screenshot (names are ours)
const u32 SAVE_PROCESS_TYPE_PHOTO = 4;
const u32 SAVE_PROCESS_TYPE_SCREENSHOT = 5;
// the screenshot bit of DistinctionTypeBit
const u8 DISTINCTION_TYPE_BIT_SCREENSHOT = 0x80;
const u16 HANDLE_TYPE_BIT_4 = 1 << 4;

} // namespace

// 0x005A5BC8 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseSaver::SetScreenshotFlag(bool isScreenshot)
{
    SysMakerNote* note = m_pSysMakerNote;
    u8 bits = note->GetDistinctionTypeBit();
    if (isScreenshot)
    {
        note->m_SaveProcessType = SAVE_PROCESS_TYPE_SCREENSHOT;
        note->SetDistinctionTypeBit(static_cast<DistinctionTypeBit>(bits | DISTINCTION_TYPE_BIT_SCREENSHOT));
    }
    else
    {
        note->m_SaveProcessType = SAVE_PROCESS_TYPE_PHOTO;
        note->SetDistinctionTypeBit(static_cast<DistinctionTypeBit>(bits & ~DISTINCTION_TYPE_BIT_SCREENSHOT));
    }
}

// 0x005A9B98 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseSaver::SetTitleUniqueId(u32 titleUniqueId)
{
    m_pSysMakerNote->m_TitleUniqueId = titleUniqueId;
    m_HasTitleUniqueId = true;
}

// 0x005A9BAC (name is ours)
void imgdb::JpegMpBaseSaver::SetHandleTypeBit4(bool isSet)
{
    if (isSet)
    {
        m_pSysMakerNote->m_HandleTypeBit |= HANDLE_TYPE_BIT_4;
    }
    else
    {
        m_pSysMakerNote->m_HandleTypeBit &= ~HANDLE_TYPE_BIT_4;
    }
}

// 0x00755980 (name is ours)
bool imgdb::JpegMpBaseSaver::GetHandleTypeBit4() const
{
    return (m_pSysMakerNote->m_HandleTypeBit & HANDLE_TYPE_BIT_4) != 0;
}

// (a photo of this console)
// 0x005A9BC8 | nintendogs:bytes [tier B]
imgdb::JpegMpBaseSaver::JpegMpBaseSaver() : m_pUserMakerNote(NULL), m_UserMakerNoteSize(0)
{
    m_pSysMakerNote = static_cast<SysMakerNote*>(GetAllocator()->Allocate(sizeof(SysMakerNote), 4));
    m_HasTitleUniqueId = false;
    m_pSysMakerNote->Initialize();
    SetScreenshotFlag(false);
    SetHandleTypeBit4(false);
    m_pSysMakerNote->m_BodyIds[0] = util::GetOwnBodyId();
    m_pSysMakerNote->m_BodyIdValidBits |= 1 << 0;
}

// 0x005A9C94
// 0x005A9C5C (deleting dtor)
imgdb::JpegMpBaseSaver::~JpegMpBaseSaver()
{
    GetAllocator()->Free(m_pSysMakerNote);
}

} // namespace imgdb
