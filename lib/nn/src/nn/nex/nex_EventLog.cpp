#include "nn/nex/nex_EventLog.h"
#include <cwchar>

namespace nn {
namespace nex {
// 0x003D4EBC (name is ours)
void nn::nex::EventLog::AppendLevel(wchar_t* pBuffer, u32 size, u8 level)
{
    if (level == LEVEL_ALWAYS) {
        return;
    }
    const wchar_t* pName;
    switch (level) {
    case LEVEL_DEBUG:
        pName = L"Debug";
        break;
    case LEVEL_INFO:
        pName = L"Info";
        break;
    case LEVEL_WARNING:
        pName = L"Warning";
        break;
    case LEVEL_ERROR:
        pName = L"Error";
        break;
    case LEVEL_FATAL:
        pName = L"Fatal";
        break;
    case LEVEL_ALWAYS:
        pName = L"Always";
        break;
    default:
        pName = L"Unknown";
        break;
    }
    if (pName == nullptr) {
        return;
    }
    swprintf(pBuffer + wcslen(pBuffer), size, L"%ls|", pName);
}

// 0x003D5000
// 0x003D4F84 (deleting dtor)
nn::nex::EventLog::~EventLog()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
