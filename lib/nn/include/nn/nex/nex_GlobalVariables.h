#pragma once

#include "decomp.h"
#include "nn/nex/nex_CacheManager.h"
#include "nn/nex/nex_EventLog.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_ThreadVariable.h"
#include "nn/nex/nex_TraceLog.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class ObjectThreadRoot;

// The global state of nex, shared by its users (a reference count). The names of the members and
// of the functions other than AcquireInstance are ours.
// RTTI N2nn3nex15GlobalVariablesE @ 0x008CE49C
// vtable 0x008FCE90 (vptr 0x008FCE98), offset_to_top 0, 2 entries
class GlobalVariables : public ::nn::nex::RefCountedObject
{
public:
    GlobalVariables(); // 0x00377810 | fefates:callseq [tier B]
    virtual ~GlobalVariables(); // 0x00377B84 slot 0x00 | fefates:callseq
    // 0x00377B54 slot 0x04 (deleting dtor)

    // makes the instance (with the ThreadVariableList) or takes a reference to it
    static void AcquireInstance(); // 0x0037765C | fefates:callseq [tier B]
    // releases a reference; the last one deletes the instance and the ThreadVariableList
    static void ReleaseInstance(); // 0x00377750

    String m_BroadcastAddress;                                    // 0x00C
    ThreadVariable<u32> m_LastError;                              // 0x014 (SystemError)
    ThreadVariable<u32> m_LastErrorDetail;                        // 0x068 (SystemError)
    ThreadVariable<ObjectThreadRoot*> m_CurrentThread;            // 0x0BC
    qMap<String, qVector<u8> > m_RootCaCache;                     // 0x110 (RootCaReader: the certificates by file name)
    EventLog m_EventLog;                                          // 0x12C
    TraceLog m_TraceLog;                                          // 0x148
    CacheManager m_CacheManager;                                  // 0x160
    std::vector<PacketBufferManager::UnitHeapParam> m_UnitHeapParams; // 0x180
    bool m_Unknown0x18C;                                          // 0x18C

    static GlobalVariables* s_pInstance;
    static ForcedCriticalSection s_Lock;
};
ASSERT_SIZE(GlobalVariables, 0x190);
} // namespace nex
} // namespace nn

namespace std {
// (out of line in the original; nex_GlobalVariables.cpp)
template <>
DECOMP_NOINLINE pair<map<nn::nex::String, nn::nex::qVector<u8> >::iterator, bool> map<nn::nex::String, nn::nex::qVector<u8> >::insert(const value_type& value);
} // namespace std
