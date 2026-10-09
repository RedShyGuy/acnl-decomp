#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/ro/ro_Module.h"

namespace nn {
namespace ro {
namespace detail {
// level, summary, module 75 (RO) and description after 3dbrew
// (usage, invalid state, already initialized)
const bit32 RESULT_ALREADY_INITIALIZED = 0xE0A12FF9;
// (usage, invalid state, not initialized)
const bit32 RESULT_NOT_INITIALIZED = 0xE0A12FF8;
// (status, not found, 5): the module has no exception index node
const bit32 RESULT_NO_EXCEPTION_INDEX = 0xC9612C05;
// (permanent, invalid argument, 4): the module has no control object
const bit32 RESULT_NO_CONTROL_OBJECT = 0xD8E12C04;

// the commands of the control object of a module (names are ours)
enum ControlCommand
{
    CONTROL_GET_EXCEPTION_INDEX = 0,    // out: ExceptionIndexNode*
    CONTROL_SET_FUNCTION_LIST = 1,      // out: the FunctionList storage
    CONTROL_GET_FUNCTION_LIST = 2,      // out: FunctionList*
    CONTROL_GET_CONSTRUCTORS = 3,       // out: FunctionRange
    CONTROL_GET_CONSTRUCTORS_2 = 4      // out: FunctionRange
};
typedef nn::Result (*ControlObject)(void* out, u32 command);

// an entry of an exception index table (ARM EHABI; names are ours)
struct ExceptionIndexEntry
{
    u32 m_Function;     // prel31 offset to the function
    u32 m_Data;
};

// the code of a module and its exception index table, in a list for the unwinder (names are
// ours); the first node is the one of the program
struct ExceptionIndexNode
{
    ExceptionIndexNode* m_Previous;     // 0x00
    ExceptionIndexNode* m_Next;         // 0x04
    uptr m_CodeBegin;                   // 0x08
    uptr m_CodeEnd;                     // 0x0C
    const ExceptionIndexEntry* m_TableBegin;    // 0x10
    const ExceptionIndexEntry* m_TableEnd;      // 0x14
};
ASSERT_SIZE(ExceptionIndexNode, 0x18);

struct ExceptionIndexList
{
    ExceptionIndexNode* m_Head;         // 0x0
    ExceptionIndexNode* m_Tail;         // 0x4
};

// the functions registered for the end of a module (an object and its destructor); names are
// ours
struct ExitFunction
{
    void* m_Argument;
    void (*m_Function)(void*);
};

struct FunctionList
{
    ExitFunction* m_Functions;  // 0x0
    s32 m_Count;                // 0x4
};

struct FunctionRange
{
    void (**m_Begin)();
    void (**m_End)();
};

// the static module and the current registration list (names are ours)
struct State
{
    Module* m_StaticModule;             // 0x0, as mapped by ldr:ro
    RegistrationList* m_CurrentList;    // 0x4
};

// the debug information of a registration list: the names of the modules by hash (names are
// ours)
struct DebugInfoTable
{
    u32 m_EntriesOffset;
    s32 m_EntryCount;
};

// what the debugger gets at svc Break for a loaded / unloaded module (names are ours)
struct ModuleNotification
{
    const char* m_Name;
    size_t m_NameSize;
    const Module* m_Module;
    uptr m_DataAddress;
};

const u32 BREAK_REASON_LOAD_RO = 3;
const u32 BREAK_REASON_UNLOAD_RO = 4;

// (defined in a .cpp, with its address)
extern State s_State;
// (defined in a .cpp, with its address)
extern ExceptionIndexList s_ExceptionIndexList;
// (defined in a .cpp, with its address)
extern bool s_IsDebugInfoAvailable;

DECOMP_NOIPA uptr GetCodeSize(); // 0x0011F6AC (name is ours)
bool IsCodeAddress(uptr address); // 0x0012427C | fefates:bytes [tier B]
DECOMP_NOIPA uptr GetCodeAddress(); // 0x001299C8 (name is ours)
DECOMP_NOINLINE void SetDebugInfoAvailable(bool isAvailable); // 0x00129B94 (name is ours)
// the exception index entry of the function at address (for the unwinder)
const ExceptionIndexEntry* FindExceptionIndex(uptr address); // 0x0012E54C (name is ours)
DECOMP_NOINLINE nn::Result Initialize(uptr crs, size_t size); // 0x0012E5D0 (name is ours)
int CompareExceptionIndex(const void* key, const void* entry); // 0x001309D0 (name is ours)
Module* GetStaticModule(); // 0x00130A1C (name is ours)
nn::Result Finalize(); // 0x00136650 (name is ours)
void SetCurrentList(RegistrationList* list); // 0x0013B388 (name is ours)
nn::Result UnregisterExceptionIndex(Module* module); // 0x0013E8F8 (name is ours)
// heap addresses (from 0x08000000 on) as ldr:ro maps them behind the program and back
DECOMP_NOINLINE uptr ToLoadAddress(uptr address); // 0x0013E97C (name is ours)
void NotifyUnload(Module* module); // 0x0013E998 (name is ours)
DECOMP_NOINLINE nn::Result CallControlObject(Module* module, void* out, u32 command); // 0x001406A0 (name is ours)
// the registration list with the hash of the module header; the index of the hash or -1
int FindHash(RegistrationList** list, const void* hash); // 0x00140704 (name is ours)
nn::Result RegisterExceptionIndex(Module* module); // 0x0034CD08 (name is ours)
void MakeRegionInfo(RegionInfo* info, const Module* module); // 0x0034CF64 (name is ours)
void GetFixedSizes(SizeInfo* info, const Module* cro); // 0x0034D06C (name is ours)
void NotifyLoad(Module* module); // 0x0034D1E0 (name is ours)
void SetUpFunctionList(Module* module, void* buffer, size_t bufferSize); // 0x0034D2C4 (name is ours)
// svc Break for the debugger (the original falls into its svc Break function at 0x00135C84)
DECOMP_NOINLINE void BreakUnloadRo(const void* data, size_t size); // 0x00135C74 (name is ours)
DECOMP_NOINLINE void BreakLoadRo(const void* data, size_t size); // 0x00351028 (name is ours)
} // namespace detail
} // namespace ro
} // namespace nn
