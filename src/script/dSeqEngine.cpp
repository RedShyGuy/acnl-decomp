#include "script/dSeqEngine.h"
#include "script/dITalkRecept.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "nn/nstd/nstd_String.h"
#include "script/dToUpper.h"

namespace script {
// 0x005F39C4
void SeqEngine::InsertSexText(IRecept* pSpeaker, const Tag* tag, const void*)
{
    static const u32 SEX_NUM = 2;

    // the parameters: per sex the size of the text in bytes and the text
    TextPtr texts[SEX_NUM];
    u16 lengths[SEX_NUM];
    const u8* param = reinterpret_cast<const u8*>(tag->args);
    for (u32 i = 0; i < SEX_NUM; i++) {
        u16 size;
        nnnstdMemCpy(&size, param, sizeof(size));
        lengths[i] = size / sizeof(char16);
        param += sizeof(size);
        texts[i].pText = reinterpret_cast<const char16*>(param);
        param += size;
    }

    u32 sex = 0;
    if (pSpeaker != NULL) {
        switch (pSpeaker->GetSpeakerSex()) {
        case 1:
            break;
        case 2:
            sex = 1;
            break;
        }
    } else {
        const SvPlayer* player = SvGetCurrentPlayer();
        if (player != NULL && player->playerInfo.IsValid() && (player->playerInfo.playerData.gender & 1) != 0) {
            sex = 1;
        }
    }

    const char16* begin = texts[sex].pText;
    const char16* end = begin + lengths[sex];
    m_Frame.PushClose();
    m_Frame.pEngine->PushText(&begin, &end);
    m_Frame.PushOpen();
}

// 0x00602568
bool SeqEngine::ProcessCapital(char16* c, const CharInfo* info)
{
    bool isChanged = false;
    if ((m_IsCapitalNext || m_CapitalState == 1) && info->size / sizeof(char16) == 1) {
        char16 upper = *info->pText;
        if (ToUpper(&upper)) {
            *c = upper;
            isChanged = true;
        }
    }
    m_IsCapitalNext = 0;
    if (m_CapitalState == 1) {
        m_CapitalState = 2;
    } else if (m_CapitalState == 2) {
        if (info->units == ' ' || info->units == 0x3000 || info->units == '\n') {
            m_CapitalState = 1;
        }
    }
    return isChanged;
}

// 0x00602614
void SeqEngine::Reset()
{
    ResetText();
    m_IsCapitalNext = 0;
    m_CapitalState = 0;
    m_Frame.isTag2 = 0;
    m_Frame.isTag3 = 0;
    m_Frame.color = InsertFrame::COLOR_NONE;
    for (u32 i = 0; i < 6; i++) {
        m_Frame.pWords[i] = NULL;
    }
    m_Frame.pBegin = NULL;
    m_Frame.lastChar = 0;
}

// 0x00602690 (name is ours)
SeqEngine::SeqEngine() : m_IsCapitalNext(0), m_CapitalState(0), m_Frame(this)
{
    for (u32 i = 0; i < 6; i++) {
        m_Frame.pWords[i] = NULL;
    }
    m_Frame.pBegin = NULL;
    m_Frame.lastChar = 0;
}

// 0x006026F8 slot 0x00
// 0x006026E8 (deleting dtor)
SeqEngine::~SeqEngine()
{
    // empty (in the original too)
}
} // namespace script
