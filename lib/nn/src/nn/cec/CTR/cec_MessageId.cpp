#include "nn/cec/CTR/cec_MessageId.h"
#include <string.h>

namespace nn {
namespace cec {
namespace CTR {
// 0x00350A84 | nintendogs:callgraph [tier A]
nn::cec::CTR::MessageId::MessageId(const u8* p)
{
    if (p != NULL) {
        memcpy(m_Id, p, SIZE);
    } else {
        memset(m_Id, 0, SIZE);
    }
}

// 0x00350AAC | nintendogs:callgraph [tier A]
nn::cec::CTR::MessageId::MessageId(u8* p)
{
    if (p != NULL) {
        memcpy(m_Id, p, SIZE);
    } else {
        memset(m_Id, 0, SIZE);
    }
}

// 0x00350AD4 | nintendogs:callgraph [tier A]
nn::cec::CTR::MessageId::MessageId()
{
    memset(m_Id, 0, SIZE);
}

// 0x007295B0 | nintendogs:bytes [tier A]
bool nn::cec::CTR::MessageId::IsEmpty() const
{
    for (int i = 0; i < static_cast<int>(SIZE); i++) {
        if (m_Id[i] != 0) {
            return false;
        }
    }
    return true;
}

// 0x007295EC | nintendogs:callgraph [tier A]
bool nn::cec::CTR::MessageId::IsEqual(const u8* p) const
{
    if (p == NULL) {
        return false;
    }
    return memcmp(p, m_Id, SIZE) == 0;
}

// 0x00729618 | nintendogs:callgraph [tier A]
void nn::cec::CTR::MessageId::GetBinary(u8* p) const
{
    memcpy(p, m_Id, SIZE);
}

} // namespace CTR
} // namespace cec
} // namespace nn
