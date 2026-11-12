#include "script/dIFlowRecept.h"
#include "Bs/dBsMenuMgr.h"
#include "Other/dDemoActor.h"
#include "Other/dProcMgr.h"
#include "Other/dVisitorLocalInfo.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvCensus.h"
#include "Sv/dSvGardenPlus.h"
#include "demo/demo_Api.h"
#include "demo/demo_Extern.h"
#include "event/event_Extern.h"
#include "libms/FlowNodeBranch.h"
#include "libms/FlowNodeEntry.h"
#include "libms/FlowNodeEvent.h"
#include "netgame/dNetGameMgr.h"
#include "netgame/netgame_Api.h"
#include "nn/os/CTR/CTR_Api.h"
#include "oml/framework/dProcessManager.h"
#include "script/dChoiceStandardWin.h"
#include "script/dCmpEngine.h"
#include "script/dFlowSystem.h"
#include "script/dITalkRecept.h"
#include "script/dInsert.h"
#include "script/dMgr.h"
#include "script/dScriptExtern.h"
#include "script/dTalkPhrase.h"
#include "script/dTalkSeq.h"
#include "script/dTalkSystem.h"
#include "script/dWordFix.h"
#include "sead/seadStringFormatter.h"
#include "stage/dTransitionMgr.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"
#include "time/time_Api.h"

namespace script {
namespace {
// (inline) the second state of a menu event: done when the menus are closed
DECOMP_ALWAYS_INLINE s32 WaitMenuClosed(u32 state)
{
    if (state != 1) {
        return IFlowRecept::RESULT_NONE;
    }
    return BsMenuMgr::IsIdle() ? IFlowRecept::RESULT_DONE : IFlowRecept::RESULT_WAIT;
}

// (inline) the talk recept of the actor the player talks to (+0x68)
DECOMP_ALWAYS_INLINE ITalkRecept* GetActorTalkRecept(DemoActor* actor)
{
    return *reinterpret_cast<ITalkRecept**>(reinterpret_cast<u8*>(actor) + 0x68);
}

// (inline) the save data at +0x290A4 of the garden (purpose unknown; the Sv structures have it in the 4th player)
DECOMP_ALWAYS_INLINE u8* GetUnknownData290A4()
{
    return &GetGardenPlus()->data.players.players[3].unk_0x9604[0xC80];
}

// (inline) the flag of the visited town by its number (-1: none)
DECOMP_ALWAYS_INLINE s32 GetVisitorFlag(const VisitorLocalInfo::Flags* flags, u32 index)
{
    switch (index) {
    case 0:
        return flags->bit0;
    case 1:
        return flags->bit1;
    case 2:
        return flags->bit2;
    case 3:
        return flags->bit3;
    case 4:
        return flags->bit4;
    case 5:
        return flags->bit5;
    case 6:
        return flags->bit6;
    case 7:
        return flags->bit7;
    case 8:
        return flags->bit8;
    case 9:
        return flags->bit9;
    case 10:
        return flags->bit10;
    case 11:
        return flags->bit11;
    case 12:
        return flags->bit12;
    case 13:
        return flags->bit13;
    case 14:
        return flags->bit14;
    case 15:
        return flags->bit15;
    case 16:
        return flags->bit16;
    case 17:
        return flags->bit17;
    case 18:
        return flags->bit18;
    case 19:
        return flags->bit19;
    case 20:
        return flags->bit20;
    case 21:
        return flags->bit21;
    case 22:
        return flags->bit22;
    case 23:
        return flags->bit23;
    case 24:
        return flags->bit24;
    case 25:
        return flags->bit25;
    case 26:
        return flags->bit26;
    case 27:
        return flags->bit27;
    case 28:
        return flags->bit28;
    case 29:
        return flags->bit29;
    case 30:
        return flags->bit30;
    case 31:
        return flags->bit31;
    case 32:
        return flags->bit32;
    case 33:
        return flags->bit33;
    case 34:
        return flags->bit34;
    case 35:
        return flags->bit35;
    case 36:
        return flags->bit36;
    case 37:
        return flags->bit37;
    case 38:
        return flags->bit38;
    case 39:
        return flags->bit39;
    case 40:
        return flags->bit40;
    case 41:
        return flags->bit41;
    case 42:
        return flags->bit42;
    case 43:
        return flags->bit43;
    case 44:
        return flags->bit44;
    case 45:
        return flags->bit45;
    case 46:
        return flags->bit46;
    case 47:
        return flags->bit47;
    case 48:
        return flags->bit48;
    case 49:
        return flags->bit49;
    case 50:
        return flags->bit50;
    default:
        return -1;
    }
}

// (inline) the case 0 if the condition holds, otherwise the case 1
DECOMP_ALWAYS_INLINE s32 SelectCase(u32* index, bool isFirst)
{
    *index = isFirst ? 0 : 1;
    return IFlowRecept::RESULT_DONE;
}
} // namespace

// 0x0095EF08 (name is ours)
u32 IFlowRecept::s_ProcessArg0;
// 0x0095EF0C (name is ours)
u32 IFlowRecept::s_ProcessArg1;
// 0x0095EF10 (name is ours)
oml::framework::Process* IFlowRecept::s_pProcess;

// 0x005D5CFC (name is ours)
s32 IFlowRecept::Event100(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    s32 result = RESULT_NONE;
    u32 param;
    node.GetParam(&param);
    switch (m_EventState) {
    case 0: {
        if (param == 0) {
            break;
        }
        oml::framework::Process* parent = oml::framework::ProcessManager::Get()->FindProcess(
            static_cast<oml::framework::ProcessIndex>(PROC_SCRIPT_BASE), NULL);
        if (parent == NULL) {
            break;
        }
        FlowProcessArg arg;
        InitFlowProcessArg(&arg);
        arg.param = param;
        arg.arg0 = s_ProcessArg0;
        arg.arg1 = s_ProcessArg1;
        s_ProcessArg0 = 0;
        s_ProcessArg1 = 0;
        g_FlowProcessArg = arg;
        oml::framework::Process* process = ProcMgr::Create(PROC_BS_COMMON_DIALOG, parent, 0, static_cast<ProcType>(0));
        if (process != NULL) {
            s_pProcess = process;
            m_EventState++;
            result = RESULT_WAIT;
        }
        break;
    }
    case 1:
        result = RESULT_WAIT;
        if (s_pProcess != NULL && s_pProcess->m_State != 0) {
            m_EventState++;
            s_pProcess = NULL;
        }
        break;
    case 2:
        result = RESULT_WAIT;
        if (!Func251930()) {
            result = RESULT_DONE;
        }
        break;
    }
    return result;
}

// 0x005D9B34 (name is ours)
s32 IFlowRecept::BranchRandom(u32* index, const libms::FlowNodeBranch& node)
{
    *index = GetRandom(node.GetCaseNum());
    return RESULT_DONE;
}

// 0x005D9B54 slot 0x14 (name is ours)
s32 IFlowRecept::OnEntry(const libms::FlowNodeEntry& node)
{
    (void)node;
    return RESULT_DONE;
}

// 0x005D9B5C slot 0x10 (name is ours)
s32 IFlowRecept::OnEvent(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    s32 result = RESULT_NONE;
    u16 id = node.GetId();
    for (const EventHandler* handler = s_EventHandlers; handler < s_EventHandlers + EVENT_HANDLER_NUM; handler++) {
        if (handler->id == id) {
            result = (this->*handler->func)(flow, node);
            if (result != RESULT_NONE) {
                return result;
            }
            break;
        }
    }
    if (g_SvIsSaveMemoryLocked) {
        return result;
    }
    SvTownFlags* townFlags = &GetGardenData()->townData.townFlags;
    result = RESULT_NONE;
    switch (node.GetId()) {
    case 76:
        townFlags->SetFlag(node.GetParam(), true);
        return RESULT_DONE;
    case 77:
        townFlags->SetFlag(node.GetParam(), false);
        return RESULT_DONE;
    case 78:
        townFlags->SetValue(35, node.GetParam());
        return RESULT_DONE;
    case 79:
        townFlags->SetValue(56, node.GetParam());
        return RESULT_DONE;
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return result;
    }
    SvPlayerFlags* playerFlags = &player->playerFlags;
    switch (node.GetId()) {
    case 49:
        playerFlags->SetValue(0, node.GetParam());
        result = RESULT_DONE;
        break;
    case 50:
        playerFlags->SetFlag(node.GetParam(), true);
        result = RESULT_DONE;
        break;
    case 51:
        playerFlags->SetFlag(node.GetParam(), false);
        result = RESULT_DONE;
        break;
    case 52:
        playerFlags->SetValue(67, node.GetParam());
        result = RESULT_DONE;
        break;
    case 53:
        playerFlags->SetValue(80, node.GetParam());
        result = RESULT_DONE;
        break;
    case 54:
        playerFlags->SetValue(111, node.GetParam());
        result = RESULT_DONE;
        break;
    case 55:
        playerFlags->SetValue(154, node.GetParam());
        result = RESULT_DONE;
        break;
    case 56:
        playerFlags->SetValue(171, node.GetParam());
        result = RESULT_DONE;
        break;
    case 57:
        playerFlags->SetValue(172, node.GetParam());
        result = RESULT_DONE;
        break;
    case 58:
        playerFlags->SetValue(173, node.GetParam());
        result = RESULT_DONE;
        break;
    case 59:
        playerFlags->SetValue(169, node.GetParam());
        result = RESULT_DONE;
        break;
    case 60:
        playerFlags->SetValue(198, node.GetParam());
        result = RESULT_DONE;
        break;
    }
    if (result != RESULT_NONE) {
        return result;
    }
    u8* playerData = player->unk_0x8F77;
    switch (node.GetId()) {
    case 61:
        Func2C1304(playerData, node.GetParam(), true);
        return RESULT_DONE;
    case 62:
        Func2C1304(playerData, node.GetParam(), false);
        return RESULT_DONE;
    case 63: {
        s32 param = node.GetParam();
        u32 value = (playerData[7] & 0x1F) + param;
        if (value > 30) {
            value = param > 0 ? 30 : 0;
        }
        playerData[7] = (playerData[7] & ~0x1F) | (value & 0x1F);
        return RESULT_DONE;
    }
    }
    u8* playerByte = &player->unk_0x8F99;
    switch (node.GetId()) {
    case 89:
        switch (node.GetParam()) {
        case 0:
            *playerByte |= 1;
            break;
        case 1:
            *playerByte |= 2;
            break;
        case 2:
            *playerByte |= 4;
            break;
        }
        return RESULT_DONE;
    case 90:
        switch (node.GetParam()) {
        case 0:
            *playerByte &= ~1;
            break;
        case 1:
            *playerByte &= ~2;
            break;
        case 2:
            *playerByte &= ~4;
            break;
        }
        return RESULT_DONE;
    }
    VisitorLocalInfo* visitor = VisitorLocalInfo::s_pInstance;
    if (visitor->m_HasFlags) {
        VisitorLocalInfo::Flags* flags = visitor->GetFlags();
        switch (node.GetId()) {
        case 64:
            Func2BBDE8(flags, node.GetParam(), true);
            return RESULT_DONE;
        case 65:
            Func2BBDE8(flags, node.GetParam(), false);
            return RESULT_DONE;
        }
    }
    if (visitor->m_HasByte41) {
        u8* byte41 = visitor->GetByte41();
        switch (node.GetId()) {
        case 80:
            if (node.GetParam() == 0) {
                *byte41 |= 1;
            }
            return RESULT_DONE;
        case 81:
            if (node.GetParam() == 0) {
                *byte41 &= ~1;
            }
            return RESULT_DONE;
        }
    }
    if (visitor->m_HasByte42) {
        u8* byte42 = visitor->GetByte42();
        switch (node.GetId()) {
        case 91:
            switch (node.GetParam()) {
            case 0:
                *byte42 |= 1;
                break;
            case 1:
                *byte42 |= 2;
                break;
            case 2:
                *byte42 |= 4;
                break;
            case 3:
                *byte42 |= 8;
                break;
            case 4:
                *byte42 |= 16;
                break;
            }
            return RESULT_DONE;
        case 92:
            switch (node.GetParam()) {
            case 0:
                *byte42 &= ~1;
                break;
            case 1:
                *byte42 &= ~2;
                break;
            case 2:
                *byte42 &= ~4;
                break;
            case 3:
                *byte42 &= ~8;
                break;
            case 4:
                *byte42 &= ~16;
                break;
            }
            return RESULT_DONE;
        }
    }
    return result;
}

// 0x005DA1F4 (name is ours)
s32 IFlowRecept::BranchWeighted(u32* index, const libms::FlowNodeBranch& node)
{
    u32 num = node.GetCaseNum();
    u16 low;
    u16 high;
    node.GetParams(&low, &high);
    if (low != 0 || high != 0) {
        *index = low <= static_cast<s32>(GetRandom(low + high)) ? 1 : 0;
    } else {
        *index = GetRandom(num);
    }
    return RESULT_DONE;
}

// 0x005DA26C slot 0x24 (name is ours)
void IFlowRecept::OnFlowEnd()
{
    // empty (in the original too)
}

// 0x005DA278 slot 0x0C (name is ours)
s32 IFlowRecept::OnBranch(u32* index, const libms::FlowNodeBranch& node)
{
    u16 id = node.GetId();
    for (const BranchHandler* handler = s_BranchHandlers; handler < s_BranchHandlers + BRANCH_HANDLER_NUM; handler++) {
        if (handler->id == id) {
            if (handler->func != NULL) {
                s32 result = (this->*handler->func)(index, node);
                if (result != RESULT_NONE) {
                    return result;
                }
            }
            break;
        }
    }
    SvTownFlags* townFlags = &GetGardenPlus()->data.townData.townFlags;
    switch (node.GetId()) {
    case 119:
        return SelectCase(index, townFlags->GetValue(node.GetParam()) == 1);
    case 120:
        return SelectCase(index, townFlags->GetValue(5) >= node.GetParam());
    case 121:
        return SelectCase(index, townFlags->GetValue(31) >= node.GetParam());
    case 122:
        return SelectCase(index, townFlags->GetValue(35) >= node.GetParam());
    case 123:
        return SelectCase(index, townFlags->GetValue(56) >= node.GetParam());
    }
    VisitorLocalInfo* visitor = VisitorLocalInfo::s_pInstance;
    townFlags = visitor->GetTownFlags();
    switch (node.GetId()) {
    case 124:
        return SelectCase(index, townFlags->GetValue(node.GetParam()) == 1);
    case 125:
        return SelectCase(index, townFlags->GetValue(5) >= node.GetParam());
    case 126:
        return SelectCase(index, townFlags->GetValue(31) >= node.GetParam());
    case 127:
        return SelectCase(index, townFlags->GetValue(35) >= node.GetParam());
    case 128:
        return SelectCase(index, townFlags->GetValue(56) >= node.GetParam());
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return RESULT_NONE;
    }
    SvPlayerFlags* playerFlags = &player->playerFlags;
    switch (node.GetId()) {
    case 129:
        return SelectCase(index, playerFlags->GetValue(node.GetParam()) == 1);
    case 130:
        return SelectCase(index, playerFlags->GetValue(0) >= node.GetParam());
    case 131:
        return SelectCase(index, playerFlags->GetValue(8) >= node.GetParam());
    case 132:
        return SelectCase(index, playerFlags->GetValue(42) >= node.GetParam());
    case 133:
        return SelectCase(index, playerFlags->GetValue(67) >= node.GetParam());
    case 134:
        return SelectCase(index, playerFlags->GetValue(80) >= node.GetParam());
    case 135:
        return SelectCase(index, playerFlags->GetValue(111) >= node.GetParam());
    case 136:
        return SelectCase(index, playerFlags->GetValue(117) >= node.GetParam());
    case 137:
        return SelectCase(index, playerFlags->GetValue(154) >= node.GetParam());
    case 138:
        return SelectCase(index, playerFlags->GetValue(160) >= node.GetParam());
    case 139:
        return SelectCase(index, playerFlags->GetValue(169) >= node.GetParam());
    case 140:
        return SelectCase(index, playerFlags->GetValue(171) >= node.GetParam());
    case 141:
        return SelectCase(index, playerFlags->GetValue(172) >= node.GetParam());
    case 142:
        return SelectCase(index, playerFlags->GetValue(173) >= node.GetParam());
    case 143:
        return SelectCase(index, playerFlags->GetValue(198) >= node.GetParam());
    }
    const u8* playerData = player->unk_0x8F77;
    switch (node.GetId()) {
    case 144:
        return SelectCase(index, Func71E9A4(playerData, node.GetParam()) == 1);
    case 145:
        return SelectCase(index, Func71E9A4(playerData, 56) >= node.GetParam());
    }
    const u8* playerByte = &player->unk_0x8F99;
    if (node.GetId() == 153) {
        switch (node.GetParam()) {
        case 0:
            return SelectCase(index, (*playerByte & 1) != 0);
        case 1:
            return SelectCase(index, (*playerByte & 2) != 0);
        case 2:
            return SelectCase(index, (*playerByte & 4) != 0);
        default:
            return SelectCase(index, false);
        }
    }
    if (visitor->m_HasFlags) {
        const VisitorLocalInfo::Flags* flags = visitor->GetFlags();
        if (node.GetId() == 146) {
            return SelectCase(index, GetVisitorFlag(flags, node.GetParam()) == 1);
        }
    }
    if (visitor->m_HasByte41) {
        const u8* byte41 = visitor->GetByte41();
        if (node.GetId() == 151) {
            return SelectCase(index, node.GetParam() == 0 && (*byte41 & 1) != 0);
        }
    }
    if (visitor->m_HasByte42) {
        const u8* byte42 = visitor->GetByte42();
        if (node.GetId() == 154) {
            switch (node.GetParam()) {
            case 0:
                return SelectCase(index, (*byte42 & 1) != 0);
            case 1:
                return SelectCase(index, (*byte42 & 2) != 0);
            case 2:
                return SelectCase(index, (*byte42 & 4) != 0);
            case 3:
                return SelectCase(index, (*byte42 & 8) != 0);
            case 4:
                return SelectCase(index, (*byte42 & 16) != 0);
            default:
                return SelectCase(index, false);
            }
        }
    }
    return RESULT_NONE;
}

// 0x005DACBC (name is ours)
s32 IFlowRecept::BranchMonth(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    if (m_pOwner != NULL) {
        u32 month = m_pOwner->m_pDate->month - 1;
        if (month < 12) {
            value = month;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DAD00 (name is ours)
s32 IFlowRecept::BranchChoice(u32* index, const libms::FlowNodeBranch& node)
{
    s32 num = node.GetCaseNum();
    s32 cursor = m_pTalkRecept->m_pOwner->m_pChoice->GetCursor();
    if (cursor >= 0 && num <= cursor) {
        cursor = num - 1;
    }
    *index = cursor;
    return RESULT_DONE;
}

// 0x005DAD48 (name is ours)
s32 IFlowRecept::BranchSeason(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    if (m_pOwner != NULL) {
        value = time::GetSeason(*m_pOwner->m_pDate);
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DAD7C (name is ours)
s32 IFlowRecept::Branch155(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    s32 value = g_FlowProcessArg.result;
    if (value < 0 || value >= 2) {
        return RESULT_NONE;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DADA4 (name is ours)
s32 IFlowRecept::Branch21(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    if (Func630C3C()) {
        value = 7;
    } else {
        switch (Func62FB14()) {
        case 1:
            value = 1;
            break;
        case 2:
            value = 2;
            break;
        case 3:
            value = 3;
            break;
        case 4:
            value = 4;
            break;
        case 5:
            value = 5;
            break;
        case 6:
            value = 6;
            break;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DAE24 (name is ours)
s32 IFlowRecept::EventStartMessage(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    if (m_pOwner == NULL || m_pOwner->m_pCurrent == NULL) {
        return RESULT_NONE;
    }
    return m_pOwner->m_pCurrent->StartMessage(flow, node);
}

// 0x005DAE48 (name is ours)
s32 IFlowRecept::EventStartFlow(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    FlowSeq* current;
    if (m_pOwner == NULL || (current = m_pOwner->m_pCurrent) == NULL) {
        return RESULT_NONE;
    }
    s32 result = RESULT_NONE;
    if (node.GetKind() == libms::FlowNodeEvent::KIND_CALL) {
        u32 offset;
        node.GetParam(&offset);
        const char* file = static_cast<const char*>(flow->GetData(offset));
        if (file != NULL) {
            FlowSystem* owner = current->m_pOwner;
            if (owner->Push()) {
                owner->m_pCurrent->Start(file, static_cast<const char*>(NULL));
            }
            result = RESULT_DONE;
        }
    }
    return result;
}

// 0x005DAEE4 slot 0x18 (name is ours)
bool IFlowRecept::IsContinue()
{
    return true;
}

// 0x005DAEEC (name is ours)
s32 IFlowRecept::Branch41(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 1;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid()) {
        if (Func76D45C(Func71B33C(&player->playerAppearance.playerOutfit))) {
            value = 0;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DAF40 (name is ours)
s32 IFlowRecept::BranchPartOfDay(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    if (m_pOwner != NULL) {
        value = time::GetPartOfDay(*m_pOwner->m_pDate);
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DAF74 (name is ours)
s32 IFlowRecept::Event0(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    if (m_pOwner == NULL) {
        return RESULT_NONE;
    }
    u32 value;
    node.GetParam(&value);
    if (value != 0) {
        SvPlayer* player = SvGetCurrentPlayer();
        if (player != NULL && player->playerInfo.IsValid()) {
            if (node.GetId() != 0) {
                Func2C02C8(&player->inventory, value);
                Func6B9468(GetCensus(), value);
            } else {
                Func2C00A8(&player->inventory, value);
                GetCensus()->bellsEarned.Add(value);
            }
        }
    }
    return RESULT_DONE;
}

// 0x005DB024 (name is ours)
s32 IFlowRecept::EventCallEntry(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    if (m_pOwner == NULL || m_pOwner->m_pCurrent == NULL) {
        return RESULT_NONE;
    }
    return m_pOwner->m_pCurrent->CallEntry(flow, node);
}

// 0x005DB048 (name is ours)
s32 IFlowRecept::EventWait(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    s32 result = RESULT_WAIT;
    u32 frames = node.GetParam();
    FlowSeq* current = m_pOwner != NULL ? m_pOwner->m_pCurrent : NULL;
    u32 count = current != NULL ? current->m_Count : 0;
    if (frames <= count * 2) {
        result = RESULT_DONE;
    }
    return result;
}

// 0x005DB088 (name is ours)
s32 IFlowRecept::BranchHour(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u16 start;
    u16 end;
    node.GetParams(&start, &end);
    u32 value = 0;
    if (m_pOwner != NULL) {
        s32 hour = m_pOwner->m_pDate->hour;
        if (end < start) {
            if (hour < start && hour >= end) {
                value = 1;
            }
        } else if (hour < start || hour >= end) {
            value = 1;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DB114 (name is ours)
s32 IFlowRecept::BranchWeekday(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    if (m_pOwner != NULL) {
        switch (static_cast<u8>(m_pOwner->m_pDate->dayOfWeek)) {
        case 1:
            break;
        case 2:
            value = 1;
            break;
        case 3:
            value = 2;
            break;
        case 4:
            value = 3;
            break;
        case 5:
            value = 4;
            break;
        case 6:
            value = 5;
            break;
        default:
            value = 6;
            break;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DB190 (name is ours)
s32 IFlowRecept::Branch45(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    *index = 1;
    return RESULT_DONE;
}

// 0x005DB1AC (name is ours)
s32 IFlowRecept::Branch106(u32* index, const libms::FlowNodeBranch& node)
{
    s32 value = Func23C368(node.GetParam());
    if (value == 2) {
        return RESULT_NONE;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DB1D8 (name is ours)
s32 IFlowRecept::BranchNookling(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 param;
    node.GetParam(&param);
    u32 value = 0;
    switch (GetGardenPlus()->data.townData.nooklingState) {
    case 1:
        value = 1;
        break;
    case 2:
        value = 2;
        break;
    case 3:
        value = 3;
        break;
    case 4:
        value = 4;
        break;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DB258 (name is ours)
s32 IFlowRecept::Branch107(u32* index, const libms::FlowNodeBranch& node)
{
    return Func606170(g_pObject94FD54, index, node);
}

// 0x005DB268 (name is ours)
void IFlowRecept::SetSelectedPocketItem(const SvFgName* item)
{
    SvPlayer* player = SvGetCurrentPlayer();
    u8 slot = BsMenuMgr::s_SelectedSlot;
    if (slot < SvPlayerInventory::SIZE && player != NULL) {
        Func2BFF98(&player->inventory, slot, item, 0, 1);
    }
}

// 0x005DB2B0 (name is ours)
s32 IFlowRecept::EventMenu(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    if (node.GetKind() != EVENT_KIND_MENU) {
        return RESULT_NONE;
    }
    u32 param;
    node.GetParam(&param);
    switch (param) {
    case 0:
        return OpenTalkMenu(46);
    case 15:
        return OpenTalkMenu(48);
    case 19:
        return OpenTalkMenu(47);
    case 20:
        return OpenTalkMenu(49);
    case 21:
        if (m_EventState == 0) {
            BsMenuMgr::Func6D44B0();
            m_EventState = 1;
            return RESULT_WAIT;
        }
        return WaitMenuClosed(m_EventState);
    case 16:
        if (m_EventState == 0) {
            TalkSystem* talk = m_pTalkRecept->m_pOwner;
            if (talk != NULL) {
                talk->RequestQuit(0, false, true);
            }
            BsMenuMgr::Func6D3274();
            m_EventState = 1;
            return RESULT_WAIT;
        }
        if (m_EventState == 1) {
            return BsMenuMgr::s_Result == 2 ? RESULT_WAIT : RESULT_DONE;
        }
        return RESULT_NONE;
    case 3:
    case 4:
    case 5:
    case 6: {
        void* process = BsMenuMgr::s_pInstance->m_pProcess;
        if (m_EventState == 0) {
            if (process != NULL) {
                BsMenuMgr::Func6D2744();
                m_OpenCount++;
            } else {
                BsMenuMgr::s_Flags |= 0x200;
                BsMenuMgr::s_Unknown950D25 = 1;
                m_OpenCount = 1;
            }
            m_EventState = 1;
            return RESULT_WAIT;
        }
        if (m_EventState == 1) {
            if (process == NULL || BsMenuMgr::Func6D2718()) {
                return RESULT_DONE;
            }
            return RESULT_WAIT;
        }
        return RESULT_NONE;
    }
    case 17:
        if (m_EventState == 0) {
            BsMenuMgr::Func6D40D4();
            m_EventState = 1;
            return RESULT_WAIT;
        }
        return WaitMenuClosed(m_EventState);
    case 23: {
        TalkSystem* talk = m_pTalkRecept->m_pOwner;
        switch (m_EventState) {
        case 0:
            if (talk != NULL && talk->m_State != TalkSystem::STATE_CLOSED) {
                talk->RequestQuit(0, false, true);
            }
            m_EventState = 1;
            break;
        case 1:
            if (talk != NULL && talk->m_State == TalkSystem::STATE_CLOSED) {
                BsMenuMgr::Func6D3F60();
                m_EventState = 2;
            }
            break;
        case 2:
            if (BsMenuMgr::s_Result != 2) {
                m_EventState = 3;
                return RESULT_DONE;
            }
            break;
        }
        return RESULT_WAIT;
    }
    case 24:
        if (m_EventState == 0) {
            BsMenuMgr::Func6D2A08(66);
            m_EventState = 1;
            return RESULT_WAIT;
        }
        return WaitMenuClosed(m_EventState);
    case 27:
        if (m_EventState == 0) {
            BsMenuMgr::Func6D2A08(67);
            m_EventState = 1;
            return RESULT_WAIT;
        }
        return WaitMenuClosed(m_EventState);
    case 36:
        if (m_EventState == 0) {
            BsMenuMgr::Func6D3DF8();
            m_EventState = 1;
            return RESULT_WAIT;
        }
        if (m_EventState == 1) {
            if (BsMenuMgr::IsIdle()) {
                BsMenuMgr::s_Unknown950D42 = 0;
                return RESULT_DONE;
            }
            return RESULT_WAIT;
        }
        return RESULT_NONE;
    case 1:
    case 2:
    case 7:
    case 8:
    case 9:
    case 11:
    case 13:
    case 14:
    case 34:
    case 35: {
        TalkSystem* talk = m_pTalkRecept->m_pOwner;
        if (m_EventState == 0) {
            WordFix<33> word;
            u32 kind = 11;
            bool hasWord = false;
            Insert* insert = talk != NULL ? talk->m_pPhrase->GetInsert() : NULL;
            switch (param) {
            case 11:
                kind = 9;
                break;
            case 13:
                kind = 5;
                break;
            case 14:
                kind = 6;
                break;
            case 34:
                kind = 8;
                break;
            case 35:
                kind = 7;
                break;
            case 1:
                kind = 0;
                if (insert != NULL) {
                    insert->ClearTag5_0();
                }
                break;
            case 2:
                kind = 1;
                if (insert != NULL) {
                    insert->ClearTag5_6();
                }
                break;
            case 7:
                kind = 2;
                if (insert != NULL) {
                    insert->ClearWord238();
                }
                hasWord = vf_0x30(&word);
                break;
            case 8:
                kind = 3;
                if (insert != NULL) {
                    insert->ClearTag5_7();
                }
                hasWord = vf_0x2C(&word);
                break;
            case 9:
                kind = 4;
                if (insert != NULL) {
                    insert->ClearExtra();
                }
                hasWord = vf_0x34(&word);
                break;
            }
            if (hasWord) {
                Func1FE478(kind, &word);
            }
            Func1FE400(kind);
            m_EventState = 1;
            return RESULT_WAIT;
        }
        if (m_EventState == 1) {
            if (g_pObject951138->isDone) {
                Func1FE458();
                return RESULT_DONE;
            }
            return RESULT_WAIT;
        }
        return RESULT_NONE;
    }
    case 25:
    case 33:
        if (m_EventState == 0) {
            if (param == 33 ? BsMenuMgr::Func6D3C68() : BsMenuMgr::Func6D3C34()) {
                m_EventState = 1;
            }
            return RESULT_WAIT;
        }
        if (m_EventState == 1) {
            u8 menuResult = BsMenuMgr::s_Result;
            return menuResult == 0 || menuResult == 1 ? RESULT_DONE : RESULT_WAIT;
        }
        return RESULT_NONE;
    case 18:
        if (m_EventState == 0) {
            BsMenuMgr::Func6D33D8(3);
            m_EventState = 1;
            return RESULT_WAIT;
        }
        if (m_EventState == 1) {
            return BsMenuMgr::s_Result == 2 ? RESULT_WAIT : RESULT_DONE;
        }
        return RESULT_DONE;
    case 10:
        return RESULT_DONE;
    }
    return RESULT_NONE;
}

// 0x005DB9EC (name is ours)
void IFlowRecept::ClearSelectedPocketItem()
{
    SvPlayer* player = SvGetCurrentPlayer();
    u8 slot = BsMenuMgr::s_SelectedSlot;
    if (slot < SvPlayerInventory::SIZE && player != NULL) {
        Func2BFF98(&player->inventory, slot, &SvFgName::s_Empty, 0, 1);
    }
}

// 0x005DBA34 (name is ours)
s32 IFlowRecept::BranchIsNew3ds(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, nn::os::CTR::IsRunOnSnake());
}

// 0x005DBA58 (name is ours)
s32 IFlowRecept::BranchConnection(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value;
    if (netgame::IsMultiPlayer()) {
        value = 0;
    } else if (netgame::NetGameMgr::s_pInstance->m_ConnectMgr.IsConnected(static_cast<netgame::ConnectMgr::Connection>(2)) ||
               netgame::NetGameMgr::s_pInstance->m_ConnectMgr.IsConnected(static_cast<netgame::ConnectMgr::Connection>(3))) {
        value = 1;
    } else {
        value = 2;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DBACC (name is ours)
s32 IFlowRecept::BranchBirthday(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 1;
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL) {
        const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
        if (player->birthDay == 29 && player->birthMonth == 2 && !event::IsLeapYear(date->year)) {
            if (date->day == 28 && date->month == 2) {
                value = 0;
            }
        } else if (player->birthMonth == date->month && player->birthDay == date->day) {
            value = 0;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DBB6C (name is ours)
s32 IFlowRecept::Branch29(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, netgame::GetNetgameType() == netgame::TYPE_DREAM);
}

// 0x005DBB94 (name is ours)
s32 IFlowRecept::EventCloseTalk(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    (void)node;
    switch (m_EventState) {
    case 0: {
        TalkSystem* talk = m_pTalkRecept->m_pOwner;
        if (talk != NULL && talk->CanQuit(false)) {
            talk->RequestQuit(0, false, true);
        }
        m_EventState++;
        return RESULT_WAIT;
    }
    case 1: {
        TalkSystem* talk = m_pTalkRecept->m_pOwner;
        if (talk == NULL || talk->m_State == TalkSystem::STATE_CLOSED) {
            m_EventState = 2;
        }
        return RESULT_WAIT;
    }
    }
    return RESULT_DONE;
}

// 0x005DBC48 (name is ours)
s32 IFlowRecept::Event13(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    u32 param;
    node.GetParam(&param);
    u32 value = 0;
    switch (param) {
    case 0:
        value = 6;
        break;
    case 1:
        value = 7;
        break;
    case 2:
        value = 3;
        break;
    case 3:
        value = 4;
        break;
    case 4:
        value = 2;
        break;
    case 5:
        value = 5;
        break;
    case 6:
        value = 1;
        break;
    case 7:
        value = 80;
        break;
    }
    Func5CE494(&GetGardenData()->strcData, value);
    return RESULT_DONE;
}

// 0x005DBCE8 (name is ours)
void IFlowRecept::ClearMarkedPocketItems()
{
    SvPlayer* player = SvGetCurrentPlayer();
    u16 mask = BsMenuMgr::s_PocketItemMask;
    if (player == NULL) {
        return;
    }
    for (u32 i = 0; i < SvPlayerInventory::SIZE; i++) {
        if (mask & (1 << i)) {
            Func2BFF98(&player->inventory, i, &SvFgName::s_Empty, 0, 1);
        }
    }
}

// 0x005DBD58 slot 0x08 (name is ours)
bool IFlowRecept::LoadFlow(FlowFile* file, const sead::SafeStringBase<char>& name, sead::Heap* heap)
{
    if (heap != NULL) {
        file->m_pName = &name;
        sead::StringFormatter formatter(&file->m_Path);
        formatter.append("Script/Flow/%s.msbf").appendValue(name).flush();
        file->m_pHeap = heap;
    }
    return file->m_Loader.Load(file->m_Path, file->m_pHeap, 0);
}

// 0x005DBDF4 (name is ours)
s32 IFlowRecept::Branch32(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, Func5C3750());
}

// 0x005DBE18 (name is ours)
s32 IFlowRecept::BranchStage(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 param;
    node.GetParam(&param);
    u32 value = 1;
    switch (param) {
    case 0:
        if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
            value = 0;
        }
        break;
    case 1:
        return RESULT_NONE;
    case 2:
        value = 0;
        if (stage::IsKind(stage::KIND_TOUR, stage::GetCurrent())) {
            value = 1;
        }
        break;
    default: {
        stage::Name name = stage::GetCurrent();
        if (stage::IsKind(stage::KIND_CLUB, name) || stage::IsKind(stage::KIND_CAFE_ENVIRONMENT, name)) {
            value = 0;
            break;
        }
        switch (name) {
        case stage::NAME_DOWNTOWN:
        case stage::NAME_RECYCLE_SHOP:
        case stage::NAME_CONVENIENCE_STORE:
        case stage::NAME_SUPERMARKET:
        case stage::NAME_HOME_CENTER:
        case stage::NAME_DEPARTMENT_STORE_1F:
        case stage::NAME_TAILOR:
        case stage::NAME_MUSEUM_FISH_0:
        case stage::NAME_MUSEUM_FISH_1:
        case stage::NAME_MUSEUM_FISH_2:
        case stage::NAME_MUSEUM_INSECT_0:
        case stage::NAME_MUSEUM_INSECT_1:
        case stage::NAME_MUSEUM_INSECT_2:
        case stage::NAME_MUSEUM_FOSSIL_0:
        case stage::NAME_MUSEUM_FOSSIL_1:
        case stage::NAME_MUSEUM_FOSSIL_2:
        case stage::NAME_MUSEUM_PICTURE_0:
        case stage::NAME_MUSEUM_PICTURE_1:
            value = 0;
            break;
        default:
            break;
        }
        break;
    }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DBF48 (name is ours)
s32 IFlowRecept::BranchChoice12(u32* index, const libms::FlowNodeBranch& node)
{
    s32 num = node.GetCaseNum();
    s32 cursor = m_pTalkRecept->m_pOwner->m_pChoice->GetCursor();
    if (cursor >= 0 && num <= cursor) {
        cursor = num - 1;
    }
    *index = cursor;
    return RESULT_DONE;
}

// 0x005DBF90 (name is ours)
s32 IFlowRecept::BranchChoiceOdd(u32* index, const libms::FlowNodeBranch& node)
{
    s32 num = node.GetCaseNum();
    s32 cursor = m_pTalkRecept->m_pOwner->m_pChoice->GetCursor();
    if (cursor >= 0 && num <= cursor) {
        cursor = num - 1;
    }
    *index = cursor;
    return RESULT_DONE;
}

// 0x005DBFD8 (name is ours)
s32 IFlowRecept::Branch34(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 param;
    node.GetParam(&param);
    if (param == 0) {
        return RESULT_NONE;
    }
    u32 value = 1;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid() && param <= Func71E70C(&player->inventory)) {
        value = 0;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC050 (name is ours)
s32 IFlowRecept::Branch54(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    return SelectCase(index, (g_Byte0xAAEAD4 & 0x20) != 0);
}

// 0x005DC088 (name is ours)
s32 IFlowRecept::BranchBeeSting(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 2;
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL && player->playerFlags.hasBeeSting) {
        value = Func713404(player) ? 1 : 0;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC0E0 (name is ours)
s32 IFlowRecept::BranchGender(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL && player->playerInfo.IsValid() && (player->playerInfo.playerData.gender & 1)) {
        value = 1;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC13C (name is ours)
s32 IFlowRecept::Branch43(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid()) {
        u32 result = Func713798(&player->playerAppearance);
        if (result >= 8) {
            result = 7;
        }
        value = result;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC190 (name is ours)
s32 IFlowRecept::BranchMeowCoupons(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 param;
    node.GetParam(&param);
    u32 value = 1;
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL && Func3037DC(&player->meowCoupons) != 0) {
        value = 0;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC1E8 (name is ours)
s32 IFlowRecept::Branch15(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    return SelectCase(index, Func758CDC(&GetGardenPlus()->data));
}

// 0x005DC21C (name is ours)
s32 IFlowRecept::BranchLeapDay(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    return SelectCase(index, date->month == 2 && date->day == 29);
}

// 0x005DC24C (name is ours)
s32 IFlowRecept::BranchIsVillage(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
        value = 1;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC284 (name is ours)
s32 IFlowRecept::Branch25(u32* index, const libms::FlowNodeBranch& node)
{
    u32 param;
    node.GetParam(&param);
    u32 value = Func118C8C(GetUnknownData290A4()) - 5;
    if (value > 5) {
        value = 0;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC2C0 (name is ours)
s32 IFlowRecept::EventOpenMenu(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    s32 result = RESULT_WAIT;
    if (node.GetKind() != EVENT_KIND_MENU) {
        return result;
    }
    u32 param;
    node.GetParam(&param);
    if (m_EventState == 0) {
        u32 arg = 0;
        if (param < MENU_NUM) {
            if (m_MenuFilter != NULL) {
                arg = BsMenuMgr::Func6D2AC8(m_MenuFilter);
            } else if (s_Menus[param].filter != NULL) {
                arg = BsMenuMgr::Func6D2AC8(s_Menus[param].filter);
            }
            if (m_MenuArg != 0) {
                BsMenuMgr::s_Unknown950D94 = m_MenuArg;
            }
        }
        u8 menu = param < MENU_NUM ? s_Menus[param].menu : 9;
        if (param == 1 || param == 7 || param == 8 || param == 24 || param == 34 || param == 19 || param == 33) {
            BsMenuMgr::Func6D4724(arg, menu, BsMenuMgr::s_MenuParam);
        } else {
            BsMenuMgr::Func6D3D90(arg, menu, BsMenuMgr::s_MenuParam);
        }
        m_EventState = 1;
    } else if (m_EventState == 1 && BsMenuMgr::IsIdle()) {
        BsMenuMgr::s_Unknown950D42 = 0;
        result = RESULT_DONE;
    }
    return result;
}

// 0x005DC408 (name is ours)
s32 IFlowRecept::Event6(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    if (m_EventState == 0) {
        u32 arg = vf_0x28(node);
        if (arg == 0) {
            return RESULT_NONE;
        }
        BsMenuMgr::Func6D322C(arg, 0, 69);
        m_EventState = 1;
        TalkSystem* talk = m_pTalkRecept->m_pOwner;
        if (talk != NULL) {
            talk->RequestQuit(0, false, true);
        }
        return RESULT_WAIT;
    }
    if (m_EventState == 1) {
        return BsMenuMgr::IsIdle() ? RESULT_DONE : RESULT_WAIT;
    }
    return RESULT_NONE;
}

// 0x005DC4C0 (name is ours)
s32 IFlowRecept::Branch53(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 1;
    if (Func5C7BDC() != NULL && Func717120() == 1) {
        value = 0;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC4F8 (name is ours)
s32 IFlowRecept::Branch46(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    SvPlayer* player = g_pSvCurrentPlayer;
    return SelectCase(index, player != NULL && player->playerInfo.IsValid() && player->playerFlags.playerSetNameAndTownName);
}

// 0x005DC550 (name is ours)
s32 IFlowRecept::Branch28(u32* index, const libms::FlowNodeBranch& node)
{
    switch (node.GetParam()) {
    case 0:
        return SelectCase(index, Func118EAC(GetUnknownData290A4(), 1) != -1);
    case 1: {
        u8* data = GetUnknownData290A4();
        s32 entry = Func118FCC(data);
        bool isFirst = false;
        if (entry != -1) {
            const u8* found = static_cast<const u8*>(Func300928(data, entry));
            if (found != NULL) {
                isFirst = ((*reinterpret_cast<const u32*>(found + 0x2510) << 21) >> 30) != 2;
            }
        }
        return SelectCase(index, isFirst);
    }
    case 2: {
        const u8* flags = reinterpret_cast<const u8*>(&GetGardenPlus()->data.townData.townFlags);
        return SelectCase(index, (flags[2] & 0xE0) != 0 || flags[3] != 0 || (flags[4] & 0x06) != 0 || (flags[14] & 0x10) != 0);
    }
    }
    return RESULT_NONE;
}

// 0x005DC6CC (name is ours)
s32 IFlowRecept::Branch13(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    *index = BsMenuMgr::s_Result == 0 ? 1 : 0;
    return RESULT_DONE;
}

// 0x005DC6F0 (name is ours)
s32 IFlowRecept::EventTransition(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    TalkSystem* talk = m_pTalkRecept->m_pOwner;
    switch (m_EventState) {
    case 0:
        if (talk != NULL && talk->CanQuit(false)) {
            talk->RequestQuit(0, false, true);
            m_EventState = 1;
        } else {
            m_EventState = 2;
        }
        break;
    case 1:
        if (talk != NULL && talk->m_State == TalkSystem::STATE_CLOSED) {
            m_EventState = 2;
        }
        break;
    case 2: {
        u32 entrance;
        node.GetParam(&entrance);
        stage::GetTransitionMgr()->TransitionByEntrance(entrance, 0, 0, 1, 1);
        m_EventState = 3;
        break;
    }
    }
    return RESULT_WAIT;
}

// 0x005DC7E8 (name is ours)
s32 IFlowRecept::EventSetNoRepeat(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    (void)node;
    m_pTalkRecept->m_pOwner->m_pSeq->m_RandomBranch.isNoRepeat = 1;
    return RESULT_DONE;
}

// 0x005DC804 (name is ours)
s32 IFlowRecept::Branch44(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 param;
    node.GetParam(&param);
    u32 value = 1;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid() && param <= Func72519C(&player->inventory)) {
        value = 0;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC86C (name is ours)
s32 IFlowRecept::BranchLeapDayBirthday(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 1;
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL && player->birthDay == 29 && player->birthMonth == 2) {
        value = 0;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC8C0 (name is ours)
s32 IFlowRecept::Branch40(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 param;
    node.GetParam(&param);
    u32 value = 1;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && player->playerInfo.IsValid()) {
        u32 kind = Func713768(&player->playerAppearance);
        if (kind - 17 <= 16) {
            if (param == kind - 17) {
                value = 0;
            }
        } else if (kind <= 16 && param == kind) {
            value = 0;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DC950 (name is ours)
s32 IFlowRecept::EventMenu2(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    if (node.GetKind() != EVENT_KIND_MENU) {
        return RESULT_NONE;
    }
    u32 param;
    node.GetParam(&param);
    switch (param) {
    case 0:
    case 1:
    case 2:
    case 8:
    case 9:
        if (m_EventState == 0) {
            BsMenuMgr::OpenMenu(param < 3 ? 55 + param : 59 + (param - 8), BsMenuMgr::s_MenuParam);
            m_EventState = 1;
            return RESULT_WAIT;
        }
        return WaitMenuClosed(m_EventState);
    case 3:
    case 4:
    case 5:
    case 6:
        if (m_EventState == 0) {
            u32 menu = param == 3 ? 58 : param == 4 ? 51 : param == 5 ? 50 : 53;
            BsMenuMgr::Func6D3F8C(menu, BsMenuMgr::s_MenuParam, 0);
            m_EventState = 1;
            return RESULT_WAIT;
        }
        return WaitMenuClosed(m_EventState);
    case 7:
        if (m_EventState == 0) {
            Func1FE400(10);
            m_EventState = 1;
            return RESULT_WAIT;
        }
        if (m_EventState == 1) {
            if (g_pObject951138->isDone) {
                Func1FE458();
                return RESULT_DONE;
            }
            return RESULT_WAIT;
        }
        return RESULT_NONE;
    }
    return RESULT_NONE;
}

// 0x005DCD28 (name is ours)
s32 IFlowRecept::Event94(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    switch (node.GetParam()) {
    case 0:
        Func23CC08(1);
        return RESULT_DONE;
    case 1:
        Func23CC08(0);
        return RESULT_DONE;
    }
    return RESULT_NONE;
}

// 0x005DCD74 (name is ours)
s32 IFlowRecept::EventClearNoRepeat(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    (void)node;
    m_pTalkRecept->m_pOwner->m_pSeq->m_RandomBranch.isNoRepeat = 0;
    return RESULT_DONE;
}

// 0x005DCD90 (name is ours)
s32 IFlowRecept::EventQuitAndWait(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    s32 result = RESULT_WAIT;
    FlowSeq* current = m_pOwner != NULL ? m_pOwner->m_pCurrent : NULL;
    if (current != NULL && current->m_Count == 0) {
        TalkSystem* talk = m_pTalkRecept->m_pOwner;
        if (talk != NULL && talk->CanQuit(false)) {
            talk->RequestQuit(0, false, true);
        }
    }
    u32 frames = node.GetParam();
    current = m_pOwner != NULL ? m_pOwner->m_pCurrent : NULL;
    u32 count = current != NULL ? current->m_Count : 0;
    if (frames <= count * 2) {
        result = RESULT_DONE;
    }
    return result;
}

// 0x005DCE44 (name is ours)
void IFlowRecept::SetStart(const char* file, const char* label)
{
    m_File.copy(sead::SafeStringBase<char>(file), -1);
    m_Label.copy(sead::SafeStringBase<char>(label), -1);
}

// 0x005DCE94 (name is ours)
s32 IFlowRecept::Branch157(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    *index = Func350D40() ? 1 : 0;
    return RESULT_DONE;
}

// 0x005DCEB4 (name is ours)
s32 IFlowRecept::Branch88(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    *index = BsMenuMgr::s_pInstance->m_pProcess != NULL ? 1 : 0;
    return RESULT_DONE;
}

// 0x005DCED8 (name is ours)
s32 IFlowRecept::Branch55(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    *index = 1;
    return RESULT_DONE;
}

// 0x005DCEF4 (name is ours)
s32 IFlowRecept::Branch36(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    if (Func5C7BDC() != NULL) {
        u32 result = Func717120();
        if (result < 10) {
            value = result;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DCF2C (name is ours)
s32 IFlowRecept::BranchPlayerCount(u32* index, const libms::FlowNodeBranch& node)
{
    u32 param;
    node.GetParam(&param);
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        SvPlayer* player = SvGetPlayer(static_cast<PlayerSaveNumber>(i));
        if (player != NULL && player->playerInfo.IsValid()) {
            count++;
        }
    }
    *index = count >= 1 && count <= 4 ? count - 1 : 0;
    return RESULT_DONE;
}

// 0x005DCFA8 slot 0x28
u32 IFlowRecept::vf_0x28(const libms::FlowNodeEvent& node)
{
    (void)node;
    return 0;
}

// 0x005DCFB0 (name is ours)
s32 IFlowRecept::Branch159(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, Func23CB00());
}

// 0x005DCFD4 (name is ours)
s32 IFlowRecept::Branch161(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, Func710100());
}

// 0x005DCFF8 (name is ours)
s32 IFlowRecept::EventMenu3(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    s32 result = RESULT_WAIT;
    if (node.GetKind() != EVENT_KIND_MENU) {
        return result;
    }
    u32 param;
    node.GetParam(&param);
    u32 menuParam = BsMenuMgr::s_MenuParam;
    switch (m_EventState) {
    case 0:
        switch (param) {
        case 0:
            BsMenuMgr::Func6D448C(78, menuParam);
            break;
        case 1:
            BsMenuMgr::Func6D448C(88, menuParam);
            break;
        case 2:
            BsMenuMgr::Func6D448C(93, menuParam);
            break;
        case 3:
            BsMenuMgr::Func6D448C(89, menuParam);
            break;
        case 4:
            BsMenuMgr::Func6D448C(83, menuParam);
            break;
        case 5:
            BsMenuMgr::Func6D448C(90, menuParam);
            break;
        case 6:
            BsMenuMgr::Func6D448C(79, menuParam);
            break;
        case 7:
            BsMenuMgr::Func6D448C(80, menuParam);
            break;
        case 8:
            BsMenuMgr::Func6D448C(91, menuParam);
            break;
        case 9:
            BsMenuMgr::Func6D4308();
            break;
        case 10:
            BsMenuMgr::Func6D448C(81, menuParam);
            break;
        case 11:
            BsMenuMgr::Func6D448C(82, menuParam);
            break;
        case 12:
            BsMenuMgr::Func6D448C(Func150280() ? 85 : 87, BsMenuMgr::s_MenuParam);
            // fall through
        case 13:
            BsMenuMgr::Func6D448C(86, BsMenuMgr::s_MenuParam);
            break;
        }
        m_EventState++;
        break;
    case 1:
        if (param == 3) {
            if (!BsMenuMgr::IsIdle()) {
                break;
            }
            if (BsMenuMgr::s_Result == 0) {
                return RESULT_DONE;
            }
            if (Func150288() != NULL && Func150288()->pTalkRecept->m_pOwner != NULL) {
                Func150288()->pTalkRecept->m_pOwner->RequestQuit(1, true, true);
            }
            m_EventState++;
        } else if (param == 13) {
            if (!BsMenuMgr::IsIdle()) {
                break;
            }
            if (BsMenuMgr::s_Result == 0) {
                return RESULT_DONE;
            }
            if (demo::PlayerGetActorEntry(static_cast<netgame::PlayerNo>(4), false) == NULL) {
                break;
            }
            DemoActor* actor = demo::GetTalkActor();
            if (actor == NULL) {
                break;
            }
            TalkSystem* talk = GetActorTalkRecept(actor)->m_pOwner;
            if (talk == NULL) {
                break;
            }
            talk->RequestQuit(1, true, true);
            m_EventState++;
        } else if (BsMenuMgr::IsIdle()) {
            return RESULT_DONE;
        }
        break;
    case 2:
        if (param == 3) {
            if (Func150288() == NULL || Func150288()->pTalkRecept->m_pOwner == NULL ||
                Func150288()->pTalkRecept->m_pOwner->m_State != TalkSystem::STATE_CLOSED) {
                break;
            }
            BsMenuMgr::Func6D29C4(BsMenuMgr::s_Unknown950DB0, Func150288()->value);
            m_EventState++;
        } else if (param == 13) {
            if (demo::PlayerGetActorEntry(static_cast<netgame::PlayerNo>(4), false) == NULL) {
                break;
            }
            DemoActor* actor = demo::GetTalkActor();
            if (actor == NULL) {
                break;
            }
            TalkSystem* talk = GetActorTalkRecept(actor)->m_pOwner;
            if (talk == NULL || talk->m_State != TalkSystem::STATE_CLOSED) {
                break;
            }
            BsMenuMgr::Func6D3A74(g_ModuleStandIndex, BsMenuMgr::s_Unknown950DB0);
            m_EventState++;
        } else if (BsMenuMgr::IsIdle()) {
            return RESULT_DONE;
        }
        break;
    case 3:
        if (param == 3) {
            if (BsMenuMgr::IsIdle()) {
                m_EventState++;
                return RESULT_DONE;
            }
        } else if (param == 13) {
            if (BsMenuMgr::IsIdle()) {
                if (BsMenuMgr::s_Result == 1) {
                    SvDesignStand* stand = &GetGardenData()->strcData.stands[g_ModuleStandIndex];
                    SvPlayer* player = SvGetCurrentPlayer();
                    if (player != NULL && stand != NULL) {
                        Func1B4F6C(Func20C4C8(player->patterns, BsMenuMgr::s_Unknown950DB0), stand);
                    }
                }
                m_EventState++;
                return RESULT_DONE;
            }
        } else if (BsMenuMgr::IsIdle()) {
            return RESULT_DONE;
        }
        break;
    }
    return result;
}

// 0x005DD4A4 (name is ours)
void IFlowRecept::SetPocketItemAndNotify(u32 a, u32 b)
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        Func2BFDE0(&player->inventory, a, b, 1);
        Func5C8CC4();
    }
}

// 0x005DD4E0 (name is ours)
s32 IFlowRecept::Branch47(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL && player->playerInfo.IsValid() && (player->unk_0x8F77[4] & 4) != 0) {
        value = 1;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DD544 (name is ours)
s32 IFlowRecept::Branch30(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, IsStructurePlaced(79));
}

// 0x005DD56C (name is ours)
s32 IFlowRecept::BranchNetPlayerCount(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 count = netgame::GetPlayerCount();
    if (count != 0) {
        count--;
    }
    *index = count;
    return RESULT_DONE;
}

// 0x005DD594 (name is ours)
s32 IFlowRecept::OpenTalkMenu(u8 menu)
{
    TalkSystem* talk = m_pTalkRecept->m_pOwner;
    if (m_EventState == 0) {
        if (talk != NULL && talk->m_pPhrase != NULL && menu == 49) {
            Insert* insert = talk->m_pPhrase->GetInsert();
            insert->ClearYear(0);
            insert->ClearMonth(0);
            insert->ClearDay(0);
            insert->ClearConstellation();
        }
        BsMenuMgr::Func6D3B20(menu, BsMenuMgr::s_MenuParam);
        m_EventState = 1;
        return RESULT_WAIT;
    }
    if (m_EventState != 1) {
        return RESULT_NONE;
    }
    if (!BsMenuMgr::IsIdle()) {
        return RESULT_WAIT;
    }
    if (talk != NULL && talk->m_pPhrase != NULL && menu == 48) {
        const s32* date = Func1BB904();
        s32 value = date[0];
        s32 monthDay = date[1];
        Insert* insert = talk->m_pPhrase->GetInsert();
        insert->SetTag4_13(value, 0);
        insert->SetMonth(static_cast<s8>(monthDay), 0);
        insert->SetDay(static_cast<s8>(monthDay >> 8), 0);
    }
    return RESULT_DONE;
}

// 0x005DD6E4 (name is ours)
s32 IFlowRecept::Branch160(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, Func23CBC0());
}

// 0x005DD708 (name is ours)
s32 IFlowRecept::Branch23(u32* index, const libms::FlowNodeBranch& node)
{
    u32 param;
    node.GetParam(&param);
    u32 value = 1;
    for (s32 i = 0; i < 4; i++) {
        SvPlayer* player = SvGetPlayer(static_cast<PlayerSaveNumber>(i));
        if (player != NULL && player->playerInfo.IsValid() && player->playerFlags.playerSetNameAndTownName) {
            value = 0;
            break;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DD780 (name is ours)
s32 IFlowRecept::Branch57(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    *index = g_Byte0x951155 == 0 ? 1 : 0;
    return RESULT_DONE;
}

// 0x005DD7B4 (name is ours)
s32 IFlowRecept::Event22(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    if (node.GetParam() != 0) {
        Func293264();
    } else {
        Func2912B8(4);
    }
    return RESULT_DONE;
}

// 0x005DD7E8 (name is ours)
s32 IFlowRecept::Branch85(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, Func150290());
}

// 0x005DD80C (name is ours)
s32 IFlowRecept::Branch81(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    SvPlayer* player = g_pSvCurrentPlayer;
    *index = player != NULL && player->playerFlags.unk_0x8_0 ? 1 : 0;
    return RESULT_DONE;
}

// 0x005DD848 (name is ours)
s32 IFlowRecept::Branch31(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 param;
    node.GetParam(&param);
    *index = Func75A27C(&GetGardenData()->strcData) < param ? 1 : 0;
    return RESULT_DONE;
}

// 0x005DD894 (name is ours)
s32 IFlowRecept::BranchSameName(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    SvPlayer* current = SvGetCurrentPlayer();
    u8 saveNo = netgame::GetDefaultSaveNo(4);
    SvPersonalId* id = &current->playerInfo;
    WordFix<9> name;
    MakePlayerName(&name, id->playerData.playerName, id->playerData.gender & 1);
    *index = 2;
    if (saveNo >= 4) {
        return RESULT_DONE;
    }
    for (u32 i = 0; i < 4; i++) {
        if (i == saveNo || !SvIsAnyPlayerSaveNo(static_cast<PlayerSaveNumber>(i))) {
            continue;
        }
        SvPlayer* player = SvGetPlayer(static_cast<PlayerSaveNumber>(i));
        if (player == NULL || !player->playerInfo.IsValid()) {
            continue;
        }
        WordFix<9> other;
        MakePlayerName(&other, player->playerInfo.playerData.playerName, player->playerInfo.playerData.gender & 1);
        CmpEngine engine;
        if (engine.Compare(&name, &other)) {
            *index = player->playerFlags.playerSetNameAndTownName ? 0 : 1;
            break;
        }
    }
    return RESULT_DONE;
}

// 0x005DD9DC (name is ours)
s32 IFlowRecept::Branch37(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    if (Func5C7BDC() != NULL && Func717120() != 2) {
        value = 1;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DDA14 (name is ours)
s32 IFlowRecept::Branch38(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 0;
    void* object = Func5C7BDC();
    if (object != NULL) {
        if (Func717120() == 9) {
            for (u32 i = 0; i < 6; i++) {
                void* entry = Func7171D4(object, static_cast<u8>(i));
                if (!Func305240(entry) || Func720FC0(entry)) {
                    value = 1;
                    break;
                }
            }
        } else {
            value = 1;
        }
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DDA9C (name is ours)
s32 IFlowRecept::Event93(libms::Flow* flow, const libms::FlowNodeEvent& node)
{
    (void)flow;
    switch (node.GetParam()) {
    case 0:
        Func1E0818(1);
        return RESULT_DONE;
    case 1:
        Func1E0818(0);
        Func1E0870(0);
        return RESULT_DONE;
    }
    return RESULT_NONE;
}

// 0x005DDAF4 (name is ours)
s32 IFlowRecept::Branch80(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    *index = m_OpenCount <= 4 ? 1 : 0;
    return RESULT_DONE;
}

// 0x005DDB10 (name is ours)
s32 IFlowRecept::Branch162(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, Func23CB14(&BsMenuMgr::s_Unknown950D70));
}

// 0x005DDB3C (name is ours)
s32 IFlowRecept::Branch50(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 1;
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL && player->unk_0x8D38 >= 7) {
        value = 0;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DDB84 (name is ours)
s32 IFlowRecept::Branch158(u32* index, const libms::FlowNodeBranch& node)
{
    (void)node;
    return SelectCase(index, Func23CBF0());
}

// 0x005DDBA8 (name is ours)
s32 IFlowRecept::Branch51(u32* index, const libms::FlowNodeBranch& node)
{
    node.GetCaseNum();
    u32 value = 1;
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL && player->unk_0x8D38 >= 30) {
        value = 0;
    }
    *index = value;
    return RESULT_DONE;
}

// 0x005DDBF0 (name is ours)
s32 IFlowRecept::BranchSelectedItem(u32* index, const libms::FlowNodeBranch& node)
{
    SvPlayer* player = SvGetCurrentPlayer();
    u8 slot = BsMenuMgr::s_SelectedSlot;
    const SvFgName* item;
    if (slot < SvPlayerInventory::SIZE && player != NULL) {
        item = player->inventory.GetItem(slot, NULL);
    } else {
        item = &SvFgName::s_Empty;
    }
    switch (node.GetParam()) {
    case 0:
        *index = Func3107F4(item) ? 0 : 1;
        break;
    case 1:
        *index = Func76A650(item) ? 1 : 0;
        break;
    case 2:
        *index = Func769774(item) ? 0 : 1;
        break;
    case 3:
        *index = Func768FF8(item) ? 0 : 1;
        break;
    case 4:
        *index = Func768FF8(item) == 2 ? 0 : 1;
        break;
    }
    return RESULT_DONE;
}

// 0x005DDCFC slot 0x1C (name is ours)
void IFlowRecept::OnNode(const libms::FlowNode& node)
{
    (void)node;
}

// 0x005DDD00 (name after the RTTI)
IFlowRecept::IFlowRecept(ITalkRecept* pTalkRecept)
    : m_pTalkRecept(pTalkRecept), m_pOwner(NULL), m_File(), m_Label(), m_EventState(0), m_MenuFilter(NULL), m_MenuArg(0),
      m_OpenCount(0)
{
}

// 0x005DDD54 slot 0x00 (name after the RTTI)
// 0x005DDD44 (deleting dtor)
IFlowRecept::~IFlowRecept()
{
    // empty (in the original too)
}

// 0x0075BB5C slot 0x2C
bool IFlowRecept::vf_0x2C(WordFix<33>* word)
{
    (void)word;
    return false;
}

// 0x0075BB64 slot 0x34
bool IFlowRecept::vf_0x34(WordFix<33>* word)
{
    (void)word;
    return false;
}

// 0x0075BB6C slot 0x20 (name is ours)
void IFlowRecept::OnStartMessage(sead::FixedSafeString<32>* file)
{
    (void)file;
}

// 0x0075BB70 slot 0x30
bool IFlowRecept::vf_0x30(WordFix<33>* word)
{
    (void)word;
    return false;
}

} // namespace script
