#include "nn/nex/nex_PacketBufferManager.h"

namespace nn {
namespace nex {
// (inline in the constructor of Network)
nn::nex::PacketBufferManager::PacketBufferManager()
    : PseudoSingleton(KIND), m_pAllocateFunction(nullptr), m_pAllocateObjectFunction(nullptr), m_Unknown0x1C(nullptr), m_Unknown0x20(0),
      m_Unknown0x24(0), m_Unknown0x28(8), m_Unknown0x2C(4), m_Unknown0x30(0.0f), m_IsInitialized(false), m_pPool(nullptr), m_pPool2(nullptr)
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00394188
// 0x00394140 (deleting dtor)
nn::nex::PacketBufferManager::~PacketBufferManager()
{
    m_Unknown0x24 = 0;
    Finalize();
}

} // namespace nex
} // namespace nn
