#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_MD5.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Time.h"

#include <cstdio>

namespace nn {
namespace nex {
// 0x00371058 slot 0x00
// 0x00371040 (deleting dtor)
nn::nex::HttpConnection::~HttpConnection()
{
    // nothing to do
}

// 0x0072AFA0 slot 0x18
bool nn::nex::HttpConnection::IsChunked() const
{
    return m_IsChunked;
}

// 0x0037103C slot 0x50
void nn::nex::HttpConnection::SetUnknown0x10CE()
{
    // nothing to do
}

// 0x0072AFA8 | fefates:bytes [tier B]
const char* nn::nex::HttpConnection::GetBoundaryString() const
{
    if (m_Boundary[0] == '\0') {
        Time now = SystemClock::GetTime();
        Time time = now;
        Buffer timeBuffer(sizeof(Time));
        Buffer hashBuffer(MD5::DIGEST_SIZE);
        timeBuffer.AppendData(&time, sizeof(time), static_cast<unsigned>(-1));
        MD5 md5;
        md5.Update(timeBuffer.m_pData + timeBuffer.m_HeaderSize, timeBuffer.m_ContentSize);
        hashBuffer.AppendData(md5.GetDigest(), MD5::DIGEST_SIZE, static_cast<unsigned>(-1));
        char* pHash;
        {
            String hash = hashBuffer.ToString();
            hash.CreateCopy(&pHash);
        }
        snprintf(m_Boundary, BOUNDARY_SIZE, "--------BOUNDARY--------%s", pHash);
        m_Boundary[BOUNDARY_SIZE - 1] = '\0';
        String::ReleaseBuffer(pHash);
    }
    return m_Boundary;
}

} // namespace nex
} // namespace nn
