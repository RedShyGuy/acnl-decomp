#pragma once

#include "decomp.h"
#include "oml/framework/dProcess.h"

namespace oml {
namespace framework {
// how a process class is made and destroyed (name after libgarden)
struct ProcessFactory
{
    Process* (*create)();           // 0x0
    void (*destroy)(Process* p);    // 0x4
};
ASSERT_SIZE(ProcessFactory, 0x8);

// a process class (name after libgarden; the priorities are its link1/link2): the lower priority calcs/draws first
struct ProcessInfo
{
    ProcessIndex index;     // 0x0
    const char* name;       // 0x4
    u16 calcPriority;       // 0x8
    u16 drawPriority;       // 0xA
};
ASSERT_SIZE(ProcessInfo, 0xC);

// The processes of the game: a tree of all processes (parents and children), the lists of the steps, a slot per
// handle (1024) and flags that pause or hide the children of a process (class and the static functions after
// libgarden; other names are ours).
class ProcessManager
{
public:
    // the slots
    static const s32 PROCESS_MAX = 1024;

    // m_Step: the step running
    enum Step : u8
    {
        STEP_NONE = 0,
        STEP_UPDATE = 1,
        STEP_INITIALIZE = 2,
        STEP_CALC = 3,
        STEP_FINALIZE = 4,
        STEP_DRAW = 5,
    };
    // m_DisabledSteps
    enum DisabledStep : u8
    {
        DISABLE_UPDATE = 1 << 0,
        DISABLE_INITIALIZE = 1 << 1,
        DISABLE_CALC = 1 << 2,
        DISABLE_FINALIZE = 1 << 3,
        DISABLE_DRAW = 1 << 4,
    };
    // m_Flags
    enum Flag : u8
    {
        FLAG_PAUSE_CHILDREN = 1 << 0, // the children do not calc
        FLAG_PAUSED = 1 << 1,         // a parent pauses its children
        FLAG_HIDE_CHILDREN = 1 << 2,  // the children do not draw
        FLAG_HIDDEN = 1 << 3,         // a parent hides its children
    };

    // a process in the tree
    struct TreeNode
    {
        DECOMP_NOIPA TreeNode(); // 0x005222D8
        // (inline; a copy of the original)
        DECOMP_EMIT_COPY DECOMP_NOIPA Process* GetProcess() const { return process; } // 0x007488D4

        Process* process;     // 0x00
        TreeNode* parent;     // 0x04
        TreeNode* child;      // 0x08 the first child
        TreeNode* prev;       // 0x0C
        TreeNode* next;       // 0x10
    };
    // the tree (the processes without a parent are siblings of m_pRoot)
    struct Tree
    {
        DECOMP_NOIPA Tree(); // 0x00522298
        // node as the last child of parent (NULL: at the top); false without a node
        DECOMP_NOIPA bool Add(TreeNode* node, TreeNode* parent); // 0x00522190
        // false without a node or for a node with children
        DECOMP_NOIPA bool Remove(TreeNode* node); // 0x0052221C
        // (inline; copies of the original) the next node in preorder / the next node after the children
        DECOMP_EMIT_COPY DECOMP_NOIPA static TreeNode* Next(TreeNode* node); // 0x007488DC
        DECOMP_EMIT_COPY DECOMP_NOIPA static TreeNode* NextSkipChildren(TreeNode* node); // 0x00748928
        // the first node after start (NULL: from the top) of a process of that class
        DECOMP_EMIT_COPY DECOMP_NOIPA TreeNode* Find(ProcessIndex index, TreeNode* start) const; // 0x0074885C
        // the processes of that class
        DECOMP_EMIT_COPY DECOMP_NOINLINE s32 Count(ProcessIndex index) const; // 0x0074880C

        TreeNode* m_pRoot; // 0x0
    };
    // a process in a list
    struct ListNode
    {
        DECOMP_NOIPA ListNode(); // 0x005222BC
        // both priorities (the new one takes effect with ApplyPriority)
        DECOMP_NOIPA void SetPriority(u16 priority); // 0x005222A4
        DECOMP_NOIPA void ApplyPriority(); // 0x005222B0
        // (inline; a copy of the original)
        DECOMP_EMIT_COPY DECOMP_NOIPA bool IsPriorityChanged() const { return newPriority != priority; } // 0x007488BC

        ListNode* prev;       // 0x00
        ListNode* next;       // 0x04
        Process* process;     // 0x08
        u16 priority;         // 0x0C
        u16 newPriority;      // 0x0E
    };
    // a list of nodes; false without a node
    struct List
    {
        DECOMP_NOIPA List(); // 0x00521600
        DECOMP_NOIPA bool PushFront(ListNode* node); // 0x00521408
        DECOMP_NOIPA bool Remove(ListNode* node); // 0x00521438
        DECOMP_NOIPA bool PushBack(ListNode* node); // 0x005214A0
        // behind the nodes with a lower or equal priority
        DECOMP_NOIPA bool InsertSorted(ListNode* node); // 0x005214D0

        ListNode* m_pHead; // 0x0
        ListNode* m_pTail; // 0x4
    };

    ProcessManager(); // 0x00521298

    static ProcessManager* Get(); // 0x005204DC | libgarden [tier A]
    // the tables of the process classes (name is ours)
    static void SetTables(const ProcessInfo* infos, const ProcessFactory* factories, s32 count); // 0x005213F8
    DECOMP_NOIPA static const ProcessInfo* GetInfo(ProcessIndex index); // 0x00521394 | libgarden [tier A]
    DECOMP_NOIPA static Process* CallCreate(ProcessIndex index); // 0x005213C8 | libgarden [tier A]
    DECOMP_NOIPA static void CallDestroy(ProcessIndex index, Process* process); // 0x005213DC | libgarden [tier A]

    // a process of the class without a parent / as a child (NULL for a stopped or stopping parent)
    Process* CreateProcess(ProcessIndex index, u32 arg, u8 type); // 0x005202C0 | libgarden [tier A]
    Process* CreateProcess(ProcessIndex index, Process* parent, u32 arg, u8 type); // 0x005202F4

    // (names are ours) the process of the class with its slot; it initializes at once if it can
    DECOMP_NOIPA Process* Create(const ProcessInfo* info, TreeNode* parent, u32 arg, u8 type); // 0x005205D4
    // an initialized process into the calc and draw lists (during the calc step: at the next update)
    DECOMP_NOIPA void Activate(ListNode* node); // 0x005203A8
    // the process and its children are stopped
    void Stop(Process* process); // 0x00520B74
    // the steps of a frame: the update of the tree, Finalize, Initialize and Calc
    void Update(); // 0x0052112C
    void Draw(); // 0x005204EC
    // the steps of the lists; always true
    DECOMP_NOIPA bool InitializeAll(); // 0x00520CB4
    DECOMP_NOIPA bool FinalizeAll(); // 0x00520E88
    // a stopped process into the finalize list, the flags of the parent for the others, new priorities
    DECOMP_NOIPA void UpdateProcess(TreeNode* node); // 0x0052177C
    // the children of the process of the handle calc / draw or not
    void ResumeChildren(s32 handle); // 0x005210C4
    void PauseChildren(s32 handle); // 0x005210F8
    void ShowChildren(s32 handle); // 0x00520C30
    void HideChildren(s32 handle); // 0x00520C64
    // whether all of the steps are disabled (DisabledStep)
    bool AreStepsDisabled(u8 steps) const; // 0x00520C98

    // (inline; copies of the original)
    DECOMP_EMIT_COPY DECOMP_NOIPA s32 AllocHandle(); // 0x007485FC
    DECOMP_EMIT_COPY Process* GetFirstChild(const Process* process) const; // 0x007485A4
    // whether a process below process has not initialized yet
    DECOMP_EMIT_COPY bool HasUninitializedDescendant(const Process* process) const; // 0x0074866C
    DECOMP_EMIT_COPY u8 IsPausingChildren(s32 handle) const; // 0x00748748
    // the first process of the class after parent (NULL: from the top)
    DECOMP_EMIT_COPY Process* FindProcess(ProcessIndex index, const Process* parent) const; // 0x00748774
    DECOMP_EMIT_COPY s32 CountProcesses(ProcessIndex index) const { return m_Tree.Count(index); } // 0x00748804

    // the slot of a handle (the second round of handles uses the upper half of the slots)
    static u32 GetSlot(u32 handle)
    {
        if (handle < static_cast<u32>(PROCESS_MAX)) {
            return handle & (PROCESS_MAX - 1);
        }
        s32 slot = handle & (PROCESS_MAX - 1);
        if (slot < PROCESS_MAX / 2) {
            slot += PROCESS_MAX / 2;
        }
        return slot;
    }

    static ProcessManager* s_pInstance;
    // the last handle
    static s32 s_HandleCounter;
    // (names after libgarden)
    static const ProcessInfo* s_pInfos;
    static const ProcessFactory* s_pFactories;
    static s32 s_IndexCount;

    TreeNode m_TreeNodes[PROCESS_MAX];      // 0x00000
    ListNode m_CalcNodes[PROCESS_MAX];      // 0x05000 also in the initialize and finalize lists
    ListNode m_DrawNodes[PROCESS_MAX];      // 0x09000
    Tree m_Tree;                            // 0x0D000
    List m_InitializeList;                  // 0x0D004
    List m_CalcList;                        // 0x0D00C
    List m_DrawList;                        // 0x0D014
    List m_FinalizeList;                    // 0x0D01C
    ListNode m_ProcessNodes[PROCESS_MAX];   // 0x0D024
    List m_ProcessList;                     // 0x11024 every process
    u8 m_Flags[PROCESS_MAX];                // 0x1102C Flag
    u8 m_Step;                              // 0x1142C Step
    u8 m_DisabledSteps;                     // 0x1142D DisabledStep
};
ASSERT_OFFSET(ProcessManager, m_Tree, 0xD000);
ASSERT_OFFSET(ProcessManager, m_ProcessNodes, 0xD024);
ASSERT_OFFSET(ProcessManager, m_Flags, 0x1102C);
ASSERT_SIZE(ProcessManager, 0x11430);
} // namespace framework
} // namespace oml
