#include "script/dTalkRuby.h"
#include "Sound/dSoundMgr.h"
#include "script/dRenderBase.h"
#include "script/dScriptExtern.h"
#include "script/dTalkSystem.h"

namespace script {
// 0x005FE150 slot 0x14
void TalkRuby::OnChar(const CharInfo* info)
{
    m_pRender->Put(info);
    m_Timer.count++;
    if (m_Mode == MODE_RUBY) {
        QueueTalkSpeech(m_pOwner->m_pState, static_cast<u16>(info->units), 100);
    }
    m_CharNum++;
}

// 0x005FE1F4 slot 0x0C
void TalkRuby::End()
{
    m_IsDone = 1;
}

// 0x005FE200 slot 0x18
void TalkRuby::OnTag(const Tag*)
{
    // empty (in the original too)
}

// 0x005FE204 slot 0x08
void TalkRuby::Begin()
{
    // empty (in the original too)
}

// 0x005FE208 slot 0x10
bool TalkRuby::IsContinue()
{
    if (m_IsDone) {
        return false;
    }
    return m_Timer.Step(m_IsFast);
}

// 0x005FE290 slot 0x00
// 0x005FE280 (deleting dtor)
TalkRuby::~TalkRuby()
{
    // empty (in the original too)
}
} // namespace script
