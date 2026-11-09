#pragma once

#include "decomp.h"
#include "script/dFlowEngine.h"
#include "script/dLoader.h"
#include "sead/seadFixedSafeString.h"

namespace sead {
class Heap;
} // namespace sead

namespace libms {
class FlowNodeEvent;
} // namespace libms

namespace script {
class FlowSystem;
class IFlowRecept;
class ITalkRecept;
class TalkSystem;

// The message a flow asks the talk for (FlowSystem +0x24, shared by its flows; no RTTI, names are ours): the file and
// the label of a talk message, handed to the talk recept of the flow recept when the talk may start one; a state
// machine (PMF table 0x008920A8).
struct FlowMessage
{
    enum State : u8
    {
        STATE_NONE = 0,
        STATE_START = 1,   // the message is given to the talk
        STATE_RESUME = 2,  // the message goes on in the kept window (as the jump of the talk)
        STATE_WAIT = 3,    // waits for the talk
        STATE_END = 4,     // the talk has ended
        STATE_KEPT = 5,    // the talk keeps its window open
        STATE_NUM = 6,
    };
    typedef void (FlowMessage::*StateFunc)();

    // (inline)
    FlowMessage()
        : m_pRecept(NULL), m_pTalk(NULL), m_State(STATE_NONE), m_Label(-1), m_Number(-1), m_LabelName(), m_IsResumed(0),
          m_File(), m_EndMode(0)
    {
    }

    // (inline) back to no message
    void Clear()
    {
        m_pRecept = NULL;
        m_pTalk = NULL;
        m_State = STATE_NONE;
        m_Label = -1;
        m_Number = -1;
        m_LabelName.clear();
        m_IsResumed = 0;
        m_File.clear();
        m_EndMode = 0;
    }
    // (inline) the function of the state
    void Update()
    {
        StateFunc func = s_StateFuncs[m_State];
        if (func != NULL) {
            (this->*func)();
        }
    }
    // (inline) whether the talk can take a message (it is idle or closed, or keeps its window without a quit)
    static bool CanStart(const TalkSystem* talk);

    // the state functions
    void UpdateWait(); // 0x005F402C
    void UpdateStart(); // 0x005F409C
    void UpdateResume(); // 0x005F4264

    IFlowRecept* m_pRecept;               // 0x00
    ITalkRecept* m_pTalk;                 // 0x04 the talk recept of m_pRecept
    u8 m_State;                           // 0x08
    s32 m_Label;                          // 0x0C the index of the text (-1: none)
    s32 m_Number;                         // 0x10 the label as a number (-1: none)
    sead::FixedSafeString<8> m_LabelName; // 0x14 the label (empty: none)
    u8 m_IsResumed;                       // 0x28 the message goes on from the kept window
    sead::FixedSafeString<32> m_File;     // 0x2C the file of the message
    u8 m_EndMode;                         // 0x58 (FlowSystem) 1: the talk is ended, 2: waits for the end

    static const StateFunc s_StateFuncs[STATE_NUM];
};
ASSERT_SIZE(FlowMessage, 0x5C);

// The file of a flow (Script/Flow/<name>.msbf; names are ours).
struct FlowFile
{
    // (inline)
    FlowFile() : m_Loader(false), m_pName(NULL), m_Path(), m_pHeap(NULL) {}

    // (inline) the file is freed
    void Clear()
    {
        m_Loader.Unload();
        m_pName = NULL;
        m_Path.clear();
        m_pHeap = NULL;
    }

    Loader m_Loader;                            // 0x000
    const sead::SafeStringBase<char>* m_pName;  // 0x100
    sead::FixedSafeString<64> m_Path;           // 0x104
    sead::Heap* m_pHeap;                        // 0x150
};
ASSERT_SIZE(FlowFile, 0x154);

// A flow chart that runs (one of the four of FlowSystem; a call pushes the next one): it loads its file, runs the
// nodes and hands messages, branches, events and entries to the flow recept. State machine with 4 states (PMF tables
// 0x008920D8 enter / 0x008920F8 update); member names are ours.
// RTTI N6script7FlowSeqE @ 0x008D3AB8
// vtable 0x0090A8EC (vptr 0x0090A8F4), offset_to_top 0, 9 entries
class FlowSeq : public ::script::FlowEngine
{
public:
    enum State : u8
    {
        STATE_IDLE = 0,
        STATE_LOAD = 1,  // the file is loaded
        STATE_RUN = 2,   // the nodes run
        STATE_END = 3,   // the flow is done
        STATE_NUM = 4,   // (m_NextState: none)
    };
    typedef void (FlowSeq::*StateFunc)();

    // (inline)
    FlowSeq(FlowSystem* pOwner, FlowMessage* pMessage)
        : m_pOwner(pOwner), m_pMessage(pMessage), m_State(STATE_IDLE), m_NextState(STATE_NUM), m_File(),
          m_StartNode(-1), m_Label(), m_Name(), m_pParent(NULL), m_ReturnNode(-1), m_IsYield(0), m_IsWaiting(0),
          m_IsDone(0), m_IsAborted(0)
    {
    }
    virtual ~FlowSeq(); // 0x005F559C slot 0x00
    // 0x005F5560 slot 0x04 (deleting dtor)
    virtual void OnMessage(libms::Flow* flow, const libms::FlowNode& node); // 0x005F4AE4 slot 0x08
    virtual void OnBranch(libms::Flow* flow, const libms::FlowNode& node); // 0x005F4584 slot 0x0C
    virtual void OnEvent(libms::Flow* flow, const libms::FlowNode& node); // 0x005F44D4 slot 0x10
    virtual void OnEntry(libms::Flow* flow, const libms::FlowNode& node); // 0x005F442C slot 0x14
    virtual void OnJump(libms::Flow* flow, const libms::FlowNode& node); // 0x005F5430 slot 0x18
    virtual void OnNode(const libms::FlowNode& node); // 0x005F53D8 slot 0x1C
    virtual bool OnNodeDone(); // 0x005F5298 slot 0x20

    // A talk message for the event the game handles (the recept): the message of file with the label, with the
    // number of the label, or the file in the parameter of an event of kind 5. 2: the talk is busy, 1: done.
    s32 StartMessage(const sead::SafeStringBase<char>& file, const char* label); // 0x005F4688 (name is ours)
    s32 StartMessage(const sead::SafeStringBase<char>& file, s32 number); // 0x005F4840 (name is ours)
    s32 StartMessage(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005F4DCC (name is ours)
    // an event of kind 5 calls the entry (in its parameter) of the flow: in place without a next node, else in the
    // next flow of the stack; true: done
    bool CallEntry(libms::Flow* flow, const libms::FlowNodeEvent& node); // 0x005F518C (name is ours)
    // the flow of the file starts at the entry label (NULL: the file name)
    void Start(const char* file, const char* label); // 0x005F5330
    // (inline) the flow of the file starts at the entry node
    void Start(const char* file, s32 entry)
    {
        if (m_State != STATE_IDLE) {
            return;
        }
        Reset();
        m_Name.copy(sead::SafeStringBase<char>(file), -1);
        m_StartNode = entry;
        m_IsDone = 0;
        m_IsAborted = 0;
        m_NextState = STATE_LOAD;
    }
    // (inline) the flow is called from parent at the entry node (it uses the file of parent)
    void Call(FlowSeq* parent, s32 entry)
    {
        if (m_State != STATE_IDLE) {
            return;
        }
        Reset();
        m_Name.copy(parent->m_Name, -1);
        m_pParent = parent;
        m_StartNode = entry;
        m_IsDone = 0;
        m_IsAborted = 0;
        m_NextState = STATE_LOAD;
    }
    // (inline) the flow chart and the file are freed
    void Finalize()
    {
        m_Flow.Finalize();
        m_Index = -1;
        m_pNode = NULL;
        m_Next = -1;
        if (m_pParent == NULL) {
            m_File.Clear();
        }
    }
    // (inline) the start values
    void Reset()
    {
        m_StartNode = -1;
        m_Label.clear();
        m_Name.clear();
        m_pParent = NULL;
    }
    // (inline) the state machine
    void Update()
    {
        (this->*s_UpdateFuncs[m_State])();
        if (m_NextState != STATE_NUM) {
            m_State = m_NextState;
            m_NextState = STATE_NUM;
            (this->*s_EnterFuncs[m_State])();
        }
    }

    // (the state functions)
    void EnterIdle(); // 0x005F4AE0
    void UpdateIdle(); // 0x005F4AD0
    void EnterLoad(); // 0x005F5144
    void UpdateLoad(); // 0x005F5014
    void EnterRun(); // 0x005F5188
    void UpdateRun(); // 0x005F50F8
    void EnterEnd(); // 0x005F4ADC
    void UpdateEnd(); // 0x005F49D8

    FlowSystem* m_pOwner;               // 0x020
    FlowMessage* m_pMessage;            // 0x024 (FlowSystem::m_Message)
    u8 m_State;                         // 0x028
    u8 m_NextState;                     // 0x029 (STATE_NUM: none)
    FlowFile m_File;                    // 0x02C
    s32 m_StartNode;                    // 0x180 (-1: the entry m_Label)
    sead::FixedSafeString<32> m_Label;  // 0x184 the entry label
    sead::FixedSafeString<32> m_Name;   // 0x1B0 the name of the flow file
    FlowSeq* m_pParent;                 // 0x1DC the flow that called this one (its file is used)
    s32 m_ReturnNode;                   // 0x1E0 the node a call in place goes on with (-1: none)
    u8 m_IsYield;                       // 0x1E4 the flows stop for this frame
    u8 m_IsWaiting;                     // 0x1E5 the node waits (for the talk or the recept)
    u8 m_IsDone;                        // 0x1E6 the flow has ended (FlowSystem pops it)
    u8 m_IsAborted;                     // 0x1E7

    static const StateFunc s_EnterFuncs[STATE_NUM];
    static const StateFunc s_UpdateFuncs[STATE_NUM];
    // the file of a jump to another flow
    static sead::FixedSafeString<32> s_JumpFile;
};
ASSERT_SIZE(FlowSeq, 0x1E8);
} // namespace script
