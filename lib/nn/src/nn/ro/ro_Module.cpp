#include "nn/crypto/crypto_Api.h"
#include "nn/os/os_Atomic.h"
#include "nn/ro/detail/detail_Api.h"
#include "nn/ro/detail/ro_LdrRoClient.h"
#include "nn/ro/ro_Module.h"
#include "nn/svc/svc_Api.h"
#include <string.h>

namespace nn {
namespace ro {
namespace detail {
namespace {
const nn::Handle CURRENT_PROCESS(nn::PSEUDO_HANDLE_CURRENT_PROCESS);
// ldr:ro maps heap memory (from HEAP_ADDRESS on) behind the program (from PROGRAM_END on)
const uptr HEAP_ADDRESS = 0x08000000;
const uptr PROGRAM_END = 0x00AFA000;
const size_t PAGE_SIZE = 0x1000;

// a hash of the registration list (SHA-256 of the first 0x80 bytes of a module)
struct Hash
{
    u8 m_Data[32];
};

inline uptr AlignUp(uptr value, uptr alignment)
{
    return (value + alignment - 1) & ~(alignment - 1);
}

// the name of a module in the debug information of its registration list
DECOMP_ALWAYS_INLINE void Notify(Module* module, bool isLoad)
{
    if (!s_IsDebugInfoAvailable) {
        return;
    }
    Hash hash;
    nn::crypto::CalculateSha256(&hash, module, sizeof(module->m_Hashes));
    RegistrationList* list;
    int index = FindHash(&list, &hash);
    if (index < 0) {
        return;
    }
    if (list->m_DebugInfoSize == 0) {
        return;
    }
    const DebugInfoTable* table = reinterpret_cast<const DebugInfoTable*>(reinterpret_cast<uptr>(list) + list->m_DebugInfoOffset);
    if (table->m_EntryCount <= index) {
        return;
    }
    const u32* entries = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(list) + table->m_EntriesOffset);
    const u32* nameInfo = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(list) + entries[index * 2]);
    const char* name = reinterpret_cast<const char*>(list) + nameInfo[0];
    if (name == NULL) {
        return;
    }
    ModuleNotification notification;
    notification.m_Name = name;
    notification.m_NameSize = nameInfo[1] + 1;
    notification.m_Module = module;
    uptr data = 0;
    const Segment* segments = reinterpret_cast<const Segment*>(module->m_SegmentTableOffset);
    for (s32 i = 0; i < module->m_SegmentTableCount; i++) {
        if (segments[i].m_Type == SEGMENT_TYPE_DATA) {
            data = segments[i].m_Offset;
            break;
        }
    }
    notification.m_DataAddress = data;
    if (isLoad) {
        BreakLoadRo(&notification, sizeof(notification));
    } else {
        BreakUnloadRo(&notification, sizeof(notification));
    }
}
} // namespace

// 0x00135C74 (name is ours)
void BreakUnloadRo(const void* data, size_t size)
{
    nn::svc::Break(BREAK_REASON_UNLOAD_RO, data, size);
}

// 0x0013E8F8 (name is ours)
nn::Result UnregisterExceptionIndex(Module* module)
{
    ExceptionIndexNode* node = NULL;
    if (CallControlObject(module, &node, CONTROL_GET_EXCEPTION_INDEX).IsFailure() || node == NULL) {
        return nn::Result(RESULT_NO_EXCEPTION_INDEX);
    }
    if (s_ExceptionIndexList.m_Tail == node) {
        s_ExceptionIndexList.m_Tail = node->m_Previous;
    }
    if (node->m_Next != NULL) {
        node->m_Next->m_Previous = node->m_Previous;
    }
    node->m_Previous->m_Next = node->m_Next;
    node->m_Next = NULL;
    node->m_Previous = NULL;
    node->m_CodeBegin = 0;
    node->m_CodeEnd = 0;
    return nn::Result();
}

// 0x0013E97C (name is ours)
uptr ToLoadAddress(uptr address)
{
    if (address >= PROGRAM_END) {
        address += HEAP_ADDRESS - PROGRAM_END;
    }
    return address;
}

// 0x0013E998 (name is ours)
void NotifyUnload(Module* module)
{
    Notify(module, false);
}

// 0x001406A0 (name is ours)
nn::Result CallControlObject(Module* module, void* out, u32 command)
{
    uptr controlObject = module->GetSegmentAddress(module->m_ControlObject);
    if (controlObject == 0) {
        return nn::Result(RESULT_NO_CONTROL_OBJECT);
    }
    return reinterpret_cast<ControlObject>(controlObject)(out, command);
}

// 0x00140704 (name is ours)
int FindHash(RegistrationList** found, const void* hash)
{
    for (RegistrationList* list = s_State.m_CurrentList; list != NULL; list = list->m_Next) {
        const Hash* hashes = reinterpret_cast<const Hash*>(reinterpret_cast<uptr>(list) + list->m_HashTableOffset);
        const Hash* end = hashes + list->m_HashCount;
        // the first hash that is not smaller (the hashes are sorted)
        const Hash* first = hashes;
        s32 count = end - first;
        while (count > 0) {
            s32 half = count / 2;
            const Hash* middle = first + half;
            if (memcmp(middle, hash, sizeof(Hash)) < 0) {
                first = middle + 1;
                count = count - half - 1;
            } else {
                count = half;
            }
        }
        if (first != end) {
            int index = first - hashes;
            if (index != -1) {
                *found = list;
                return index;
            }
        }
    }
    return -1;
}

// 0x0034CD08 (name is ours)
nn::Result RegisterExceptionIndex(Module* module)
{
    ExceptionIndexNode* node = NULL;
    if (CallControlObject(module, &node, CONTROL_GET_EXCEPTION_INDEX).IsFailure() || node == NULL) {
        return nn::Result(RESULT_NO_EXCEPTION_INDEX);
    }
    node->m_CodeBegin = module->m_CodeOffset;
    node->m_CodeEnd = module->m_CodeOffset + module->m_CodeSize;
    ExceptionIndexNode* tail = s_ExceptionIndexList.m_Tail;
    node->m_Previous = tail;
    node->m_Next = NULL;
    nn::os::detail::DataMemoryBarrier();
    tail->m_Next = node;
    s_ExceptionIndexList.m_Tail = node;
    return nn::Result();
}

// 0x0034CF64 (name is ours)
void MakeRegionInfo(RegionInfo* info, const Module* module)
{
    uptr codeBegin = 0xFFFFFFFF;
    uptr codeEnd = 0;
    uptr dataBegin = 0xFFFFFFFF;
    uptr dataEnd = 0;
    const Segment* segments = reinterpret_cast<const Segment*>(module->m_SegmentTableOffset);
    for (s32 i = 0; i < module->m_SegmentTableCount; i++) {
        const Segment& segment = segments[i];
        if (segment.m_Size == 0) {
            continue;
        }
        switch (segment.m_Type) {
        case SEGMENT_TYPE_CODE:
            if (segment.m_Offset < codeBegin) {
                codeBegin = segment.m_Offset;
            }
            if (codeEnd < segment.m_Offset + segment.m_Size) {
                codeEnd = segment.m_Offset + segment.m_Size;
            }
            break;
        case SEGMENT_TYPE_DATA:
        case SEGMENT_TYPE_BSS:
            if (segment.m_Offset < dataBegin) {
                dataBegin = segment.m_Offset;
            }
            if (dataEnd < segment.m_Offset + segment.m_Size) {
                dataEnd = segment.m_Offset + segment.m_Size;
            }
            break;
        }
    }
    info->m_Module = module;
    info->m_Size = module->m_FixedSize;
    info->m_LoadAddress = ToLoadAddress(reinterpret_cast<uptr>(module));
    info->m_FixedSize = module->m_FixedSize;
    info->m_DataAddress = dataBegin;
    info->m_DataSize = dataEnd - dataBegin;
    info->m_CodeAddress = codeBegin;
    info->m_CodeSize = codeEnd - codeBegin;
}

// 0x0034D06C (name is ours)
void GetFixedSizes(SizeInfo* info, const Module* cro)
{
    // the end of the header tables that each fix level keeps
    size_t end = cro->m_CodeOffset + cro->m_CodeSize;
    if (end <= Module::HEADER_END) {
        end = Module::HEADER_END;
    }
    if (end < cro->m_ModuleNameOffset + cro->m_ModuleNameSize) {
        end = cro->m_ModuleNameOffset + cro->m_ModuleNameSize;
    }
    if (end < cro->m_SegmentTableOffset + cro->m_SegmentTableCount * sizeof(Segment)) {
        end = cro->m_SegmentTableOffset + cro->m_SegmentTableCount * sizeof(Segment);
    }
    size_t level3 = end;
    if (end < cro->m_NamedExportTableOffset + cro->m_NamedExportTableCount * 8) {
        end = cro->m_NamedExportTableOffset + cro->m_NamedExportTableCount * 8;
    }
    if (end < cro->m_IndexedExportTableOffset + cro->m_IndexedExportTableCount * 4) {
        end = cro->m_IndexedExportTableOffset + cro->m_IndexedExportTableCount * 4;
    }
    if (end < cro->m_ExportStringsOffset + cro->m_ExportStringsSize) {
        end = cro->m_ExportStringsOffset + cro->m_ExportStringsSize;
    }
    if (end < cro->m_ExportTrieOffset + cro->m_ExportTrieCount * 8) {
        end = cro->m_ExportTrieOffset + cro->m_ExportTrieCount * 8;
    }
    size_t level2 = end;
    if (end < cro->m_ImportModuleTableOffset + cro->m_ImportModuleTableCount * 8) {
        end = cro->m_ImportModuleTableOffset + cro->m_ImportModuleTableCount * 8;
    }
    if (end < cro->m_ImportRelocationsOffset + cro->m_ImportRelocationsCount * 12) {
        end = cro->m_ImportRelocationsOffset + cro->m_ImportRelocationsCount * 12;
    }
    if (end < cro->m_NamedImportTableOffset + cro->m_NamedImportTableCount * 8) {
        end = cro->m_NamedImportTableOffset + cro->m_NamedImportTableCount * 8;
    }
    if (end < cro->m_IndexedImportTableOffset + cro->m_IndexedImportTableCount * 8) {
        end = cro->m_IndexedImportTableOffset + cro->m_IndexedImportTableCount * 8;
    }
    if (end < cro->m_AnonymousImportTableOffset + cro->m_AnonymousImportTableCount * 8) {
        end = cro->m_AnonymousImportTableOffset + cro->m_AnonymousImportTableCount * 8;
    }
    if (end < cro->m_ImportStringsOffset + cro->m_ImportStringsSize) {
        end = cro->m_ImportStringsOffset + cro->m_ImportStringsSize;
    }
    size_t level1 = end;
    if (end < cro->m_UnknownRelocationsOffset + cro->m_UnknownRelocationsCount * 12) {
        end = cro->m_UnknownRelocationsOffset + cro->m_UnknownRelocationsCount * 12;
    }
    if (end < cro->m_Unknown120Offset + cro->m_Unknown120Count * 8) {
        end = cro->m_Unknown120Offset + cro->m_Unknown120Count * 8;
    }
    if (end < cro->m_InternalRelocationsOffset + cro->m_InternalRelocationsCount * 12) {
        end = cro->m_InternalRelocationsOffset + cro->m_InternalRelocationsCount * 12;
    }
    info->m_FixedSizes[0] = end;
    info->m_FixedSizes[1] = level1;
    info->m_FixedSizes[2] = level2;
    info->m_FixedSizes[3] = level3;
    info->m_BufferSize = 0;
}

// 0x0034D1E0 (name is ours)
void NotifyLoad(Module* module)
{
    Notify(module, true);
}

// 0x0034D2C4 (name is ours)
void SetUpFunctionList(Module* module, void* buffer, size_t bufferSize)
{
    // behind .bss (or .data), else at buffer
    uptr address = reinterpret_cast<uptr>(buffer);
    const Segment* segments = reinterpret_cast<const Segment*>(module->m_SegmentTableOffset);
    for (s32 i = 0; i < module->m_SegmentTableCount; i++) {
        const Segment& segment = segments[i];
        if (segment.m_Type == SEGMENT_TYPE_BSS) {
            address = AlignUp(segment.m_Offset + segment.m_Size, 4);
            break;
        }
        if (segment.m_Type == SEGMENT_TYPE_DATA) {
            address = AlignUp(segment.m_Offset + segment.m_Size, 4);
        }
    }
    CallControlObject(module, reinterpret_cast<void*>(address), CONTROL_SET_FUNCTION_LIST);
    FunctionList* list = reinterpret_cast<FunctionList*>(address);
    // (the functions start 32 bytes behind the list)
    list->m_Functions = reinterpret_cast<ExitFunction*>(address + 32);
    list->m_Count = 0;
}

// 0x00351028 (name is ours)
void BreakLoadRo(const void* data, size_t size)
{
    nn::svc::Break(BREAK_REASON_LOAD_RO, data, size);
}
} // namespace detail

namespace {
const nn::Handle CURRENT_PROCESS(nn::PSEUDO_HANDLE_CURRENT_PROCESS);
} // namespace

// 0x0013B170 (name is ours)
nn::Result RegistrationList::Unregister()
{
    RegistrationList* other = m_Previous;
    if (other == NULL) {
        other = m_Next;
    }
    nn::Result result = detail::LdrRoClient::UnloadCRR(CURRENT_PROCESS, reinterpret_cast<uptr>(this));
    if (result.IsSuccess()) {
        detail::SetCurrentList(other);
    }
    return result;
}

// 0x0013B1B4 (name is ours)
nn::Result Module::Unload()
{
    detail::NotifyUnload(this);
    detail::UnregisterExceptionIndex(this);
    detail::CallControlObject(this, NULL, detail::CONTROL_SET_FUNCTION_LIST);

    // .data goes back into the module file if ldr:ro moved it
    uptr data = 0;
    uptr original = 0;
    size_t size = 0;
    if (m_Magic == MAGIC) {
        const Segment* segments = reinterpret_cast<const Segment*>(m_SegmentTableOffset);
        for (s32 i = 0; i < m_SegmentTableCount; i++) {
            if (segments[i].m_Type == SEGMENT_TYPE_DATA) {
                data = segments[i].m_Offset;
                if (data != 0) {
                    original = (m_DataOffset - reinterpret_cast<uptr>(this)) + detail::ToLoadAddress(reinterpret_cast<uptr>(this));
                    if (original != data) {
                        size = m_DataSize;
                    }
                }
                break;
            }
        }
    }
    nn::Result result = detail::LdrRoClient::UnloadCRO(CURRENT_PROCESS, reinterpret_cast<uptr>(this), 0,
                                                       detail::ToLoadAddress(reinterpret_cast<uptr>(this)));
    if (result.IsFailure()) {
        return result;
    }
    if (size != 0) {
        memcpy(reinterpret_cast<void*>(original), reinterpret_cast<void*>(data), size);
    }
    // the segment table of the file is relative again
    Module* module = reinterpret_cast<Module*>(detail::ToLoadAddress(reinterpret_cast<uptr>(this)));
    for (s32 i = 0; i < module->m_SegmentTableCount; i++) {
        Segment& segment = reinterpret_cast<Segment*>(reinterpret_cast<uptr>(module) + module->m_SegmentTableOffset)[i];
        if (segment.m_Type == SEGMENT_TYPE_DATA) {
            segment.m_Offset = module->m_DataOffset;
        } else if (segment.m_Type == SEGMENT_TYPE_BSS) {
            segment.m_Offset = 0;
        }
    }
    return result;
}

// 0x0034CB08 (name is ours)
Module* Module::Load(const void* cro, size_t croSize, void* buffer, size_t bufferSize, bool isAutoLink, u32 fixLevel, uptr crr)
{
    const Module* file = static_cast<const Module*>(cro);
    if (file->m_Magic != MAGIC) {
        return NULL;
    }
    uptr address = reinterpret_cast<uptr>(cro);
    uptr mapped = address;
    if (address >= detail::HEAP_ADDRESS) {
        mapped = address - (detail::HEAP_ADDRESS - detail::PROGRAM_END);
    }
    // .data, then .bss in buffer
    size_t dataSize = detail::AlignUp(file->m_DataSize, 4);
    uptr bss = reinterpret_cast<uptr>(buffer) + dataSize;
    size_t bssSize = bufferSize - dataSize;
    size_t fixedSize;
    if (detail::LdrRoClient::LoadCRO(&fixedSize, detail::CURRENT_PROCESS, address, mapped, croSize, reinterpret_cast<uptr>(buffer), 0, dataSize,
                                     bss, bssSize, isAutoLink, fixLevel, crr)
            .IsFailure()) {
        return NULL;
    }
    Module* module = reinterpret_cast<Module*>(mapped);
    size_t size = module->m_DataSize;
    if (size != 0) {
        // the part of .data in the fixed (mapped) part comes from there, the rest from the file
        uptr data = module->m_DataOffset;
        uptr source = data - mapped + address;
        uptr fixedEnd = address + fixedSize;
        size_t mappedSize = 0;
        if (source < fixedEnd) {
            mappedSize = fixedEnd - source;
            if (size < mappedSize) {
                mappedSize = size;
            }
        }
        if (size > mappedSize) {
            memmove(static_cast<u8*>(buffer) + mappedSize, reinterpret_cast<const void*>(source + mappedSize), size - mappedSize);
        }
        if (mappedSize != 0) {
            memcpy(buffer, reinterpret_cast<const void*>(data), mappedSize);
        }
    }
    memset(reinterpret_cast<void*>(bss), 0, bssSize);
    detail::SetUpFunctionList(module, buffer, bufferSize);
    detail::RegisterExceptionIndex(module);
    detail::NotifyLoad(module);
    return module;
}

// 0x0034CC64 (name is ours)
nn::Result Module::GetSizeInfo(SizeInfo* info, const void* cro)
{
    const Module* file = static_cast<const Module*>(cro);
    detail::GetFixedSizes(info, file);
    for (s32 i = 0; i < 4; i++) {
        info->m_FixedSizes[i] = detail::AlignUp(info->m_FixedSizes[i], detail::PAGE_SIZE) + reinterpret_cast<uptr>(cro);
    }
    info->m_BufferSize = detail::AlignUp(file->m_DataSize, 8) + file->m_BssSize;
    return nn::Result();
}

// 0x0034CD7C (name is ours)
bool Module::DoFinalize()
{
    detail::FunctionList* list;
    if (detail::CallControlObject(this, &list, detail::CONTROL_GET_FUNCTION_LIST).IsFailure()) {
        list = NULL;
    }
    uptr onExit = GetSegmentAddress(m_OnExit);
    if (onExit != 0) {
        reinterpret_cast<void (*)()>(onExit)();
    }
    for (s32 i = list->m_Count - 1; i >= 0; i--) {
        list->m_Functions[i].m_Function(list->m_Functions[i].m_Argument);
    }
    list->m_Count = 0;
    return true;
}

// 0x0034CE10 (name is ours)
bool Module::DoInitialize()
{
    detail::FunctionRange range;
    if (detail::CallControlObject(this, &range, detail::CONTROL_GET_CONSTRUCTORS).IsSuccess()) {
        for (void (**function)() = range.m_Begin; function < range.m_End; function++) {
            (*function)();
        }
    }
    if (detail::CallControlObject(this, &range, detail::CONTROL_GET_CONSTRUCTORS_2).IsSuccess()) {
        for (void (**function)() = range.m_Begin; function < range.m_End; function++) {
            (*function)();
        }
    }
    uptr onLoad = GetSegmentAddress(m_OnLoad);
    if (onLoad != 0) {
        reinterpret_cast<void (*)()>(onLoad)();
    }
    return true;
}

// 0x0034CF54 (name is ours)
void Module::GetRegionInfo(RegionInfo* info) const
{
    detail::MakeRegionInfo(info, this);
}

} // namespace ro
} // namespace nn
