#include <cstring>

#include "imgdb/ValidityStateTable.h"

namespace imgdb {

// (the header and a byte per record, in words)
// 0x005AB360 | nintendogs:callgraph [tier C]
u32 imgdb::ValidityStateTable::ComputeTableSize(int recordNum)
{
    return (recordNum + 4 + 3) & ~3;
}

// 0x005AB36C | nintendogs:bytes [tier B]
bool imgdb::ValidityStateTable::SetInitialTableData(void* data, u32 size, int recordNum)
{
    m_pData = static_cast<u8*>(data);
    m_Size = size;
    if (data == NULL || size == 0)
    {
        return true;
    }
    if (ComputeTableSize(recordNum) > size)
    {
        return false;
    }

    std::memset(data, 0, size);
    *reinterpret_cast<u32*>(m_pData) = 0;
    *reinterpret_cast<u16*>(m_pData) = recordNum;
    for (int i = 0; i < recordNum; i++)
    {
        GetRecords()[i].m_State = 0;
    }
    return true;
}

// 0x005AB3EC | nintendogs:callgraph [tier C]
imgdb::ValidityStateTable::ValidityStateTable() : m_pData(NULL), m_Size(0)
{
}

// 0x005AB858 | nintendogs:callgraph [tier C]
void imgdb::ValidityStateTableRecord::Initialize()
{
    m_State = 0;
}

} // namespace imgdb
