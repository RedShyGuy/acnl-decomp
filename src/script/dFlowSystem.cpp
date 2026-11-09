#include "script/dFlowSystem.h"
#include "script/dScriptExtern.h"
#include "script/dIFlowRecept.h"
#include "script/dITalkRecept.h"
#include "script/dTalkSystem.h"

#include <new>

namespace script {
// 0x00600864 (name is ours)
void FlowSystem::SetRecept(IFlowRecept* recept, TalkSystem* talk)
{
    m_pRecept = recept;
    recept->m_pOwner = this;
    ITalkRecept* talkRecept = recept->m_pTalkRecept;
    if (talkRecept != NULL) {
        talk->SetRecept(talkRecept);
    }
}

// 0x00600904 (name is ours)
void FlowSystem::ReleaseRecept()
{
    IFlowRecept* recept = m_pRecept;
    if (recept == NULL) {
        return;
    }
    ITalkRecept* talkRecept = recept->m_pTalkRecept;
    if (talkRecept != NULL && talkRecept->m_pOwner != NULL) {
        talkRecept->m_pOwner->ReleaseRecept();
    }
    recept->m_pOwner = NULL;
    recept->m_File.clear();
    recept->m_Label.clear();
    recept->m_EventState = 0;
    recept->m_MenuFilter = NULL;
    m_pRecept = NULL;
}

// 0x006009AC (name is ours)
void FlowSystem::UpdateEndTalk()
{
    TalkSystem* talk = m_Message.m_pTalk->m_pOwner;
    switch (m_Message.m_EndMode) {
    case 2:
        if (talk->m_State == TalkSystem::STATE_IDLE && talk->m_pJump->IsEnd()) {
            m_Message.Clear();
            m_State = STATE_RUN;
        }
        break;
    case 1:
        if (talk->m_State == TalkSystem::STATE_KEEP) {
            if (talk->m_QuitMode != TalkSystem::QUIT_NONE) {
                break;
            }
            talk->m_IsEnd = 1;
            talk->m_NextState = TalkSystem::STATE_CLOSE;
            m_Message.m_EndMode = 2;
        } else if (talk->m_State == TalkSystem::STATE_CLOSED) {
            talk->Reset();
            talk->m_NextState = TalkSystem::STATE_IDLE;
            m_Message.m_EndMode = 2;
        }
        break;
    }
}

// 0x00600A94 (name is ours)
void FlowSystem::UpdateRun()
{
    if (m_pCurrent == NULL) {
        return;
    }
    m_pCurrent->m_IsYield = 0;
    do {
        m_pCurrent->Update();
        if (m_pCurrent->m_IsDone) {
            // the flow is popped
            if (m_Depth != 0) {
                m_Depth--;
                if (m_Depth != 0) {
                    m_pCurrent = m_pSeqs[m_Depth - 1];
                } else {
                    m_pCurrent = NULL;
                }
            } else {
                m_pCurrent = NULL;
            }
            // a jump starts the next flow
            if (m_pJumpFile != NULL) {
                s32 entry = m_JumpEntry;
                if (entry >= 0) {
                    if (Push()) {
                        m_pCurrent->Start(m_pJumpFile, entry);
                    }
                } else {
                    const char* label = m_pJumpLabel;
                    if (Push()) {
                        m_pCurrent->Start(m_pJumpFile, label);
                    }
                }
                m_pJumpFile = NULL;
                m_pJumpLabel = NULL;
                m_JumpEntry = -1;
            }
        }
        if (m_pCurrent == NULL) {
            if (m_Message.m_pTalk != NULL && m_Message.m_pTalk->m_pOwner != NULL) {
                m_Message.m_EndMode = 1;
                m_State = STATE_END_TALK;
            }
            return;
        }
    } while (!m_pCurrent->m_IsYield);
}

// 0x00600CC0 (name is ours)
void FlowSystem::Start(const char* file, const char* label)
{
    if (m_pRecept == NULL) {
        return;
    }
    if (m_pCurrent == NULL) {
        Func23CCB4();
    }
    if (!Push()) {
        return;
    }
    if (file == NULL) {
        IFlowRecept* recept = m_pRecept;
        file = recept->m_File.getCString();
        label = recept->m_Label.getCString();
    }
    m_pCurrent->Start(file, label);
}

// 0x00600D5C (name is ours)
void FlowSystem::RequestJump(const char* file, const char* label)
{
    if (m_pCurrent != NULL) {
        m_pCurrent->m_IsEnd = 1;
        m_pJumpFile = file;
        m_pJumpLabel = label;
        m_JumpEntry = -1;
    }
}

// 0x00600DB4 (name is ours)
FlowSystem::FlowSystem(nn::fnd::DateTimeParameters* pDate)
    : m_pDate(pDate), m_Depth(0), m_pCurrent(NULL), m_pRecept(NULL), m_State(STATE_RUN), m_Message(),
      m_pJumpFile(NULL), m_pJumpLabel(NULL), m_JumpEntry(-1)
{
    for (u32 i = 0; i < SEQ_NUM; i++) {
        m_pSeqs[i] = new (std::nothrow) FlowSeq(this, &m_Message);
    }
}

} // namespace script
