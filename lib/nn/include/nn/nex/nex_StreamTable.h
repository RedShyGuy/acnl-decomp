#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
class ConnectionOrientedStream;

// The streams of a ConnectionManager. The member name is ours.
// RTTI N2nn3nex11StreamTableE @ 0x008CE08C
// vtable 0x008FC454 (vptr 0x008FC45C), offset_to_top 0, 2 entries
class StreamTable : public ::nn::nex::RootObject
{
public:
    StreamTable(); // 0x0035BAE0 | mk7dlp:bytes [tier A]
    virtual ~StreamTable(); // 0x0035BB98 slot 0x00 | fefates:bytes
    // 0x0035BB68 (deleting dtor)
    // the stream of the URL type (null if none)
    nn::nex::ConnectionOrientedStream* FindStream(wchar_t* pType); // 0x0035BA80 | mk7dlp:bytes-fuzzy [tier A]

    qList<ConnectionOrientedStream*> m_Streams; // 0x04
};
ASSERT_SIZE(StreamTable, 0x1C);
} // namespace nex
} // namespace nn
