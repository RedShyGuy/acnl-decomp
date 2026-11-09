#include "script/dFlowSeq.h"
#include "script/dFlowSystem.h"
#include "script/dIFlowRecept.h"
#include "script/dITalkRecept.h"
#include "script/dMgr.h"
#include "script/dProject.h"
#include "script/dTalkSeq.h"
#include "script/dTalkSystem.h"
#include "Bs/dBsDemoCancelMgr.h"
#include "libms/FlowNodeBranch.h"
#include "libms/FlowNodeEntry.h"
#include "libms/FlowNodeEvent.h"
#include "libms/FlowNodeJump.h"
#include "libms/FlowNodeMessage.h"
#include "sead/seadStringFormatter.h"
#include "time/time_Api.h"

namespace script {
// (inline)
inline bool FlowMessage::CanStart(const TalkSystem* talk)
{
    u8 state = talk->m_State;
    return state == TalkSystem::STATE_IDLE || state == TalkSystem::STATE_CLOSED ||
           (state == TalkSystem::STATE_KEEP && talk->m_QuitMode == TalkSystem::QUIT_NONE);
}

// 0x005F402C (name is ours)
void FlowMessage::UpdateWait()
{
    TalkSystem* talk = m_pTalk->m_pOwner;
    if (talk == NULL) {
        return;
    }
    if (talk->m_State == TalkSystem::STATE_IDLE && talk->m_pJump->IsEnd()) {
        m_State = STATE_END;
        return;
    }
    if (talk->m_State == TalkSystem::STATE_KEEP ||
        (talk->m_State == TalkSystem::STATE_CLOSED && talk->m_pJump->IsEnd())) {
        m_State = STATE_KEPT;
    }
}

// 0x005F409C (name is ours)
void FlowMessage::UpdateStart()
{
    TalkSystem* talk = m_pTalk->m_pOwner;
    if (!CanStart(talk)) {
        return;
    }
    m_pRecept->OnStartMessage(&m_File);
    ITalkRecept* recept = m_pTalk;
    recept->m_File.clear();
    recept->m_Label.clear();
    recept->m_Number = -1;
    m_pTalk->m_File.copy(sead::SafeStringBase<char>(m_File.getCString()), -1);
    s32 number = m_Number;
    if (number > 0) {
        sead::StringFormatter formatter(&m_pTalk->m_Label);
        formatter.append("%03d").appendValue(number).flush();
    } else if (m_LabelName.isEmpty()) {
        m_pTalk->m_Number = m_Label;
    } else {
        m_pTalk->m_Label.copy(sead::SafeStringBase<char>(m_LabelName.getCString()), -1);
    }
    talk->m_IsCloseAtEnd = 0;
    talk->m_NextState = TalkSystem::STATE_LOAD;
    if (talk->m_State == TalkSystem::STATE_KEEP) {
        talk->m_pSeq->Reset();
    }
    m_State = STATE_WAIT;
}

// 0x005F4264 (name is ours)
void FlowMessage::UpdateResume()
{
    TalkSystem* talk = m_pTalk->m_pOwner;
    if (!CanStart(talk)) {
        return;
    }
    m_pRecept->OnStartMessage(&m_File);
    if (m_Number > 0) {
        talk->Jump(m_Number, m_File.getCString(), NULL, 3);
    } else if (m_LabelName.isEmpty()) {
        s32 label = m_Label;
        const char* file = m_File.getCString();
        TalkLabel* jump = talk->m_pJump;
        jump->m_Flags = TalkLabel::FLAG_INDEX;
        jump->m_Index = label;
        if (file != NULL) {
            talk->m_pJump->m_File.copy(sead::SafeStringBase<char>(file), -1);
        }
    } else {
        sead::SafeStringBase<char> label(m_LabelName.getCString());
        const char* file = m_File.getCString();
        TalkLabel* jump = talk->m_pJump;
        jump->m_Label.copy(label, -1);
        jump->m_Flags = TalkLabel::FLAG_LABEL;
        if (file != NULL) {
            talk->m_pJump->m_File.copy(sead::SafeStringBase<char>(file), -1);
        }
    }
    if (m_IsResumed) {
        talk->m_pJump->m_IsUnknown0x79 = 1;
    }
    talk->m_IsCloseAtEnd = 0;
    talk->m_NextState = TalkSystem::STATE_LOAD;
    if (talk->m_State == TalkSystem::STATE_KEEP) {
        talk->m_pSeq->Reset();
    }
    m_State = STATE_WAIT;
}

// 0x005F442C slot 0x14 (name is ours)
void FlowSeq::OnEntry(libms::Flow* flow, const libms::FlowNode& node)
{
    (void)flow;
    libms::FlowNodeEntry entry(node);
    IFlowRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        m_IsWaiting = recept->OnEntry(entry) == IFlowRecept::RESULT_WAIT;
    }
    if (!m_IsWaiting) {
        if (m_ReturnNode >= 0) {
            m_Next = m_ReturnNode;
            m_ReturnNode = -1;
        } else if (entry.HasNext()) {
            m_Next = entry.GetNext();
        } else {
            m_IsEnd = 1;
        }
    }
}

// 0x005F44D4 slot 0x10 (name is ours)
void FlowSeq::OnEvent(libms::Flow* flow, const libms::FlowNode& node)
{
    libms::FlowNodeEvent event(node);
    IFlowRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        m_IsWaiting = recept->OnEvent(flow, event) == IFlowRecept::RESULT_WAIT;
    }
    if (!m_IsWaiting) {
        if (m_ReturnNode >= 0) {
            m_Next = m_ReturnNode;
            m_ReturnNode = -1;
        } else if (event.HasNext()) {
            m_Next = event.GetNext();
        } else {
            m_IsEnd = 1;
        }
    }
}

// 0x005F4584 slot 0x0C (name is ours)
void FlowSeq::OnBranch(libms::Flow* flow, const libms::FlowNode& node)
{
    libms::FlowNodeBranch branch(node);
    u32 num = branch.GetCaseNum();
    s32 next = -1;
    if (num != 0) {
        IFlowRecept* recept = m_pOwner->m_pRecept;
        if (recept != NULL) {
            u32 index = num;
            if (recept->OnBranch(&index, branch) == IFlowRecept::RESULT_WAIT) {
                m_IsWaiting = 1;
            } else {
                if (index >= num) {
                    index = 0;
                }
                m_IsWaiting = 0;
            }
            if (index < num) {
                const u16* cases = flow->GetBranchCases(m_Index);
                if (cases != NULL && cases[index] != 0xFFFF) {
                    next = cases[index];
                }
            }
        }
    }
    if (!m_IsWaiting) {
        if (m_ReturnNode >= 0) {
            m_Next = m_ReturnNode;
            m_ReturnNode = -1;
        } else if (next >= 0) {
            m_Next = next;
        } else {
            m_IsEnd = 1;
        }
    }
}

// 0x005F4688 (name is ours)
s32 FlowSeq::StartMessage(const sead::SafeStringBase<char>& file, const char* label)
{
    FlowMessage* message = m_pMessage;
    s32 result = 2;
    switch (message->m_State) {
    case FlowMessage::STATE_NONE: {
        u8 state = FlowMessage::STATE_END;
        IFlowRecept* recept = m_pOwner->m_pRecept;
        if (recept != NULL) {
            message->m_pRecept = recept;
            ITalkRecept* talk = recept->m_pTalkRecept;
            message->m_pTalk = talk;
            if (talk != NULL && talk->m_pOwner != NULL) {
                message->m_File.copy(file, -1);
                message->m_Label = -1;
                message->m_Number = -1;
                message->m_LabelName.copy(sead::SafeStringBase<char>(label), -1);
                state = FlowMessage::STATE_START;
                message->m_IsResumed = 0;
            }
        }
        message->m_State = state;
        break;
    }
    case FlowMessage::STATE_KEPT: {
        u8 state = FlowMessage::STATE_END;
        if (message->m_pRecept != NULL && message->m_pTalk != NULL && message->m_pTalk->m_pOwner != NULL) {
            message->m_File.copy(file, -1);
            message->m_Label = -1;
            message->m_Number = -1;
            message->m_LabelName.copy(sead::SafeStringBase<char>(label), -1);
            state = FlowMessage::STATE_RESUME;
            message->m_IsResumed = 0;
        }
        message->m_State = state;
        break;
    }
    }
    m_pMessage->Update();
    message = m_pMessage;
    if (message->m_State == FlowMessage::STATE_END) {
        message->Clear();
        result = 1;
    } else if (message->m_State == FlowMessage::STATE_KEPT) {
        result = 1;
    }
    return result;
}

// 0x005F4840 (name is ours)
s32 FlowSeq::StartMessage(const sead::SafeStringBase<char>& file, s32 number)
{
    FlowMessage* message = m_pMessage;
    s32 result = 2;
    switch (message->m_State) {
    case FlowMessage::STATE_NONE: {
        u8 state = FlowMessage::STATE_END;
        IFlowRecept* recept = m_pOwner->m_pRecept;
        if (recept != NULL) {
            message->m_pRecept = recept;
            ITalkRecept* talk = recept->m_pTalkRecept;
            message->m_pTalk = talk;
            if (talk != NULL && talk->m_pOwner != NULL) {
                message->m_File.copy(file, -1);
                message->m_Number = number;
                message->m_Label = -1;
                message->m_LabelName.clear();
                state = FlowMessage::STATE_START;
                message->m_IsResumed = 0;
            }
        }
        message->m_State = state;
        break;
    }
    case FlowMessage::STATE_KEPT: {
        u8 state = FlowMessage::STATE_END;
        if (message->m_pRecept != NULL && message->m_pTalk != NULL && message->m_pTalk->m_pOwner != NULL) {
            message->m_File.copy(file, -1);
            message->m_Number = number;
            message->m_Label = -1;
            message->m_LabelName.clear();
            state = FlowMessage::STATE_RESUME;
            message->m_IsResumed = 0;
        }
        message->m_State = state;
        break;
    }
    }
    m_pMessage->Update();
    message = m_pMessage;
    if (message->m_State == FlowMessage::STATE_END) {
        message->Clear();
        result = 1;
    } else if (message->m_State == FlowMessage::STATE_KEPT) {
        result = 1;
    }
    return result;
}

// 0x005F49D8 (name is ours)
void FlowSeq::UpdateEnd()
{
    FlowSeq* parent = m_pParent;
    IFlowRecept* recept = m_pOwner->m_pRecept;
    if (parent == NULL) {
        recept->OnFlowEnd();
    }
    m_Flow.Finalize();
    m_pNode = NULL;
    m_Index = -1;
    m_Next = -1;
    if (parent == NULL) {
        m_File.Clear();
    }
    m_StartNode = -1;
    m_Label.clear();
    m_Name.clear();
    m_pParent = NULL;
    m_NextState = STATE_IDLE;
    m_IsDone = 1;
    m_IsAborted = 0;
    m_IsYield = 1;
    ITalkRecept* talk = m_pOwner->m_pRecept->m_pTalkRecept;
    if (talk != NULL && talk->m_pOwner != NULL) {
        talk->m_pOwner->m_pSeq->m_RandomBranch.isNoRepeat = 0;
    }
}

// 0x005F4AD0 (name is ours)
void FlowSeq::UpdateIdle()
{
    m_IsYield = 1;
}

// 0x005F4ADC (name is ours)
void FlowSeq::EnterEnd()
{
    // empty (in the original too)
}

// 0x005F4AE0 (name is ours)
void FlowSeq::EnterIdle()
{
    // empty (in the original too)
}

// 0x005F4AE4 slot 0x08 (name is ours)
void FlowSeq::OnMessage(libms::Flow* flow, const libms::FlowNode& node)
{
    (void)flow;
    libms::FlowNodeMessage messageNode(node);
    FlowMessage* message = m_pMessage;
    switch (message->m_State) {
    case FlowMessage::STATE_NONE: {
        u8 state = FlowMessage::STATE_END;
        IFlowRecept* recept = m_pOwner->m_pRecept;
        if (recept != NULL) {
            message->m_pRecept = recept;
            ITalkRecept* talk = recept->m_pTalkRecept;
            message->m_pTalk = talk;
            if (talk != NULL) {
                TalkSystem* system = talk->m_pOwner;
                bool hasFile = messageNode.HasFile();
                bool hasLabel = messageNode.HasLabel();
                if (system != NULL && hasFile && hasLabel) {
                    u16 file = messageNode.GetFile();
                    s32 label = messageNode.GetLabel();
                    Project* project = Project::s_pInstance;
                    const char* name = project->m_Project.GetName(file);
                    if (name != NULL && project->GetFileName(&message->m_File, name)) {
                        message->m_Label = label;
                        message->m_Number = -1;
                        message->m_LabelName.clear();
                        message->m_IsResumed = 0;
                        state = FlowMessage::STATE_START;
                    }
                }
            }
        }
        message->m_State = state;
        m_IsWaiting = 1;
        break;
    }
    case FlowMessage::STATE_KEPT: {
        u8 state = FlowMessage::STATE_END;
        if (message->m_pRecept != NULL && message->m_pTalk != NULL) {
            TalkSystem* system = message->m_pTalk->m_pOwner;
            bool hasFile = messageNode.HasFile();
            bool hasLabel = messageNode.HasLabel();
            if (system != NULL && hasFile && hasLabel) {
                u16 file = messageNode.GetFile();
                s32 label = messageNode.GetLabel();
                Project* project = Project::s_pInstance;
                const char* name = project->m_Project.GetName(file);
                if (name != NULL && project->GetFileName(&message->m_File, name)) {
                    message->m_Label = label;
                    message->m_Number = -1;
                    message->m_LabelName.clear();
                    message->m_IsResumed = 0;
                    state = FlowMessage::STATE_RESUME;
                }
            }
        }
        message->m_State = state;
        m_IsWaiting = 1;
        break;
    }
    }
    m_pMessage->Update();
    message = m_pMessage;
    if (message->m_State == FlowMessage::STATE_END) {
        message->Clear();
        m_IsWaiting = 0;
    } else if (message->m_State == FlowMessage::STATE_KEPT) {
        m_IsWaiting = 0;
    }
    if (!m_IsWaiting) {
        if (m_ReturnNode >= 0) {
            m_Next = m_ReturnNode;
            m_ReturnNode = -1;
        } else if (messageNode.HasNext()) {
            m_Next = messageNode.GetNext();
        } else {
            m_IsEnd = 1;
        }
    }
}

// 0x005F4DCC (name is ours)
s32 FlowSeq::StartMessage(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    FlowMessage* message = m_pMessage;
    s32 result = 2;
    switch (message->m_State) {
    case FlowMessage::STATE_NONE: {
        u8 state = FlowMessage::STATE_END;
        IFlowRecept* recept = m_pOwner->m_pRecept;
        if (recept != NULL) {
            message->m_pRecept = recept;
            ITalkRecept* talk = recept->m_pTalkRecept;
            message->m_pTalk = talk;
            if (talk != NULL && talk->m_pOwner != NULL && node.GetKind() == libms::FlowNodeEvent::KIND_CALL) {
                u32 offset;
                node.GetParam(&offset);
                const char* file = static_cast<const char*>(flow->GetData(offset));
                if (file != NULL) {
                    message->m_File.copy(sead::SafeStringBase<char>(file), -1);
                    message->m_Label = -1;
                    message->m_Number = -1;
                    message->m_LabelName.clear();
                    message->m_IsResumed = 0;
                    state = FlowMessage::STATE_START;
                }
            }
        }
        message->m_State = state;
        break;
    }
    case FlowMessage::STATE_KEPT: {
        u8 state = FlowMessage::STATE_END;
        if (message->m_pRecept != NULL && message->m_pTalk != NULL && message->m_pTalk->m_pOwner != NULL &&
            node.GetKind() == libms::FlowNodeEvent::KIND_CALL) {
            u32 offset;
            node.GetParam(&offset);
            const char* file = static_cast<const char*>(flow->GetData(offset));
            if (file != NULL) {
                message->m_File.copy(sead::SafeStringBase<char>(file), -1);
                message->m_Label = -1;
                message->m_Number = -1;
                message->m_LabelName.clear();
                message->m_IsResumed = 1;
                state = FlowMessage::STATE_RESUME;
            }
        }
        message->m_State = state;
        break;
    }
    }
    m_pMessage->Update();
    message = m_pMessage;
    if (message->m_State == FlowMessage::STATE_END) {
        message->Clear();
        result = 1;
    } else if (message->m_State == FlowMessage::STATE_KEPT) {
        result = 1;
    }
    return result;
}

// 0x005F5014 (name is ours)
void FlowSeq::UpdateLoad()
{
    if (m_pParent != NULL || m_pOwner->m_pRecept->LoadFlow(&m_File, m_Name, NULL)) {
        FlowSeq* root = this;
        while (root->m_pParent != NULL) {
            root = root->m_pParent;
        }
        s32 start = m_StartNode;
        if (start >= 0) {
            m_Flow.Initialize(root->m_File.m_Loader.GetData());
            m_Next = start;
            m_Index = -1;
        } else {
            m_Flow.Initialize(root->m_File.m_Loader.GetData());
            m_Index = -1;
            m_Next = m_Flow.GetEntryIndex(m_Label.getCString());
        }
        m_IsEnd = 0;
        m_NextState = STATE_RUN;
    }
    m_IsYield = 1;
}

// 0x005F50F8 (name is ours)
void FlowSeq::UpdateRun()
{
    if (!m_IsAborted) {
        Process();
        if (m_IsWaiting) {
            m_IsYield = 1;
        }
    }
    if (m_IsEnd || m_IsAborted) {
        m_NextState = STATE_END;
    }
}

// 0x005F5144 (name is ours)
void FlowSeq::EnterLoad()
{
    if (m_pParent == NULL) {
        m_pOwner->m_pRecept->LoadFlow(&m_File, m_Name, Mgr::s_pFlowHeap);
    }
}

// 0x005F5188 (name is ours)
void FlowSeq::EnterRun()
{
    // empty (in the original too)
}

// 0x005F518C (name is ours)
bool FlowSeq::CallEntry(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    if (node.GetKind() != libms::FlowNodeEvent::KIND_CALL) {
        return false;
    }
    u32 offset;
    node.GetParam(&offset);
    const char* label = static_cast<const char*>(flow->GetData(offset));
    if (label == NULL) {
        return false;
    }
    s32 entry = flow->GetEntryIndex(label);
    if (entry < 0) {
        return false;
    }
    if (!node.HasNext()) {
        m_ReturnNode = entry;
        return true;
    }
    FlowSystem* owner = m_pOwner;
    FlowSeq* current = owner->m_pCurrent;
    if (owner->Push()) {
        owner->m_pCurrent->Call(current, entry);
    }
    return true;
}

// 0x005F5298 slot 0x20 (name is ours)
bool FlowSeq::OnNodeDone()
{
    if (BsDemoCancelMgr::s_pInstance != NULL && BsDemoCancelMgr::IsActiveStage() &&
        BsDemoCancelMgr::s_pInstance->m_IsActive && BsDemoCancelMgr::s_pInstance->m_State == 0) {
        m_pOwner->Abort();
        return true;
    }
    bool isContinue = m_pOwner->m_pRecept->IsContinue();
    if (!isContinue) {
        m_IsYield = 1;
    }
    return isContinue;
}

// 0x005F5330 (name is ours)
void FlowSeq::Start(const char* file, const char* label)
{
    if (m_State != STATE_IDLE) {
        return;
    }
    Reset();
    m_Name.copy(sead::SafeStringBase<char>(file), -1);
    if (label == NULL) {
        label = file;
    }
    m_Label.copy(sead::SafeStringBase<char>(label), -1);
    m_IsDone = 0;
    m_IsAborted = 0;
    m_NextState = STATE_LOAD;
}

// 0x005F53D8 slot 0x1C (name is ours)
void FlowSeq::OnNode(const libms::FlowNode& node)
{
    *m_pOwner->m_pDate = *time::GetCurrentDate();
    IFlowRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL) {
        recept->m_EventState = 0;
        recept->OnNode(node);
    }
}

// 0x005F5430 slot 0x18 (name is ours)
void FlowSeq::OnJump(libms::Flow* flow, const libms::FlowNode& node)
{
    (void)flow;
    if (m_IsWaiting) {
        return;
    }
    libms::FlowNodeJump jump(node);
    if (jump.IsLocal()) {
        if (m_ReturnNode >= 0) {
            m_Next = m_ReturnNode;
            m_ReturnNode = -1;
        } else if (jump.HasNext()) {
            m_Next = jump.GetNext();
        } else {
            m_IsEnd = 1;
        }
        return;
    }
    u16 file = jump.GetFile();
    s32 entry = jump.GetNext();
    Project* project = Project::s_pInstance;
    const char* name = project->m_Project.GetName(file);
    if (name == NULL || !project->GetFileName(&s_JumpFile, name)) {
        return;
    }
    m_pOwner->RequestJump(s_JumpFile.getCString(), entry);
    m_IsEnd = 1;
}

// 0x005F559C slot 0x00 (name after the RTTI)
// 0x005F5560 (deleting dtor)
FlowSeq::~FlowSeq()
{
    // (only the members are destroyed)
}

} // namespace script
