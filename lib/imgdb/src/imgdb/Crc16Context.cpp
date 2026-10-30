#include "imgdb/Crc16Context.h"

namespace imgdb {

// 0x00982ED1
bool imgdb::Crc16Context::s_IsTableReady;
// 0x00AF8008
u16 imgdb::Crc16Context::s_Table[256];

// 0x005A57D8
// 0x005A57D4 (deleting dtor)
imgdb::Crc16Context::~Crc16Context()
{
    // empty (in the original too)
}

} // namespace imgdb
