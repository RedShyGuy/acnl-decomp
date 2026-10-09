#include "nn/cec/CTR/CTR_Api.h"
#include <string.h>
#include "nn/fnd/fnd_ExpHeapTemplate.h"

namespace nn {
namespace cec {
namespace CTR {
typedef nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> > Heap;

namespace {
// the length of a title id in the message box list (8 digits and the terminator)
const size_t TITLE_ID_STRING_SIZE = 9;
} // namespace

// the heap of the library (made by the static initializer 0x0078507C; this program never
// initializes it, so FinalizeAllocFunc only resets it; names are ours)
// 0x00AE946C
Heap s_Heap;
// 0x00AE94D0
Heap::Allocator s_HeapAllocator;
// 0x0097E824
bool s_IsHeapInitialized = false;
// 0x0097E828
nn::fnd::IAllocator* s_pAllocator = NULL;

// 0x001409BC | nintendogs:callgraph [tier A]
void os_free(void* p)
{
    if (p != NULL && s_pAllocator != NULL) {
        s_pAllocator->Free(p);
    }
}

// 0x00144BF0 | nintendogs:callgraph [tier A]
void* os_malloc(size_t size, s32 alignment)
{
    if (s_pAllocator != NULL) {
        return s_pAllocator->Allocate(size, alignment);
    }
    return NULL;
}

// 0x0034F3F8 | nintendogs:callseq-callee [tier C]
void SetAllocFunc(nn::fnd::IAllocator& allocator)
{
    s_pAllocator = &allocator;
}

// 0x0034F408 | fefates:bytes [tier B]
void FinalizeAllocFunc()
{
    if (s_IsHeapInitialized) {
        s_HeapAllocator.Finalize();
        s_Heap.Finalize();
        s_pAllocator = NULL;
        s_IsHeapInitialized = false;
    }
}

// 0x0034F468 | nintendogs:bytes [tier A]
u32 Base64Str2CecTitleId(const u8* str)
{
    char buffer[16] = {};
    memcpy(buffer, str, TITLE_ID_STRING_SIZE);
    u32 titleId = 0;
    for (const char* p = buffer; *p != '\0'; p++) {
        titleId <<= 4;
        s32 digit;
        if ('0' <= *p && *p <= '9') {
            digit = *p - '0';
        } else {
            digit = *p - 'a' + 10;
        }
        if (0 < digit && digit < 16) {
            titleId += digit;
        }
    }
    return titleId;
}

} // namespace CTR
} // namespace cec
} // namespace nn
