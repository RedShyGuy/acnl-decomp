#pragma once

#include "decomp.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace os {
class Thread;
}
namespace nex {
// A thread that runs Run of the object (ObjectThread calls a member function). The stack is
// allocated by nex or (s_UsesAutoStack) by the AutoStackManager of nn::os. The names of the members
// and of the functions without a reference name are ours (m_bFinished is from an assert text).
// RTTI N2nn3nex16ObjectThreadRootE @ 0x008CE5E4
// vtable 0x008FD1F0 (vptr 0x008FD1F8), offset_to_top 0, 3 entries
class ObjectThreadRoot : public ::nn::nex::RootObject
{
public:
    // the thread of nn::os and its stack
    class ThreadData
    {
    public:
        ThreadData() : m_pThread(nullptr), m_pStack(nullptr), m_IsJoined(false) {}

        nn::os::Thread* m_pThread; // 0x0
        u8* m_pStack;              // 0x4 (NewArray, aligned to 4 KB inside)
        u32 m_StackSize;           // 0x8
        bool m_IsJoined;           // 0xC
    };
    // the default core: the one of the process
    static const s32 CORE_NO_DEFAULT = static_cast<s32>(0x80000000);

    // (inline)
    explicit ObjectThreadRoot(const String& name)
        : m_Name(name), m_ThreadId(0), m_StackSize(0), m_IsLaunched(false), m_IsFinished(false), m_CoreNo(CORE_NO_DEFAULT)
    {
        m_pData = new (MemoryManager::Allocate(sizeof(ThreadData))) ThreadData;
        m_Priority = s_DefaultPriority;
    }
    virtual ~ObjectThreadRoot(); // 0x00382DC4 slot 0x00 | fefates:callseq [tier C]
    // 0x00382D90 slot 0x04 (deleting dtor)
    // the function of the thread
    virtual void Run() = 0; // slot 0x08

    // starts the thread (false if it failed or there is a thread)
    bool LaunchImpl(); // 0x00382718 | fefates:bytes [tier B]
    // waits for the end of the thread and deletes it (false after the timeout)
    bool Wait(unsigned int timeout); // 0x00382AA8 | fefates:bytes [tier B]
    // starts the thread and waits until it runs (stackSize 0: the default)
    bool Launch(unsigned int stackSize); // 0x00382CF8 | fefates:callgraph [tier C]
    // (symbols.json names this ObjectThreadProc, but it is a setter; the thread function follows it)
    static void SetUsesAutoStack(bool usesAutoStack); // 0x003829A0 (name is ours)
    // the function of the threads
    static void ObjectThreadProc(nn::nex::ObjectThreadRoot* pThread); // 0x003829B0 (name from the reference symbols, see above)

    String m_Name;           // 0x04
    ThreadData* m_pData;     // 0x0C
    u32 m_ThreadId;          // 0x10
    s32 m_Priority;          // 0x14
    u32 m_StackSize;         // 0x18
    bool m_IsLaunched;       // 0x1C
    volatile bool m_IsRunning;  // 0x1D (set when Run starts)
    volatile bool m_IsFinished; // 0x1E (m_bFinished; set when Run returns)
    s32 m_CoreNo;            // 0x20

    static bool s_UsesAutoStack;
    static u32 s_DefaultStackSize;
    static s32 s_DefaultPriority;
    // the threads in Run
    static volatile s32 s_RunningCount;
};
ASSERT_OFFSET(ObjectThreadRoot, m_CoreNo, 0x20);
ASSERT_SIZE(ObjectThreadRoot, 0x24);
} // namespace nex
} // namespace nn
