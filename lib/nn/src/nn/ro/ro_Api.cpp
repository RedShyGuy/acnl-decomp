#include "nn/err/CTR/CTR_Api.h"
#include "nn/ro/detail/detail_Api.h"
#include "nn/ro/detail/ro_LdrRoClient.h"
#include "nn/ro/ro_Module.h"
#include "nn/ro/ro_Module_EnumerateCallback.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"
#include <stdlib.h>
#include <string.h>

namespace nn {
namespace ro {
namespace detail {
namespace {
const nn::Handle CURRENT_PROCESS(nn::PSEUDO_HANDLE_CURRENT_PROCESS);
const nn::Handle INVALID_HANDLE;
const char SERVICE_NAME[] = "ldr:ro";

// the start and the size of the code of the program (linker symbols in the original)
const uptr CODE_ADDRESS = 0x00100000;
const size_t CODE_SIZE = 0x007398A0;
// ldr:ro maps heap memory (from HEAP_ADDRESS on) behind the program (from PROGRAM_END on)
const uptr HEAP_ADDRESS = 0x08000000;
const uptr PROGRAM_END = 0x00AFA000;

// the key of FindExceptionIndex (names are ours)
struct ExceptionIndexKey
{
    uptr m_Address;
    const ExceptionIndexEntry* m_TableEnd;
};

// an address relative to itself in 31 bits (the entries of the exception index table)
inline uptr DecodePrel31(const u32* p)
{
    return reinterpret_cast<uptr>(p) + (static_cast<s32>(*p << 1) >> 1);
}

} // namespace

// 0x0097F058
bool s_IsDebugInfoAvailable;
// the module of the program (its exception index table) is the first node
// 0x0097F038 (name is ours)
ExceptionIndexNode s_ProgramExceptionIndex = {NULL, NULL, CODE_ADDRESS, 0x008398A0, NULL, NULL};
// 0x0097F050
ExceptionIndexList s_ExceptionIndexList = {&s_ProgramExceptionIndex, &s_ProgramExceptionIndex};
// 0x00975F98
State s_State;

// 0x0011F6AC (name is ours)
uptr GetCodeSize()
{
    return CODE_SIZE;
}

// 0x0012427C | fefates:bytes [tier B]
bool IsCodeAddress(uptr address)
{
    // the module of the address: is it code (the class name is from the binary)
    class CodeChecker : public Module::EnumerateCallback
    {
    public:
        explicit CodeChecker(uptr address) : m_Address(address), m_IsCode(false), m_IsChecked(false) {}

        // 0x0077A4B8 slot 0x00 [local class IsCodeAddress(unsigned int)::CodeChecker] (name is ours)
        virtual bool operator()(Module* module)
        {
            m_IsChecked = true;
            RegionInfo info;
            module->GetRegionInfo(&info);
            if (info.m_CodeAddress <= m_Address && m_Address < info.m_CodeAddress + info.m_CodeSize) {
                m_IsCode = true;
                return false;
            }
            return true;
        }

        uptr m_Address;     // 0x4
        bool m_IsCode;      // 0x8
        bool m_IsChecked;   // 0x9, a module was there
    };

    CodeChecker checker(address);
    Module::Enumerate(&checker);
    if (checker.m_IsChecked) {
        return checker.m_IsCode;
    }
    uptr code = GetCodeAddress();
    return code <= address && address < code + GetCodeSize();
}

// 0x001299C8 (name is ours)
uptr GetCodeAddress()
{
    return CODE_ADDRESS;
}

// 0x00129B94 (name is ours)
void SetDebugInfoAvailable(bool isAvailable)
{
    s_IsDebugInfoAvailable = isAvailable;
}

// 0x0012E54C (name is ours)
const ExceptionIndexEntry* FindExceptionIndex(uptr address)
{
    for (ExceptionIndexNode* node = s_ExceptionIndexList.m_Head; node != NULL; node = node->m_Next) {
        if (node->m_CodeBegin <= address && address < node->m_CodeEnd) {
            ExceptionIndexKey key;
            key.m_Address = address;
            key.m_TableEnd = node->m_TableEnd;
            return static_cast<const ExceptionIndexEntry*>(bsearch(&key, node->m_TableBegin, node->m_TableEnd - node->m_TableBegin,
                                                                   sizeof(ExceptionIndexEntry), CompareExceptionIndex));
        }
    }
    return NULL;
}

// 0x0012E5D0 (name is ours)
nn::Result Initialize(uptr crs, size_t size)
{
    if (s_Session != INVALID_HANDLE) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    nn::Result result = nn::srv::GetServiceHandle(&s_Session, SERVICE_NAME, strlen(SERVICE_NAME), 0);
    if (result.IsFailure()) {
        return result;
    }
    uptr mapped = crs;
    if (crs >= HEAP_ADDRESS) {
        mapped = crs - (HEAP_ADDRESS - PROGRAM_END);
    }
    result = LdrRoClient::Initialize(CURRENT_PROCESS, crs, size, mapped);
    if (result.IsFailure()) {
        nn::err::CTR::ThrowFatalErrAllIfFailure(nn::svc::CloseHandle(s_Session));
        s_Session = INVALID_HANDLE;
        return result;
    }
    s_State.m_StaticModule = reinterpret_cast<Module*>(mapped);
    SetDebugInfoAvailable(false);
    return result;
}

// 0x001309D0 (name is ours)
int CompareExceptionIndex(const void* key, const void* entry)
{
    const ExceptionIndexKey* indexKey = static_cast<const ExceptionIndexKey*>(key);
    const ExceptionIndexEntry* indexEntry = static_cast<const ExceptionIndexEntry*>(entry);
    // the function of the entry reaches up to the one of the next entry
    uptr next = (indexEntry + 1 == indexKey->m_TableEnd) ? 0xFFFFFFFF : DecodePrel31(&indexEntry[1].m_Function);
    if (indexKey->m_Address < DecodePrel31(&indexEntry->m_Function)) {
        return -1;
    }
    return next <= indexKey->m_Address ? 1 : 0;
}

// 0x00130A1C (name is ours)
Module* GetStaticModule()
{
    return s_State.m_StaticModule;
}

// 0x00136650 (name is ours)
nn::Result Finalize()
{
    if (s_Session == INVALID_HANDLE) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result result;
    Module* staticModule = s_State.m_StaticModule;
    if (staticModule != NULL) {
        // the loaded modules, from the last one on
        Module* first = staticModule->m_Next;
        if (first != NULL) {
            Module* module;
            do {
                module = first->m_Previous;
                result = module->Unload();
                if (result.IsFailure()) {
                    break;
                }
            } while (module != first);
        }
        if (result.IsSuccess()) {
            first = staticModule->m_Previous;
            if (first != NULL) {
                Module* module;
                do {
                    module = first->m_Previous;
                    result = module->Unload();
                    if (result.IsFailure()) {
                        break;
                    }
                } while (module != first);
            }
        }
        if (result.IsFailure()) {
            return result;
        }
    }
    RegistrationList* list = s_State.m_CurrentList;
    if (list != NULL) {
        while (list->m_Previous != NULL) {
            list = list->m_Previous;
        }
        while (list != NULL) {
            RegistrationList* next = list->m_Next;
            result = list->Unregister();
            if (result.IsFailure()) {
                break;
            }
            list = next;
        }
        if (result.IsFailure()) {
            return result;
        }
    }
    result = LdrRoClient::Shutdown(CURRENT_PROCESS, ToLoadAddress(reinterpret_cast<uptr>(s_State.m_StaticModule)));
    if (result.IsFailure()) {
        return result;
    }
    s_State.m_StaticModule = NULL;
    nn::err::CTR::ThrowFatalErrAllIfFailure(nn::svc::CloseHandle(s_Session));
    s_Session = INVALID_HANDLE;
    return result;
}

// 0x0013B388 (name is ours)
void SetCurrentList(RegistrationList* list)
{
    s_State.m_CurrentList = list;
}
} // namespace detail

// 0x00129B18 (name is ours)
void Module::Enumerate(EnumerateCallback* callback)
{
    Module* staticModule = detail::GetStaticModule();
    if (staticModule == NULL) {
        return;
    }
    for (Module* module = staticModule->m_Next; module != NULL; module = module->m_Next) {
        if (!(*callback)(module)) {
            return;
        }
    }
    for (Module* module = staticModule->m_Previous; module != NULL; module = module->m_Next) {
        if (!(*callback)(module)) {
            return;
        }
    }
}

// 0x0011FC70 (name is ours)
nn::Result Initialize(uptr crs, size_t size)
{
    // (the original passes IsCodeAddress to a function that armlink removed)
    return detail::Initialize(crs, size);
}

// 0x0011FC9C (name is ours)
RegistrationList* RegisterList(uptr crr, size_t size)
{
    if (detail::LdrRoClient::LoadCRR(detail::CURRENT_PROCESS, crr, size).IsFailure()) {
        return NULL;
    }
    RegistrationList* list = reinterpret_cast<RegistrationList*>(crr);
    detail::s_State.m_CurrentList = list;
    if (list->m_DebugInfoSize != 0) {
        detail::SetDebugInfoAvailable(true);
    }
    return list;
}

} // namespace ro
} // namespace nn
