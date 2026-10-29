#include "pead/peadPrintConfig.h"

namespace pead {
namespace PrintConfig {
// 0x00AE8298
DelegateEvent<const PrintEventArg&> s_PrintEvent;
// 0x0097E428
IDelegate1<const PrintEventArg&>* s_pPrintDelegate = nullptr;

// constructs s_PrintEvent
// 0x0079A3A8 (static initializer)

// 0x005382D4 (name is ours)
void PrintArg(const PrintEventArg& arg)
{
    s_PrintEvent.fire(arg);
    if (s_pPrintDelegate != nullptr) {
        s_pPrintDelegate->invoke(arg);
    } else {
        PrintDefault(arg.mString, arg.mLength);
    }
}

// 0x00538354 (name is ours)
void SetPrintDelegate(IDelegate1<const PrintEventArg&>* delegate)
{
    s_pPrintDelegate = delegate;
}

// 0x0053C6D0 (name is ours)
void Print(const char* str, s32 length)
{
    PrintEventArg arg = {str, length};
    PrintArg(arg);
}

// 0x0053C6EC (name is ours)
void PrintDefault(const char*, s32)
{
    // empty (in the original too: the release build has no output)
}
} // namespace PrintConfig
} // namespace pead

// 0x0080FFD8
// 0x0080FF64 (deleting dtor)
template pead::DelegateEvent<const pead::PrintConfig::PrintEventArg&>::~DelegateEvent();
