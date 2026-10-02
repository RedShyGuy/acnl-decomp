#include "nn/nex/nex_NetworkEmulator.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::NetworkEmulator::NetworkEmulator()
{
}

// 0x0037A6AC | fefates:bytes [tier B]
void nn::nex::NetworkEmulator::Queue(nn::nex::Buffer*, nn::nex::InetAddress&)
{
}

// 0x0037A7C4 | fefates:bytes [tier B]
void nn::nex::NetworkEmulator::Dequeue(nn::nex::Buffer*)
{
}

// 0x0037AD74 | mk7dlp:callseq [tier A]
nn::nex::NetworkEmulator::NetworkEmulator(nn::nex::EmulationDevice*)
{
}

} // namespace nex
} // namespace nn
