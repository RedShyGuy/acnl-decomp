#pragma once

#include "decomp.h"
#include "script/dFlowSeq.h"

namespace nn {
namespace fnd {
struct DateTimeParameters;
} // namespace fnd
} // namespace nn

namespace script {
class IFlowRecept;
class TalkSystem;

// The controller of the flow charts (script::Mgr +0x18; no RTTI, the name is ours, after TalkSystem): a stack of four
// flows (a call pushes the next one), the flow recept the game gives, the message the flows ask the talk for and a
// jump to another flow file. Two states (PMF table 0x00892118): 0 the flows run, 1 the talk of the last message ends.
class FlowSystem
{
public:
    enum State : u8
    {
        STATE_RUN = 0,
        STATE_END_TALK = 1,
        STATE_NUM = 2,
    };
    typedef void (FlowSystem::*StateFunc)();
    static const s32 SEQ_NUM = 4;

    explicit FlowSystem(nn::fnd::DateTimeParameters* pDate); // 0x00600DB4
    // (inline) the flows are finalized and deleted
    ~FlowSystem()
    {
        for (u32 i = 0; i < SEQ_NUM; i++) {
            FlowSeq* seq = m_pSeqs[i];
            if (seq != NULL) {
                seq->Finalize();
            }
        }
        for (u32 i = 0; i < SEQ_NUM; i++) {
            if (m_pSeqs[i] != NULL) {
                delete m_pSeqs[i];
                m_pSeqs[i] = NULL;
            }
        }
    }

    // the flows run with recept; its talk recept talks in talk
    void SetRecept(IFlowRecept* recept, TalkSystem* talk); // 0x00600864
    // the recept is let go (and the talk recept)
    void ReleaseRecept(); // 0x00600904
    // the flow of the file starts at the entry label in a new flow of the stack (file NULL: the file and the label
    // of the recept)
    void Start(const char* file, const char* label); // 0x00600CC0
    // the current flow ends; the flow of the file starts at the entry label after it
    void RequestJump(const char* file, const char* label); // 0x00600D5C
    // (inline) as RequestJump, at the entry node
    void RequestJump(const char* file, s32 entry)
    {
        if (m_pCurrent != NULL) {
            m_pCurrent->m_IsEnd = 1;
            m_pJumpFile = file;
            m_pJumpLabel = NULL;
            m_JumpEntry = entry;
        }
    }
    // all flows end
    DECOMP_EMIT_COPY void Abort() // 0x00600D88
    {
        for (u32 i = 0; i < SEQ_NUM; i++) {
            FlowSeq* seq = m_pSeqs[i];
            if (seq == NULL) {
                break;
            }
            seq->m_IsAborted = 1;
        }
    }
    DECOMP_EMIT_COPY FlowSeq* GetCurrent() { return m_pCurrent; } // 0x00600CB8
    DECOMP_EMIT_COPY nn::fnd::DateTimeParameters* GetDate() { return m_pDate; } // 0x00600D80
    // (inline) the state machine
    void Update() { (this->*s_StateFuncs[m_State])(); }
    // (inline) the next flow of the stack becomes the current one (none left: false)
    bool Push()
    {
        if (m_Depth >= static_cast<u32>(SEQ_NUM)) {
            return false;
        }
        m_pCurrent = m_pSeqs[m_Depth];
        m_Depth++;
        return true;
    }

    // (the state functions)
    void UpdateRun(); // 0x00600A94
    void UpdateEndTalk(); // 0x006009AC

    nn::fnd::DateTimeParameters* m_pDate; // 0x00 the date of the flows (script::Mgr +0x10)
    FlowSeq* m_pSeqs[SEQ_NUM];            // 0x04
    u32 m_Depth;                          // 0x14 the flows on the stack
    FlowSeq* m_pCurrent;                  // 0x18 the top of the stack (NULL: none)
    IFlowRecept* m_pRecept;               // 0x1C
    u8 m_State;                           // 0x20
    FlowMessage m_Message;                // 0x24
    const char* m_pJumpFile;              // 0x80 the jump to another flow (NULL: none)
    const char* m_pJumpLabel;             // 0x84 its entry label (NULL: the entry node)
    s32 m_JumpEntry;                      // 0x88 its entry node (-1: the label)

    static const StateFunc s_StateFuncs[STATE_NUM];
};
ASSERT_SIZE(FlowSystem, 0x8C);
} // namespace script
