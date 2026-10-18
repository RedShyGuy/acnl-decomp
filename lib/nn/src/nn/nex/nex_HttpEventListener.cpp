#include "nn/nex/nex_HttpEventListener.h"

namespace nn {
namespace nex {
// 0x00386D58
unsigned int nn::nex::HttpEventListener::PostChunkedBuffer(nn::nex::HttpConnection* pConnection, unsigned char* pBuffer, unsigned int size, bool* pIsLast)
{
    *pIsLast = true;
    return 0;
}

} // namespace nex
} // namespace nn
