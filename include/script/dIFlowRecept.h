#pragma once

#include "decomp.h"
#include "sead/seadFixedSafeString.h"

class SvFgName;

namespace sead {
class Heap;
} // namespace sead

namespace libms {
class Flow;
class FlowNode;
class FlowNodeBranch;
class FlowNodeEntry;
class FlowNodeEvent;
} // namespace libms

class SvFgName;
namespace oml {
namespace framework {
class Process;
} // namespace framework
} // namespace oml

namespace script {
template <u32 N>
class WordFix;
struct FlowFile;
class FlowSystem;
class ITalkRecept;

// The recept of the flow charts: the object the game gives a flow (an NPC, an object, a menu; usually also its talk
// recept) that loads the flow files and answers the branches and events of FlowSeq. The defaults handle the branches
// and events of the whole game by their id. Slot and member names are ours.
// RTTI N6script11IFlowReceptE @ 0x008D37C8
// vtable 0x00909E14 (vptr 0x00909E1C), offset_to_top 0, 14 entries
class IFlowRecept
{
public:
    // the results of the handlers of the nodes
    enum Result
    {
        RESULT_NONE = 0,  // not handled
        RESULT_DONE = 1,
        RESULT_WAIT = 2,  // the node is handled again in the next frame
    };

    explicit IFlowRecept(ITalkRecept* pTalkRecept); // 0x005DDD00
    virtual ~IFlowRecept(); // 0x005DDD54 slot 0x00
    // 0x005DDD44 slot 0x04 (deleting dtor)
    // the flow file name goes on loading into file (heap: the first call sets the name and the heap); true when it
    // has been loaded
    virtual bool LoadFlow(FlowFile* file, const sead::SafeStringBase<char>& name, sead::Heap* heap); // 0x005DBD58 slot 0x08
    // the case of a branch node (*index: the number of cases; a case out of range is the first one)
    virtual s32 OnBranch(u32* index, const libms::FlowNodeBranch& node); // 0x005DA278 slot 0x0C
    virtual s32 OnEvent(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005D9B5C slot 0x10
    virtual s32 OnEntry(const libms::FlowNodeEntry& node); // 0x005D9B54 slot 0x14
    // after a node; false: the flows stop for this frame
    virtual bool IsContinue(); // 0x005DAEE4 slot 0x18
    // the flow has moved to node
    virtual void OnNode(const libms::FlowNode& node); // 0x005DDCFC slot 0x1C
    // a talk message of the file starts
    virtual void OnStartMessage(sead::FixedSafeString<32>* file); // 0x0075BB6C slot 0x20
    // the flow (not a call) has ended
    virtual void OnFlowEnd(); // 0x005DA26C slot 0x24
    // the argument of the menu of the event 6 (0: the event is not handled)
    virtual u32 vf_0x28(const libms::FlowNodeEvent& node); // 0x005DCFA8 slot 0x28
    // the words for the windows of the event 16 (false: none)
    virtual bool vf_0x2C(WordFix<33>* word); // 0x0075BB5C slot 0x2C
    virtual bool vf_0x30(WordFix<33>* word); // 0x0075BB70 slot 0x30
    virtual bool vf_0x34(WordFix<33>* word); // 0x0075BB64 slot 0x34

    // (the file and the entry label of FlowSystem::Start)
    void SetStart(const char* file, const char* label); // 0x005DCE44
    DECOMP_EMIT_COPY FlowSystem* GetOwner() const { return m_pOwner; } // 0x005DDCF4
    void SetSelectedPocketItem(const SvFgName* item); // 0x005DB268
    void ClearSelectedPocketItem(); // 0x005DB9EC
    void ClearMarkedPocketItems(); // 0x005DBCE8
    void SetPocketItemAndNotify(u32 a, u32 b); // 0x005DD4A4
    // a menu that fills words of the insert of the talk
    s32 OpenTalkMenu(u8 menu); // 0x005DD594

    // the handlers of the events and branches by their id (the meaning of most ids is not known)
    s32 Event100(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005D5CFC
    s32 BranchRandom(u32* index, const libms::FlowNodeBranch& node); // 0x005D9B34
    s32 BranchWeighted(u32* index, const libms::FlowNodeBranch& node); // 0x005DA1F4
    s32 BranchMonth(u32* index, const libms::FlowNodeBranch& node); // 0x005DACBC
    s32 BranchChoice(u32* index, const libms::FlowNodeBranch& node); // 0x005DAD00
    s32 BranchSeason(u32* index, const libms::FlowNodeBranch& node); // 0x005DAD48
    s32 Branch155(u32* index, const libms::FlowNodeBranch& node); // 0x005DAD7C
    s32 Branch21(u32* index, const libms::FlowNodeBranch& node); // 0x005DADA4
    s32 EventStartMessage(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DAE24
    s32 EventStartFlow(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DAE48
    s32 Branch41(u32* index, const libms::FlowNodeBranch& node); // 0x005DAEEC
    s32 BranchPartOfDay(u32* index, const libms::FlowNodeBranch& node); // 0x005DAF40
    s32 Event0(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DAF74
    s32 EventCallEntry(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DB024
    s32 EventWait(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DB048
    s32 BranchHour(u32* index, const libms::FlowNodeBranch& node); // 0x005DB088
    s32 BranchWeekday(u32* index, const libms::FlowNodeBranch& node); // 0x005DB114
    s32 Branch45(u32* index, const libms::FlowNodeBranch& node); // 0x005DB190
    s32 Branch106(u32* index, const libms::FlowNodeBranch& node); // 0x005DB1AC
    s32 BranchNookling(u32* index, const libms::FlowNodeBranch& node); // 0x005DB1D8
    s32 Branch107(u32* index, const libms::FlowNodeBranch& node); // 0x005DB258
    s32 EventMenu(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DB2B0
    s32 BranchIsNew3ds(u32* index, const libms::FlowNodeBranch& node); // 0x005DBA34
    s32 BranchConnection(u32* index, const libms::FlowNodeBranch& node); // 0x005DBA58
    s32 BranchBirthday(u32* index, const libms::FlowNodeBranch& node); // 0x005DBACC
    s32 Branch29(u32* index, const libms::FlowNodeBranch& node); // 0x005DBB6C
    s32 EventCloseTalk(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DBB94
    s32 Event13(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DBC48
    s32 Branch32(u32* index, const libms::FlowNodeBranch& node); // 0x005DBDF4
    s32 BranchStage(u32* index, const libms::FlowNodeBranch& node); // 0x005DBE18
    s32 BranchChoice12(u32* index, const libms::FlowNodeBranch& node); // 0x005DBF48
    s32 BranchChoiceOdd(u32* index, const libms::FlowNodeBranch& node); // 0x005DBF90
    s32 Branch34(u32* index, const libms::FlowNodeBranch& node); // 0x005DBFD8
    s32 Branch54(u32* index, const libms::FlowNodeBranch& node); // 0x005DC050
    s32 BranchBeeSting(u32* index, const libms::FlowNodeBranch& node); // 0x005DC088
    s32 BranchGender(u32* index, const libms::FlowNodeBranch& node); // 0x005DC0E0
    s32 Branch43(u32* index, const libms::FlowNodeBranch& node); // 0x005DC13C
    s32 BranchMeowCoupons(u32* index, const libms::FlowNodeBranch& node); // 0x005DC190
    s32 Branch15(u32* index, const libms::FlowNodeBranch& node); // 0x005DC1E8
    s32 BranchLeapDay(u32* index, const libms::FlowNodeBranch& node); // 0x005DC21C
    s32 BranchIsVillage(u32* index, const libms::FlowNodeBranch& node); // 0x005DC24C
    s32 Branch25(u32* index, const libms::FlowNodeBranch& node); // 0x005DC284
    s32 EventOpenMenu(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DC2C0
    s32 Event6(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DC408
    s32 Branch53(u32* index, const libms::FlowNodeBranch& node); // 0x005DC4C0
    s32 Branch46(u32* index, const libms::FlowNodeBranch& node); // 0x005DC4F8
    s32 Branch28(u32* index, const libms::FlowNodeBranch& node); // 0x005DC550
    s32 Branch13(u32* index, const libms::FlowNodeBranch& node); // 0x005DC6CC
    s32 EventTransition(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DC6F0
    s32 EventSetNoRepeat(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DC7E8
    s32 Branch44(u32* index, const libms::FlowNodeBranch& node); // 0x005DC804
    s32 BranchLeapDayBirthday(u32* index, const libms::FlowNodeBranch& node); // 0x005DC86C
    s32 Branch40(u32* index, const libms::FlowNodeBranch& node); // 0x005DC8C0
    s32 EventMenu2(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DC950
    s32 Event94(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DCD28
    s32 EventClearNoRepeat(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DCD74
    s32 EventQuitAndWait(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DCD90
    s32 Branch157(u32* index, const libms::FlowNodeBranch& node); // 0x005DCE94
    s32 Branch88(u32* index, const libms::FlowNodeBranch& node); // 0x005DCEB4
    s32 Branch55(u32* index, const libms::FlowNodeBranch& node); // 0x005DCED8
    s32 Branch36(u32* index, const libms::FlowNodeBranch& node); // 0x005DCEF4
    s32 BranchPlayerCount(u32* index, const libms::FlowNodeBranch& node); // 0x005DCF2C
    s32 Branch159(u32* index, const libms::FlowNodeBranch& node); // 0x005DCFB0
    s32 Branch161(u32* index, const libms::FlowNodeBranch& node); // 0x005DCFD4
    s32 EventMenu3(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DCFF8
    s32 Branch47(u32* index, const libms::FlowNodeBranch& node); // 0x005DD4E0
    s32 Branch30(u32* index, const libms::FlowNodeBranch& node); // 0x005DD544
    s32 BranchNetPlayerCount(u32* index, const libms::FlowNodeBranch& node); // 0x005DD56C
    s32 Branch160(u32* index, const libms::FlowNodeBranch& node); // 0x005DD6E4
    s32 Branch23(u32* index, const libms::FlowNodeBranch& node); // 0x005DD708
    s32 Branch57(u32* index, const libms::FlowNodeBranch& node); // 0x005DD780
    s32 Event22(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DD7B4
    s32 Branch85(u32* index, const libms::FlowNodeBranch& node); // 0x005DD7E8
    s32 Branch81(u32* index, const libms::FlowNodeBranch& node); // 0x005DD80C
    s32 Branch31(u32* index, const libms::FlowNodeBranch& node); // 0x005DD848
    s32 BranchSameName(u32* index, const libms::FlowNodeBranch& node); // 0x005DD894
    s32 Branch37(u32* index, const libms::FlowNodeBranch& node); // 0x005DD9DC
    s32 Branch38(u32* index, const libms::FlowNodeBranch& node); // 0x005DDA14
    s32 Event93(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005DDA9C
    s32 Branch80(u32* index, const libms::FlowNodeBranch& node); // 0x005DDAF4
    s32 Branch162(u32* index, const libms::FlowNodeBranch& node); // 0x005DDB10
    s32 Branch50(u32* index, const libms::FlowNodeBranch& node); // 0x005DDB3C
    s32 Branch158(u32* index, const libms::FlowNodeBranch& node); // 0x005DDB84
    s32 Branch51(u32* index, const libms::FlowNodeBranch& node); // 0x005DDBA8
    s32 BranchSelectedItem(u32* index, const libms::FlowNodeBranch& node); // 0x005DDBF0

    static const u8 EVENT_KIND_MENU = 6;  // (name is ours)
    typedef s32 (IFlowRecept::*EventFunc)(libms::Flow* flow, const libms::FlowNodeEvent& node);
    typedef s32 (IFlowRecept::*BranchFunc)(u32* index, const libms::FlowNodeBranch& node);
    struct EventHandler
    {
        u8 id;
        EventFunc func;
    };
    struct BranchHandler
    {
        u8 id;
        BranchFunc func;
    };
    // the menu of the event 17 and the function that goes with it
    // a filter of an inventory slot (as BsMenuMgr::InventoryFilter); true: the slot is meant
    typedef bool (*InventoryFilter)(const SvFgName* item, u32 value);
    struct MenuInfo
    {
        u8 menu;
        InventoryFilter filter;  // (NULL: none)
    };
    static const u32 EVENT_HANDLER_NUM = 21;
    static const u32 BRANCH_HANDLER_NUM = 77;
    static const u32 MENU_NUM = 36;
    static const EventHandler s_EventHandlers[EVENT_HANDLER_NUM];
    static const BranchHandler s_BranchHandlers[BRANCH_HANDLER_NUM];
    static const MenuInfo s_Menus[MENU_NUM];
    // the event 100: two values for the process and the process
    static u32 s_ProcessArg0;
    static u32 s_ProcessArg1;
    static oml::framework::Process* s_pProcess;

    ITalkRecept* m_pTalkRecept;          // 0x04 the recept of the talk messages of the flows (NULL: none)
    FlowSystem* m_pOwner;                // 0x08 (set by FlowSystem::SetRecept)
    sead::FixedSafeString<32> m_File;    // 0x0C the flow file FlowSystem::Start starts with
    sead::FixedSafeString<32> m_Label;   // 0x38 its entry label
    u32 m_EventState;                    // 0x64 the step of an event that takes several frames (0 at a new node)
    InventoryFilter m_MenuFilter; // 0x68 replaces the filter of the menu of the event 17 (NULL: none)
    u32 m_MenuArg;                       // 0x6C (event 17)
    s32 m_OpenCount;                     // 0x70 the times the event 16 opened its menu
};
ASSERT_SIZE(IFlowRecept, 0x74);
} // namespace script
