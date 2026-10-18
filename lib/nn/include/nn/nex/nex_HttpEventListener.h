#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class HttpConnection;

// Receives the events of an HTTP request of a JobHttp (a second base of the jobs and listeners
// that use one). The names of the functions other than PostChunkedBuffer are ours.
// RTTI N2nn3nex17HttpEventListenerE @ 0x008CE6B0
class HttpEventListener
{
public:
    // (inline)
    HttpEventListener() {}
    virtual ~HttpEventListener() {}
    // the request is about to start: the header and the post data
    virtual void PrepareRequest(nn::nex::HttpConnection* pConnection) = 0; // slot 0x08
    // the next part of a chunked post (its size); the last one sets *pIsLast
    virtual unsigned int PostChunkedBuffer(nn::nex::HttpConnection* pConnection, unsigned char* pBuffer, unsigned int size, bool* pIsLast); // 0x00386D58 slot 0x0C
    virtual void ProcessResponseHeader(nn::nex::HttpConnection* pConnection, int statusCode) = 0; // slot 0x10
    // a part of the body; false: it is not taken yet (the job waits)
    virtual bool ProcessResponse(const unsigned char* pData, unsigned int size) = 0; // slot 0x14
};
} // namespace nex
} // namespace nn
