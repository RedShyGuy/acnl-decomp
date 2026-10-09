#include "nn/crypto/crypto_HashContextBase.h"
#include "imgdb/Crc16Context.h"

namespace imgdb {
// ctor candidate(s) 0x005B03B0 (unverified)
imgdb::Crc16Context::Crc16Context()
{
}

// 0x005A5718 slot 0x00
void imgdb::Crc16Context::Initialize()
{
}

// 0x005A57C8 slot 0x04
void imgdb::Crc16Context::Finalize()
{
}

// 0x005A5734 slot 0x08
void imgdb::Crc16Context::Update(const void*, size_t)
{
}

// 0x005A57BC slot 0x10
void imgdb::Crc16Context::GetHash(void*)
{
}

// 0x005A57D8
// 0x005A57D4 (deleting dtor)
imgdb::Crc16Context::~Crc16Context()
{
}

// 0x005A572C slot 0x20 | virtual slot, introduced by imgdb::Crc16Context
size_t imgdb::Crc16Context::vf_0x20()
{
}

} // namespace imgdb
