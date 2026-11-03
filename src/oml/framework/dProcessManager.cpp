#include "oml/framework/dProcessManager.h"

namespace oml {
namespace framework {
namespace {
// a step of a process: Can... decides whether it runs (else RESULT_INVALID), Handle... gets the result
typedef Result (Process::*RunFunc)();
typedef bool (Process::*CanFunc)() const;
typedef void (Process::*HandleFunc)(Result result);
} // namespace

// the steps (in dProcessPhases.cpp: GCC would fold the method pointers into the step functions)
struct ProcessPhase
{
    RunFunc run;        // 0x00
    CanFunc can;        // 0x08
    HandleFunc handle;  // 0x10
};
extern const ProcessPhase s_ProcessPhases[4];

namespace {
// the steps in s_ProcessPhases
enum Phase
{
    PHASE_INITIALIZE = 0,
    PHASE_FINALIZE = 1,
    PHASE_CALC = 2,
    PHASE_DRAW = 3,
};

// 0x00521628 (name is ours)
DECOMP_NOINLINE Result RunPhase(Process* process, RunFunc run, CanFunc can, HandleFunc handle)
{
    Result result;
    if (!(process->*can)()) {
        result = RESULT_INVALID;
    } else {
        result = (process->*run)();
    }
    (process->*handle)(result);
    return result;
}

// 0x005216AC (name is ours)
DECOMP_NOINLINE Result RunInitialize(Process* process)
{
    const ProcessPhase& phase = s_ProcessPhases[PHASE_INITIALIZE];
    return RunPhase(process, phase.run, phase.can, phase.handle);
}

// 0x005216DC (name is ours)
DECOMP_NOINLINE Result RunFinalize(Process* process)
{
    const ProcessPhase& phase = s_ProcessPhases[PHASE_FINALIZE];
    return RunPhase(process, phase.run, phase.can, phase.handle);
}

// 0x0052171C (name is ours)
DECOMP_NOINLINE Result RunCalc(Process* process)
{
    const ProcessPhase& phase = s_ProcessPhases[PHASE_CALC];
    return RunPhase(process, phase.run, phase.can, phase.handle);
}

// 0x005220F4 (name is ours)
DECOMP_NOINLINE Result RunDraw(Process* process)
{
    const ProcessPhase& phase = s_ProcessPhases[PHASE_DRAW];
    return RunPhase(process, phase.run, phase.can, phase.handle);
}
} // namespace

// 0x009762AC
ProcessManager* ProcessManager::s_pInstance = NULL;
// 0x009762B0
s32 ProcessManager::s_HandleCounter = 0;
// 0x009762B4
const ProcessInfo* ProcessManager::s_pInfos = NULL;
// 0x009762B8
const ProcessFactory* ProcessManager::s_pFactories = NULL;
// 0x009762BC
s32 ProcessManager::s_IndexCount = -1;

// 0x005202C0 | libgarden [tier A]
Process* oml::framework::ProcessManager::CreateProcess(ProcessIndex index, u32 arg, u8 type)
{
    return Create(GetInfo(index), NULL, arg, type);
}

// 0x005202F4 (name after libgarden)
Process* oml::framework::ProcessManager::CreateProcess(ProcessIndex index, Process* parent, u32 arg, u8 type)
{
    if (parent == NULL || parent->m_IsStopped || parent->m_State == Process::STATE_STOPPING) {
        return NULL;
    }
    return Create(GetInfo(index), &m_TreeNodes[GetSlot(parent->m_Handle)], arg, type);
}

// 0x005203A8 (name is ours)
void oml::framework::ProcessManager::Activate(ListNode* node)
{
    m_InitializeList.Remove(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
    if (m_Step != STEP_CALC) {
        m_CalcList.InsertSorted(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
        m_DrawList.InsertSorted(&m_DrawNodes[GetSlot(node->process->m_Handle)]);
        node->process->SetState(Process::STATE_RUNNING);
    } else {
        node->process->SetActivationPending(true);
    }
}

// 0x005204DC | libgarden [tier A]
ProcessManager* oml::framework::ProcessManager::Get()
{
    return s_pInstance;
}

// 0x005204EC (name is ours)
void oml::framework::ProcessManager::Draw()
{
    u8& step = m_Step;
    if (!(m_DisabledSteps & DISABLE_DRAW)) {
        step = STEP_DRAW;
        for (ListNode* node = m_DrawList.m_pHead; node != NULL;) {
            ListNode* next = node->next;
            if (!node->process->m_IsStopped && !(m_Flags[GetSlot(node->process->m_Handle)] & FLAG_HIDDEN)) {
                RunDraw(node->process);
            }
            node = next;
        }
    }
    step = STEP_NONE;
}

// 0x005205D4 (name is ours)
Process* oml::framework::ProcessManager::Create(const ProcessInfo* info, TreeNode* parent, u32 arg, u8 type)
{
    if (info == NULL) {
        return NULL;
    }
    const ProcessIndex index = info->index;
    if (m_Step == STEP_DRAW) {
        return NULL;
    }
    Process* process = CallCreate(index);
    if (process == NULL) {
        return NULL;
    }
    process->Set(index, arg, type);
    m_TreeNodes[GetSlot(process->m_Handle)].process = process;
    m_CalcNodes[GetSlot(process->m_Handle)].process = process;
    m_DrawNodes[GetSlot(process->m_Handle)].process = process;
    m_ProcessNodes[GetSlot(process->m_Handle)].process = process;
    m_CalcNodes[GetSlot(process->m_Handle)].SetPriority(info->calcPriority);
    m_DrawNodes[GetSlot(process->m_Handle)].SetPriority(info->drawPriority);
    m_Flags[GetSlot(process->m_Handle)] = 0;
    const Result result = RunInitialize(process);
    if (result == RESULT_SUCCESS) {
        Activate(&m_CalcNodes[GetSlot(process->m_Handle)]);
    } else if (result == RESULT_FAIL) {
        process->Stop();
    }
    if (!process->m_IsStopped && !process->m_IsActivationPending && process->m_State == Process::STATE_CREATED) {
        if (m_Step == STEP_INITIALIZE) {
            process->m_IsInitializationDeferred = true;
        } else {
            m_InitializeList.PushBack(&m_CalcNodes[GetSlot(process->m_Handle)]);
        }
    }
    m_Tree.Add(&m_TreeNodes[GetSlot(process->m_Handle)], parent);
    m_ProcessList.PushFront(&m_ProcessNodes[GetSlot(process->m_Handle)]);
    if (parent != NULL) {
        const s32 parentHandle = parent->process->m_Handle;
        if ((m_Flags[GetSlot(parentHandle)] & FLAG_PAUSE_CHILDREN) || (m_Flags[GetSlot(parentHandle)] & FLAG_PAUSED)) {
            m_Flags[GetSlot(process->m_Handle)] |= FLAG_PAUSED;
        }
        if ((m_Flags[GetSlot(parentHandle)] & FLAG_HIDE_CHILDREN) || (m_Flags[GetSlot(parentHandle)] & FLAG_HIDDEN)) {
            m_Flags[GetSlot(process->m_Handle)] |= FLAG_HIDDEN;
        }
    }
    return process;
}

// 0x00520B74 (name is ours)
void oml::framework::ProcessManager::Stop(Process* process)
{
    if (process->m_IsStopped || process->m_State == Process::STATE_STOPPING) {
        return;
    }
    process->m_IsStopped = true;
    process->OnNotify();
    for (TreeNode* child = m_TreeNodes[GetSlot(process->m_Handle)].child; child != NULL; child = child->next) {
        Stop(child->process);
    }
}

// 0x00520C30 (name is ours)
void oml::framework::ProcessManager::ShowChildren(s32 handle)
{
    m_Flags[GetSlot(handle)] &= ~FLAG_HIDE_CHILDREN;
}

// 0x00520C64 (name is ours)
void oml::framework::ProcessManager::HideChildren(s32 handle)
{
    m_Flags[GetSlot(handle)] |= FLAG_HIDE_CHILDREN;
}

// 0x00520C98 (name is ours)
bool oml::framework::ProcessManager::AreStepsDisabled(u8 steps) const
{
    return (steps & ~m_DisabledSteps) == 0;
}

// 0x00520CB4 (name is ours)
bool oml::framework::ProcessManager::InitializeAll()
{
    for (ListNode* node = m_InitializeList.m_pHead; node != NULL;) {
        ListNode* next = node->next;
        const Result result = RunInitialize(node->process);
        if (result == RESULT_SUCCESS) {
            m_InitializeList.Remove(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
            if (m_Step == STEP_CALC) {
                node->process->m_IsActivationPending = true;
            } else {
                m_CalcList.InsertSorted(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
                m_DrawList.InsertSorted(&m_DrawNodes[GetSlot(node->process->m_Handle)]);
                node->process->m_State = Process::STATE_RUNNING;
            }
        } else if (result == RESULT_FAIL) {
            node->process->Stop();
        }
        node = next;
    }
    return true;
}

// 0x00520E88 (name is ours)
bool oml::framework::ProcessManager::FinalizeAll()
{
    for (ListNode* node = m_FinalizeList.m_pHead; node != NULL;) {
        ListNode* next = node->next;
        TreeNode* child = m_TreeNodes[GetSlot(node->process->m_Handle)].child;
        if ((child == NULL || child->process == NULL) && RunFinalize(node->process) == RESULT_SUCCESS && node != NULL) {
            Process* process = node->process;
            if (process != NULL) {
                m_Tree.Remove(&m_TreeNodes[GetSlot(process->m_Handle)]);
                m_ProcessList.Remove(&m_ProcessNodes[GetSlot(process->m_Handle)]);
                m_FinalizeList.Remove(&m_CalcNodes[GetSlot(process->m_Handle)]);
                const u32 slot = GetSlot(process->m_Handle);
                m_CalcNodes[slot].process = NULL;
                m_DrawNodes[slot].process = NULL;
                m_ProcessNodes[slot].process = NULL;
                m_TreeNodes[slot].process = NULL;
                CallDestroy(process->m_Index, process);
            }
        }
        node = next;
    }
    return true;
}

// 0x005210C4 (name is ours)
void oml::framework::ProcessManager::ResumeChildren(s32 handle)
{
    m_Flags[GetSlot(handle)] &= ~FLAG_PAUSE_CHILDREN;
}

// 0x005210F8 (name is ours)
void oml::framework::ProcessManager::PauseChildren(s32 handle)
{
    m_Flags[GetSlot(handle)] |= FLAG_PAUSE_CHILDREN;
}

// 0x0052112C (name is ours)
void oml::framework::ProcessManager::Update()
{
    u8& step = m_Step;
    if (!(m_DisabledSteps & DISABLE_UPDATE)) {
        step = STEP_UPDATE;
        for (TreeNode* node = m_Tree.m_pRoot; node != NULL;) {
            TreeNode* next = Tree::Next(node);
            UpdateProcess(node);
            node = next;
        }
    }
    if (!(m_DisabledSteps & DISABLE_FINALIZE)) {
        step = STEP_FINALIZE;
        FinalizeAll();
    }
    if (!(m_DisabledSteps & DISABLE_INITIALIZE)) {
        step = STEP_INITIALIZE;
        InitializeAll();
    }
    if (!(m_DisabledSteps & DISABLE_CALC)) {
        step = STEP_CALC;
        for (ListNode* node = m_CalcList.m_pHead; node != NULL;) {
            ListNode* next = node->next;
            if (!node->process->m_IsStopped && !(m_Flags[GetSlot(node->process->m_Handle)] & FLAG_PAUSED)) {
                RunCalc(node->process);
            }
            node = next;
        }
    }
    step = STEP_NONE;
}

// 0x00521298 (name is ours)
oml::framework::ProcessManager::ProcessManager()
{
    m_Step = STEP_UPDATE;
    m_DisabledSteps = 0;
    for (s32 i = 0; i < PROCESS_MAX; i++) {
        m_Flags[GetSlot(i)] = 0;
    }
    s_pInstance = this;
}

// 0x00521394 | libgarden [tier A]
const ProcessInfo* oml::framework::ProcessManager::GetInfo(ProcessIndex index)
{
    if (s_IndexCount > 0 && index < static_cast<u32>(s_IndexCount)) {
        return &s_pInfos[index];
    }
    return NULL;
}

// 0x005213C8 | libgarden [tier A]
Process* oml::framework::ProcessManager::CallCreate(ProcessIndex index)
{
    return s_pFactories[index].create();
}

// 0x005213DC | libgarden [tier A]
void oml::framework::ProcessManager::CallDestroy(ProcessIndex index, Process* process)
{
    s_pFactories[index].destroy(process);
}

// 0x005213F8 (name is ours)
void oml::framework::ProcessManager::SetTables(const ProcessInfo* infos, const ProcessFactory* factories, s32 count)
{
    s_pInfos = infos;
    s_pFactories = factories;
    s_IndexCount = count;
}

// 0x00521408 (name is ours)
bool oml::framework::ProcessManager::List::PushFront(ListNode* node)
{
    if (node == NULL) {
        return false;
    }
    if (m_pHead != NULL) {
        m_pHead->prev = node;
        node->next = m_pHead;
    } else {
        m_pTail = node;
    }
    m_pHead = node;
    return true;
}

// 0x00521438 (name is ours)
bool oml::framework::ProcessManager::List::Remove(ListNode* node)
{
    if (node == NULL) {
        return false;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else if (m_pHead == node) {
        m_pHead = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else if (m_pTail == node) {
        m_pTail = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
    return true;
}

// 0x005214A0 (name is ours)
bool oml::framework::ProcessManager::List::PushBack(ListNode* node)
{
    if (node == NULL) {
        return false;
    }
    if (m_pTail != NULL) {
        m_pTail->next = node;
        node->prev = m_pTail;
    } else {
        m_pHead = node;
    }
    m_pTail = node;
    return true;
}

// 0x005214D0 (name is ours)
bool oml::framework::ProcessManager::List::InsertSorted(ListNode* node)
{
    ListNode* cur = m_pHead;
    if (node == NULL) {
        return false;
    }
    if (cur == NULL) {
        if (m_pTail != NULL) {
            m_pTail->next = node;
            node->prev = m_pTail;
        } else {
            m_pHead = node;
        }
        m_pTail = node;
        return true;
    }
    if (cur->priority > node->priority) {
        if (m_pHead != NULL) {
            m_pHead->prev = node;
            node->next = m_pHead;
        } else {
            m_pTail = node;
        }
        m_pHead = node;
        return true;
    }
    while (cur->next != NULL && cur->next->priority <= node->priority) {
        cur = cur->next;
    }
    node->next = cur->next;
    node->prev = cur;
    cur->next = node;
    if (node->next != NULL) {
        node->next->prev = node;
    } else {
        m_pTail = node;
    }
    return true;
}

// 0x00521600 (name is ours)
oml::framework::ProcessManager::List::List() : m_pHead(NULL), m_pTail(NULL)
{
}

// 0x0052177C (name is ours)
void oml::framework::ProcessManager::UpdateProcess(TreeNode* node)
{
    if (node->process->m_IsStopped) {
        node->process->m_IsStopped = false;
        if (node->process->m_State == Process::STATE_RUNNING) {
            m_CalcList.Remove(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
            m_DrawList.Remove(&m_DrawNodes[GetSlot(node->process->m_Handle)]);
        } else {
            m_InitializeList.Remove(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
        }
        ListNode* calcNode = &m_CalcNodes[GetSlot(node->process->m_Handle)];
        if (calcNode != NULL) {
            m_FinalizeList.PushFront(calcNode);
        }
        node->process->m_State = Process::STATE_STOPPING;
        for (TreeNode* child = m_TreeNodes[GetSlot(node->process->m_Handle)].child; child != NULL; child = child->next) {
            child->process->Stop();
        }
        return;
    }
    TreeNode* parentNode = m_TreeNodes[GetSlot(node->process->m_Handle)].parent;
    Process* parent = (parentNode != NULL) ? parentNode->process : NULL;
    const s32 handle = node->process->m_Handle;
    if (parent != NULL) {
        if ((m_Flags[GetSlot(parent->m_Handle)] & FLAG_PAUSE_CHILDREN) || (m_Flags[GetSlot(parent->m_Handle)] & FLAG_PAUSED)) {
            m_Flags[GetSlot(handle)] |= FLAG_PAUSED;
        } else if (m_Flags[GetSlot(handle)] & FLAG_PAUSED) {
            m_Flags[GetSlot(handle)] &= ~FLAG_PAUSED;
        }
        if ((m_Flags[GetSlot(parent->m_Handle)] & FLAG_HIDE_CHILDREN) || (m_Flags[GetSlot(parent->m_Handle)] & FLAG_HIDDEN)) {
            m_Flags[GetSlot(handle)] |= FLAG_HIDDEN;
        } else if (m_Flags[GetSlot(handle)] & FLAG_HIDDEN) {
            m_Flags[GetSlot(handle)] &= ~FLAG_HIDDEN;
        }
    }
    if (node->process->m_State == Process::STATE_RUNNING) {
        if (m_CalcNodes[GetSlot(node->process->m_Handle)].IsPriorityChanged()) {
            m_CalcList.Remove(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
            m_CalcNodes[GetSlot(node->process->m_Handle)].ApplyPriority();
            m_CalcList.InsertSorted(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
        }
        if (m_DrawNodes[GetSlot(node->process->m_Handle)].IsPriorityChanged()) {
            m_DrawList.Remove(&m_DrawNodes[GetSlot(node->process->m_Handle)]);
            m_DrawNodes[GetSlot(node->process->m_Handle)].ApplyPriority();
            m_DrawList.InsertSorted(&m_DrawNodes[GetSlot(node->process->m_Handle)]);
        }
        return;
    }
    if (node->process->m_State == Process::STATE_STOPPING) {
        return;
    }
    if (node->process->m_IsInitializationDeferred) {
        node->process->m_IsInitializationDeferred = false;
        m_InitializeList.PushBack(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
        return;
    }
    if (node->process->m_IsActivationPending) {
        node->process->m_IsActivationPending = false;
        m_CalcList.InsertSorted(&m_CalcNodes[GetSlot(node->process->m_Handle)]);
        m_DrawList.InsertSorted(&m_DrawNodes[GetSlot(node->process->m_Handle)]);
        node->process->SetState(Process::STATE_RUNNING);
    }
}

// 0x00522190 (name is ours)
bool oml::framework::ProcessManager::Tree::Add(TreeNode* node, TreeNode* parent)
{
    if (node == NULL) {
        return false;
    }
    if (parent != NULL) {
        node->parent = parent;
        if (parent->child == NULL) {
            parent->child = node;
        } else {
            TreeNode* last = parent->child;
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = node;
            node->prev = last;
        }
    } else if (m_pRoot == NULL) {
        m_pRoot = node;
    } else {
        TreeNode* last = m_pRoot;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = node;
        node->prev = last;
    }
    return true;
}

// 0x0052221C (name is ours)
bool oml::framework::ProcessManager::Tree::Remove(TreeNode* node)
{
    if (node == NULL || node->child != NULL) {
        return false;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else if (node->parent != NULL) {
        node->parent->child = node->next;
    } else if (m_pRoot == node) {
        m_pRoot = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
    node->parent = NULL;
    return true;
}

// 0x00522298 (name is ours)
oml::framework::ProcessManager::Tree::Tree() : m_pRoot(NULL)
{
}

// 0x005222A4 (name is ours)
void oml::framework::ProcessManager::ListNode::SetPriority(u16 newValue)
{
    priority = newValue;
    newPriority = newValue;
}

// 0x005222B0 (name is ours)
void oml::framework::ProcessManager::ListNode::ApplyPriority()
{
    priority = newPriority;
}

// 0x005222BC (name is ours)
oml::framework::ProcessManager::ListNode::ListNode() : prev(NULL), next(NULL), process(NULL), priority(0), newPriority(0)
{
}

// 0x005222D8 (name is ours)
oml::framework::ProcessManager::TreeNode::TreeNode() : process(NULL), parent(NULL), child(NULL), prev(NULL), next(NULL)
{
}

// 0x007485A4 (name is ours; a copy for the game, inline in the original)
Process* oml::framework::ProcessManager::GetFirstChild(const Process* process) const
{
    TreeNode* child = m_TreeNodes[GetSlot(process->m_Handle)].child;
    if (child == NULL) {
        return NULL;
    }
    return child->GetProcess();
}

// 0x007485FC (name is ours; a copy, inline in the original)
s32 oml::framework::ProcessManager::AllocHandle()
{
    do {
        s_HandleCounter++;
    } while (m_TreeNodes[GetSlot(s_HandleCounter)].process != NULL);
    if (s_HandleCounter == -1) {
        // (all handles used: the original stops here)
        for (;;) {
        }
    }
    return s_HandleCounter;
}

// 0x0074866C (name is ours; a copy for the game, inline in the original)
bool oml::framework::ProcessManager::HasUninitializedDescendant(const Process* process) const
{
    TreeNode* end = Tree::NextSkipChildren(const_cast<TreeNode*>(&m_TreeNodes[GetSlot(process->m_Handle)]));
    for (TreeNode* node = m_TreeNodes[GetSlot(process->m_Handle)].child; node != NULL && node != end; node = Tree::Next(node)) {
        Process* descendant = node->process;
        if (descendant->m_State == Process::STATE_CREATED) {
            return descendant != NULL;
        }
    }
    return false;
}

// 0x00748748 (name is ours; a copy for the game, inline in the original)
u8 oml::framework::ProcessManager::IsPausingChildren(s32 handle) const
{
    return m_Flags[GetSlot(handle)] & FLAG_PAUSE_CHILDREN;
}

// 0x00748774 (name is ours; a copy for the game, inline in the original)
Process* oml::framework::ProcessManager::FindProcess(ProcessIndex index, const Process* parent) const
{
    TreeNode* start = NULL;
    if (parent != NULL) {
        start = const_cast<TreeNode*>(&m_TreeNodes[GetSlot(parent->m_Handle)]);
    }
    TreeNode* node = m_Tree.Find(index, start);
    if (node == NULL) {
        return NULL;
    }
    return node->GetProcess();
}

// 0x0074880C (name is ours; a copy, inline in the original)
s32 oml::framework::ProcessManager::Tree::Count(ProcessIndex index) const
{
    s32 count = 0;
    for (TreeNode* node = m_pRoot; node != NULL; node = Next(node)) {
        if (node->process->m_Index == index) {
            count++;
        }
    }
    return count;
}

// 0x0074885C (name is ours; a copy, inline in the original)
ProcessManager::TreeNode* oml::framework::ProcessManager::Tree::Find(ProcessIndex index, TreeNode* start) const
{
    TreeNode* node = (start == NULL) ? m_pRoot : Next(start);
    for (; node != NULL; node = Next(node)) {
        if (node->process->m_Index == index) {
            return node;
        }
    }
    return NULL;
}

// 0x007488DC (name is ours; a copy, inline in the original)
ProcessManager::TreeNode* oml::framework::ProcessManager::Tree::Next(TreeNode* node)
{
    if (node->child != NULL) {
        return node->child;
    }
    if (node->next != NULL) {
        return node->next;
    }
    for (TreeNode* parent = node->parent; parent != NULL; parent = parent->parent) {
        if (parent->next != NULL) {
            return parent->next;
        }
    }
    return NULL;
}

// 0x00748928 (name is ours; a copy, inline in the original)
ProcessManager::TreeNode* oml::framework::ProcessManager::Tree::NextSkipChildren(TreeNode* node)
{
    if (node->next != NULL) {
        return node->next;
    }
    for (TreeNode* parent = node->parent; parent != NULL; parent = parent->parent) {
        if (parent->next != NULL) {
            return parent->next;
        }
    }
    return NULL;
}

} // namespace framework
} // namespace oml
