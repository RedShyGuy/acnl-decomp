#pragma once

#include "decomp.h"
#include "nn/nex/nex_CompressionAlgorithm.h"
#include "nn/nex/nex_EncryptionAlgorithm.h"
#include "nn/nex/nex_KeyedChecksumAlgorithm.h"
#include "nn/nex/nex_LiteBuffer.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StreamBundling.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// The settings of a stream type (a PseudoGlobalVariable per type, see Stream::GetSettings). The
// layout is from the constructor and the copy of PseudoGlobalVariable<StreamSettings>; the names of
// the members without a setter and of the functions marked so are ours.
// RTTI N2nn3nex14StreamSettingsE @ 0x008CE3E8
// vtable 0x008FCD78 (vptr 0x008FCD80), offset_to_top 0, 2 entries
class StreamSettings : public ::nn::nex::RootObject
{
public:
    StreamSettings(); // 0x00374868 (name is ours)
    // (inline, with the copy constructor of StreamBundling)
    StreamSettings(const StreamSettings& other) : RootObject() { *this = other; }
    virtual ~StreamSettings(); // 0x003749DC slot 0x00 | fefates:bytes
    // 0x0037498C slot 0x04 (deleting dtor)

    // (inline)
    StreamSettings& operator=(const StreamSettings& other)
    {
        m_Unknown0x04 = other.m_Unknown0x04;
        m_KeyByteSum = other.m_KeyByteSum;
        m_KeySum = other.m_KeySum;
        m_pCompressionAlgorithm = other.m_pCompressionAlgorithm;
        m_pEncryptionAlgorithm = other.m_pEncryptionAlgorithm;
        m_Bundling = other.m_Bundling;
        m_Unknown0x60 = other.m_Unknown0x60;
        m_KeepAliveTimeout = other.m_KeepAliveTimeout;
        m_Unknown0x68 = other.m_Unknown0x68;
        m_MaxSilenceTime = other.m_MaxSilenceTime;
        m_Unknown0x70 = other.m_Unknown0x70;
        m_Unknown0x74 = other.m_Unknown0x74;
        m_Unknown0x78 = other.m_Unknown0x78;
        m_Unknown0x7C = other.m_Unknown0x7C;
        m_Unknown0x80 = other.m_Unknown0x80;
        m_Unknown0x84 = other.m_Unknown0x84;
        m_Unknown0x88 = other.m_Unknown0x88;
        m_Unknown0x8C = other.m_Unknown0x8C;
        m_Unknown0x90 = other.m_Unknown0x90;
        m_Unknown0x94 = other.m_Unknown0x94;
        m_Unknown0x95 = other.m_Unknown0x95;
        m_Unknown0x96 = other.m_Unknown0x96;
        m_UsesKey = other.m_UsesKey;
        m_Unknown0x98 = other.m_Unknown0x98;
        m_Unknown0x99 = other.m_Unknown0x99;
        m_pChecksumAlgorithm = other.m_pChecksumAlgorithm;
        m_Unknown0xA0 = other.m_Unknown0xA0;
        m_Unknown0xA4 = other.m_Unknown0xA4;
        return *this;
    }

    nn::nex::qResult SetMaxSilenceTime(unsigned int milliseconds); // 0x003745CC | fefates:callgraph [tier C]
    nn::nex::qResult SetKeepAliveTimeout(unsigned int milliseconds); // 0x003745F0 | fefates:callgraph [tier C]
    // the sums of the bytes of the key and the MD5 of it as the key of the checksum algorithm
    void SetKey(const nn::nex::String& key); // 0x00374614 | fefates:callgraph [tier C]

    u32 m_Unknown0x04;                            // 0x04
    u8 m_KeyByteSum;                              // 0x08
    s32 m_KeySum;                                 // 0x0C
    CompressionAlgorithm* m_pCompressionAlgorithm; // 0x10
    EncryptionAlgorithm* m_pEncryptionAlgorithm;  // 0x14
    StreamBundling m_Bundling;                    // 0x18
    u32 m_Unknown0x60;                            // 0x60
    u32 m_KeepAliveTimeout;                       // 0x64
    u8 m_Unknown0x68;                             // 0x68
    u32 m_MaxSilenceTime;                         // 0x6C
    u32 m_Unknown0x70;                            // 0x70
    u32 m_Unknown0x74;                            // 0x74
    f32 m_Unknown0x78;                            // 0x78
    f32 m_Unknown0x7C;                            // 0x7C
    u32 m_Unknown0x80;                            // 0x80
    s32 m_Unknown0x84;                            // 0x84
    u32 m_Unknown0x88;                            // 0x88
    u32 m_Unknown0x8C;                            // 0x8C
    u32 m_Unknown0x90;                            // 0x90
    u8 m_Unknown0x94;                             // 0x94
    u8 m_Unknown0x95;                             // 0x95
    u8 m_Unknown0x96;                             // 0x96
    bool m_UsesKey;                               // 0x97
    u8 m_Unknown0x98;                             // 0x98
    u8 m_Unknown0x99;                             // 0x99
    KeyedChecksumAlgorithm* m_pChecksumAlgorithm; // 0x9C
    u8 m_Unknown0xA0;                             // 0xA0
    LiteBuffer<32> m_Unknown0xA4;                 // 0xA4
};
ASSERT_OFFSET(StreamSettings, m_Unknown0x60, 0x60);
ASSERT_OFFSET(StreamSettings, m_pChecksumAlgorithm, 0x9C);
ASSERT_OFFSET(StreamSettings, m_Unknown0xA4, 0xA4);
ASSERT_SIZE(StreamSettings, 0xD0);
} // namespace nex
} // namespace nn
