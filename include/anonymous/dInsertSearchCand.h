#pragma once

#include "decomp.h"
#include "Search/dSearchCand.h"

// the class is local to script's insert code (RTTI: (anonymous namespace)::d_script_insert::InsertSearchCand<256> of
// dUnityScript.cpp); it only gives the candidates a type of their own
namespace {
namespace d_script_insert {
// RTTI @ typeinfo 0x00895C64, vtable 0x00895C58
template <int N>
class InsertSearchCand : public SearchCand<N>
{
};
} // namespace d_script_insert
} // namespace
