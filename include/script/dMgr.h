#pragma once

#include "decomp.h"

namespace nn {
namespace fnd {
struct DateTimeParameters;
} // namespace fnd
} // namespace nn

namespace sead {
class Heap;
} // namespace sead

namespace script {
class FlowSystem;
class TalkSystem;

// The manager of the scripts: the talk and the flow charts with the date they share. A singleton whose
// SingletonDisposer_ lies in its first 16 bytes; script::Base runs it (names are ours).
class Mgr
{
public:
    class SingletonDisposer_;

    static Mgr* createInstance(sead::Heap* heap); // 0x005EA318 (name after sead)
    static void deleteInstance(); // 0x005EA38C (name after sead)

    // the date, the talk and the flows are created, the windows set up; true when they are ready
    bool Initialize(); // 0x005EA478
    // everything is freed
    void Finalize(); // 0x005EA5A8
    void Calc(); // 0x005EA6E0
    // the windows of the talk are drawn
    void Draw(); // 0x005EA444

    static Mgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;
    // the heaps of the script (names after the heap names "ScriptHeap<Name>"; made by CreateHeaps): the memory of libms,
    // the message project and the string lists of StrMgr, the message files of the talk, the letter files, the flow files
    static sead::Heap* s_pLibmsHeap;
    static sead::Heap* s_pStaticHeap;
    static sead::Heap* s_pMsgHeap;
    static sead::Heap* s_pMailHeap;
    static sead::Heap* s_pFlowHeap;

    // the five heaps of the script in parent (the script heap of HeapInit; name is ours)
    static void CreateHeaps(sead::Heap* parent); // 0x00120E2C
    // the five heaps are destroyed (name is ours)
    static void DestroyHeaps(); // 0x0013987C
    // the size of the script heap of HeapInit (inline; the copy is called by HeapInit; name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE static u32 GetHeapSize() { return 0x1A3200; } // 0x00120F7C

    // (inline)
    Mgr() : m_pDate(NULL), m_pTalk(NULL), m_pFlow(NULL) {}

    u32 m_SingletonDisposerBuffer[4];      // 0x00 the SingletonDisposer_
    nn::fnd::DateTimeParameters* m_pDate;  // 0x10 the date of the talk and the flows
    TalkSystem* m_pTalk;                   // 0x14
    FlowSystem* m_pFlow;                   // 0x18
};
ASSERT_SIZE(Mgr, 0x1C);

// (name is ours) loads the message project, the string lists, the letter manager with the first characters of
// STR_Mailchk and the UI manager; true when all are ready (called until then)
bool Setup(); // 0x005E9DD0
} // namespace script
