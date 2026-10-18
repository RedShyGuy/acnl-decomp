#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_SecurityContext.h"
#include "nn/nex/nex_ThreadVariable.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// (the original calls these instances out of line; defined in nex_SecurityContextManager.cpp; noipa:
// with noinline GCC still inlines the destructor into the tree functions)
template <>
DECOMP_NOIPA qList<SecurityContext>::~qList();
template <>
DECOMP_NOINLINE qList<SecurityContext>::qList(const qList<SecurityContext>& other);
template <>
DECOMP_NOINLINE qList<SecurityContext>& qList<SecurityContext>::operator=(const qList<SecurityContext>& other);
} // namespace nex
} // namespace nn

namespace std {
template <>
DECOMP_NOINLINE nn::nex::qList<nn::nex::SecurityContext>& map<u32, nn::nex::qList<nn::nex::SecurityContext>, less<u32>, nn::nex::MemAllocator<pair<const u32, nn::nex::qList<nn::nex::SecurityContext> > > >::operator[](const u32& key);
} // namespace std

namespace nn {
namespace nex {
// The SecurityContexts of the calls being processed, a stack per thread (the current one first).
// The names of the members and of GetInstance are ours.
// RTTI N2nn3nex22SecurityContextManagerE @ 0x008CEC5C
// vtable 0x008FE1B0 (vptr 0x008FE1B8), offset_to_top 0, 2 entries
class SecurityContextManager : public ::nn::nex::RootObject
{
public:
    SecurityContextManager(); // 0x0039E014
    virtual ~SecurityContextManager(); // 0x0039E1E4 slot 0x00
    // 0x0039E1B0 slot 0x04 (deleting dtor)

    // the manager of the core of the current context (symbols.json names this
    // nn::cec::CTR::MessageBox::GetMessageGroupId)
    static SecurityContextManager* GetInstance(); // 0x0039DE20 (name is ours)
    // the ids of the current call (0 if there is none)
    static u32 StaticGetCurrentCID(); // 0x0039DE34 | fefates:bytes [tier B]
    static u32 StaticGetCurrentPID(); // 0x0039DE7C | fefates:bytes [tier B]
    void Pop(); // 0x0039DEC4 | mk7dlp:bytes [tier A]
    void Push(unsigned int principalId, unsigned int connectionId, unsigned int address); // 0x0039DF20 | mk7dlp:callseq [tier A]
    // the address of the station of the current call (0 if there is none; inline in the original,
    // with a copy out of line)
    u32 GetCurrentAddress() const; // 0x0072D0B0 | fefates:bytes [tier B]

    ThreadVariable<qList<SecurityContext> > m_Contexts; // 0x04
};
} // namespace nex
} // namespace nn
