#include "nn/nex/nex_TransportSignatureGenerator.h"
#include "nn/nex/nex_BitStream.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_HMACChecksum.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// 0x003BD454 | fefates:bytes [tier B]
nn::nex::TransportSignatureGenerator::TransportSignatureGenerator() : m_pChecksumAlgorithm(nullptr)
{
    HMACChecksum* pChecksum = new HMACChecksum();
    if (m_pChecksumAlgorithm != nullptr) {
        delete m_pChecksumAlgorithm;
    }
    m_pChecksumAlgorithm = pChecksum;
    // the key is the checksum of the current time
    Buffer timeData;
    BitStream stream(&timeData);
    Time now = SystemClock::GetTime();
    stream.AppendRaw(reinterpret_cast<const unsigned char*>(&now), sizeof(now) * 8);
    Buffer checksum;
    m_pChecksumAlgorithm->ComputeChecksum(timeData, &checksum);
    Key key(checksum.m_pData + checksum.m_HeaderSize, checksum.m_ContentSize);
    static_cast<KeyedChecksumAlgorithm*>(m_pChecksumAlgorithm)->SetKeyInline(key);
}

} // namespace nex
} // namespace nn
