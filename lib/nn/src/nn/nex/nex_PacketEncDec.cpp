#include "nn/nex/nex_PacketEncDec.h"

namespace nn {
namespace nex {
// 0x0035D6D4 | fefates:bytes [tier B]
void nn::nex::PacketEncDec::GenerateKey(const nn::nex::Key&, const unsigned char*, const unsigned char*, nn::nex::Key*)
{
}

// 0x0035DA2C | fefates:bytes [tier B]
void nn::nex::PacketEncDec::ReinitUserReliableSubStreams()
{
}

// 0x0035E774 | fefates:bytes [tier B]
nn::nex::PacketEncDec::~PacketEncDec()
{
}

} // namespace nex
} // namespace nn
