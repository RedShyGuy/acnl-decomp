#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_SystemError.h"
#include <cstring>

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::LiteBuffer<16>  typeinfo 0x008CDF68  vtable 0x008FC150
//   nn::nex::LiteBuffer<20>  typeinfo 0x008CDF74  vtable 0x008FC160
//   nn::nex::LiteBuffer<32>  typeinfo 0x008CDF80  vtable 0x008FC170
//   nn::nex::LiteBuffer<52>  typeinfo 0x008CDF8C  vtable 0x008FC180
//   nn::nex::LiteBuffer<6>  typeinfo 0x008CDF98  vtable 0x008FC190
//   nn::nex::LiteBuffer<8>  typeinfo 0x008CDFA4  vtable 0x008FC1A0
//
// A buffer of at most SIZE bytes inside the object (all inline; LiteBuffer.h). The member names are
// ours.
template <int SIZE>
class LiteBuffer : public ::nn::nex::RootObject
{
public:
    LiteBuffer() : m_Size(0) {}
    virtual ~LiteBuffer() {}

    // copies size bytes to offset; false (and an error) beyond SIZE
    bool AppendData(const void* pData, u32 size, u32 offset)
    {
        u32 end = offset + size;
        if (end <= m_Size) {
            end = m_Size;
        }
        if (end > SIZE) {
            SystemError::SignalError(const_cast<wchar_t*>(L"D:\\home\\jenkins\\workspace\\Manual-Pack\\5129d543\\.\\OnlineCore/src/Core/LiteBuffer.h"), 79,
                                     SystemError::ERROR_BUFFER_OVERRUN, 0);
            return false;
        }
        m_Size = end;
        memcpy(m_Data + offset, pData, size);
        return true;
    }

    u32 m_Size;         // 0x04
    u8 m_Data[SIZE];    // 0x08
};
} // namespace nex
} // namespace nn
