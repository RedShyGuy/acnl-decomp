#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
class KerberosEncryption;

// A Kerberos ticket: its (encrypted) data, the principal and the session key. The member names
// are ours.
// RTTI N2nn3nex6TicketE @ 0x008CF5E8
// vtable 0x008FFA88 (vptr 0x008FFA90), offset_to_top 0, 2 entries
class Ticket : public ::nn::nex::RefCountedObject
{
public:
    // the encrypted ticket
    Ticket(const nn::nex::Buffer& data, unsigned int unknown0x30); // 0x003D1764 | fefates:bytes [tier B]
    // the decrypted ticket
    Ticket(const nn::nex::Buffer& data, unsigned int principalId, unsigned int unknown0x30, const nn::nex::Key& sessionKey, nn::nex::DateTime time); // 0x003D17B0 | fefates:bytes [tier B]
    virtual ~Ticket(); // 0x003D1838 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003D180C slot 0x04 | fefates:bytes (deleting dtor)
    // the size of the session key (bytes)
    static const u32 SESSION_KEY_SIZE = 32;

    // the data is decrypted with the key of the encryption and key (the time, the principal and
    // the session key)
    void Decrypt(nn::nex::KerberosEncryption* pEncryption, const nn::nex::Key& key); // 0x003D1518 | fefates:bytes [tier B]

    bool m_IsDecrypted;   // 0x09 (in the tail padding of RefCountedObject)
    Buffer m_Data;        // 0x0C
    u32 m_PrincipalId;    // 0x2C
    u32 m_Unknown0x30;    // 0x30
    Key m_SessionKey;     // 0x34
    DateTime m_Time;      // 0x50
};
ASSERT_OFFSET(Ticket, m_IsDecrypted, 0x09);
ASSERT_OFFSET(Ticket, m_PrincipalId, 0x2C);
ASSERT_OFFSET(Ticket, m_SessionKey, 0x34);
ASSERT_SIZE(Ticket, 0x58);
} // namespace nex
} // namespace nn
