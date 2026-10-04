#pragma once

// pead::PrintConfig - where pead's text output goes. The namespace and PrintEventArg are from the
// RTTI (pead::IDelegate1<pead::PrintConfig::PrintEventArg const&>); the rest is ours.

#include "decomp.h"
#include "pead/peadIDelegate1.h"

namespace pead {
namespace PrintConfig {

// one piece of text to print (pia's output delegate passes both members on; names are ours)
struct PrintEventArg
{
    const char* mString; // 0x0
    s32 mLength;         // 0x4
};
ASSERT_SIZE(PrintEventArg, 0x8);

// the delegate that gets the output (0 = the default output); name is ours
void SetPrintDelegate(IDelegate1<const PrintEventArg&>* delegate); // 0x00538354

// the default output (pia's output delegate falls back to it; name is ours)
void PrintDefault(const char* str, s32 length); // 0x0053C6EC
// prints through the delegate (name is ours)
void Print(const char* str, s32 length); // 0x0053C6D0

} // namespace PrintConfig
} // namespace pead
