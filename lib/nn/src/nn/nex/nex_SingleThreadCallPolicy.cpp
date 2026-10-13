#include "nn/nex/nex_SingleThreadCallPolicy.h"
#include "nn/nex/nex_ThreadVariableRoot.h"

namespace nn {
namespace nex {
// 0x0039E200 | mk7dlp:bytes [tier A]
void nn::nex::SingleThreadCallPolicy::EndCallImpl()
{
    if (m_CallDepth == 1) {
        m_ThreadId = 0;
    }
    m_CallDepth--;
}

// 0x0039E21C
void nn::nex::SingleThreadCallPolicy::BeginCallImpl()
{
    if (m_CallDepth == 0) {
        m_ThreadId = ThreadVariableRoot::GetCurrentThreadId();
    }
    m_CallDepth++;
}

// 0x0039E264 | mk7dlp:bytes [tier A]
nn::nex::SingleThreadCallPolicy::SingleThreadCallPolicy() : m_CallDepth(0), m_ThreadId(0), m_Unknown0x0C(1), m_Unknown0x10(0)
{
}

// 0x0039E2A4
// 0x0039E28C (deleting dtor)
nn::nex::SingleThreadCallPolicy::~SingleThreadCallPolicy()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
