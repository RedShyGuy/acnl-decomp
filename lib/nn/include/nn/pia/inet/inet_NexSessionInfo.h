#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_DateTime.h"
#include "nn/pia/session/session_ISessionInfo.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet14NexSessionInfoE @ 0x008CF878
// vtable 0x008FFF4C (vptr 0x008FFF54), offset_to_top 0, 39 entries
//
// The information about a nex matchmake session: values, six attributes, a UTF-16 string and
// application data. Their meaning is not known yet, so the getters and setters are named after
// their slots and the members after their offsets.
class NexSessionInfo : public ::nn::pia::session::ISessionInfo
{
public:
    static const u32 ATTRIBUTE_NUM = 6;
    static const u32 STRING_LENGTH_MAX = 256;
    static const u32 DATA_SIZE_MAX = 512;

    NexSessionInfo(); // 0x003E6FC4
    virtual ~NexSessionInfo(); // 0x003E70AC slot 0x00
    // 0x003E709C slot 0x04 (deleting dtor)
    virtual u32 vf_0x08() const; // 0x0072EFF4 slot 0x08
    virtual u32 vf_0x0C() const; // 0x0072F038 slot 0x0C
    virtual u32 vf_0x10() const; // 0x0072F0D0 slot 0x10
    virtual u32 vf_0x14() const; // 0x0072F0B0 slot 0x14
    virtual u32 vf_0x18() const; // 0x0072F0A8 slot 0x18
    virtual bool vf_0x1C() const; // 0x0072F0F0 slot 0x1C
    // all values back to their defaults (name is ours)
    virtual void Clear(); // 0x003E6F18 slot 0x20
    virtual void Trace(u64 flag) const; // 0x003E6FB8 slot 0x24
    // copies the application data (size bytes; INVALID_ARGUMENT if the buffer is too small)
    virtual nn::Result vf_0x28(void* pBuffer, unsigned int size) const; // 0x003E6DC4 slot 0x28
    virtual u32 vf_0x2C() const; // 0x0072F0C8 slot 0x2C
    virtual nn::Result vf_0x30(u32* pValue, unsigned int index) const; // 0x0072EFFC slot 0x30
    // copies the string (size bytes)
    virtual nn::Result vf_0x34(u16* pBuffer, unsigned int size) const; // 0x0072F040 slot 0x34
    virtual u32 vf_0x38() const; // 0x0072F098 slot 0x38
    virtual bool vf_0x3C() const; // 0x0072F0D8 slot 0x3C
    virtual bool vf_0x40() const; // 0x0072F0E4 slot 0x40
    virtual u8 vf_0x44() const; // 0x0072F088 slot 0x44
    virtual u32 vf_0x48() const; // 0x0072F0A0 slot 0x48
    virtual u32 vf_0x4C() const; // 0x0072F0B8 slot 0x4C
    virtual u8 vf_0x50() const; // 0x0072F0C0 slot 0x50
    virtual const common::DateTime* vf_0x54() const; // 0x0072F090 slot 0x54
    // all values of the other info (name is ours)
    virtual void Copy(const nn::pia::inet::NexSessionInfo& rhs); // 0x003E6E24 slot 0x58
    virtual void vf_0x5C(u32 value); // 0x003E6CAC slot 0x5C
    virtual void vf_0x60(u32 value); // 0x003E6CC4 slot 0x60
    virtual void vf_0x64(u32 value); // 0x003E6E0C slot 0x64
    virtual void vf_0x68(u32 value); // 0x003E6DAC slot 0x68
    virtual void vf_0x6C(u32 value); // 0x003E6DA4 slot 0x6C
    virtual void vf_0x70(bool value); // 0x003E6FBC slot 0x70
    virtual void vf_0x74(u32 value, unsigned int index); // 0x003E6CB4 slot 0x74
    virtual void vf_0x78(const u16* pString, unsigned int length); // 0x003E6CCC slot 0x78
    virtual void vf_0x7C(const void* pData, unsigned int size); // 0x003E6D60 slot 0x7C
    virtual void vf_0x80(bool value); // 0x003E6E14 slot 0x80
    virtual void vf_0x84(bool value); // 0x003E6E1C slot 0x84
    virtual void vf_0x88(u8 value); // 0x003E6D18 slot 0x88
    virtual void vf_0x8C(u32 value); // 0x003E6D9C slot 0x8C
    virtual void vf_0x90(u32 value); // 0x003E6DB4 slot 0x90
    virtual void vf_0x94(u8 value); // 0x003E6DBC slot 0x94
    virtual void vf_0x98(const nn::pia::common::DateTime& dateTime); // 0x003E6D20 slot 0x98

    u32 m_Unknown0x4;                          // 0x004
    u32 m_Unknown0x8;                          // 0x008
    u32 m_Unknown0xC;                          // 0x00C
    u32 m_Unknown0x10;                         // 0x010
    u32 m_Unknown0x14;                         // 0x014
    bool m_Unknown0x18;                        // 0x018
    u32 m_Attributes[ATTRIBUTE_NUM];           // 0x01C
    u16 m_String[STRING_LENGTH_MAX + 1];       // 0x034
    u32 m_StringLength;                        // 0x238
    u8 m_Data[DATA_SIZE_MAX];                  // 0x23C
    u32 m_DataSize;                            // 0x43C
    bool m_Unknown0x440;                       // 0x440
    bool m_Unknown0x441;                       // 0x441
    u8 m_Unknown0x442;                         // 0x442
    u32 m_Unknown0x444;                        // 0x444
    u32 m_Unknown0x448;                        // 0x448
    u8 m_Unknown0x44C;                         // 0x44C (255 by default)
    common::DateTime m_DateTime;               // 0x450
};
ASSERT_OFFSET(NexSessionInfo, m_StringLength, 0x238);
ASSERT_OFFSET(NexSessionInfo, m_DataSize, 0x43C);
ASSERT_OFFSET(NexSessionInfo, m_DateTime, 0x450);
ASSERT_SIZE(NexSessionInfo, 0x45C);
} // namespace inet
} // namespace pia
} // namespace nn
