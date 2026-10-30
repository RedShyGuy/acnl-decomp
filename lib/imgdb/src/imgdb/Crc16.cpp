#include "imgdb/Crc16.h"

namespace imgdb {

// (the context functions are inlined)
// 0x005B03B0 | nintendogs:bytes [tier B]
imgdb::Crc16::Crc16(const void* data, u32 size)
{
    if (!Crc16Context::s_IsTableReady)
    {
        Crc16Context::MakeTable();
    }
    m_Context.Crc16Context::Initialize();
    if (size != 0)
    {
        m_Context.Crc16Context::Update(data, size);
    }
}

// 0x005B04AC | nintendogs:callgraph [tier C]
imgdb::Crc16::~Crc16()
{
    m_Context.Crc16Context::Finalize();
}

} // namespace imgdb
