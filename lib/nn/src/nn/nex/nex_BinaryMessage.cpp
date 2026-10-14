#include "nn/nex/nex_BinaryMessage.h"
#include "nn/nex/nex_Api.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
// 0x00361C40 slot 0x00
// 0x00361C00 (deleting dtor)
nn::nex::BinaryMessage::~BinaryMessage()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00361B44 slot 0x20
void nn::nex::BinaryMessage::Trace() const
{
    UserMessage::Trace();
    StringStream stream;
    noshowbase(stream);
    zerohex(stream);
    for (u32 i = 0; i < m_binaryBody.size(); i++) {
        if (i % TRACE_LINE_LENGTH == 0) {
            // (the trace of the line is removed, only the call of GetLength is left)
            stream.GetLength();
            stream.Clear();
            stream << L"    ";
        }
        stream.StreamNumber(m_binaryBody.m_pBegin[i]);
    }
    // (the trace of the last line is removed)
    stream.GetLength();
}

} // namespace nex
} // namespace nn
