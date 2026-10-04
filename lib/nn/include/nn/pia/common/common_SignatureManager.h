#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_Hmac.h"
#include "nn/pia/common/common_Md5Context.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_StationAddress.h"
#include "pead/peadPtrArray.h"
#include <new>

namespace nn {
namespace pia {
namespace common {
class SignatureSetting;

// The HMAC-MD5 signatures of the packets: one key for all stations and up to four stations with
// their own key (Specified). One instance (CreateInstance). Layout from CreateInstance; the
// member names and the names marked so are ours.
class SignatureManager : public RootObject
{
public:
    static const int SPECIFIED_NUM = 4;

    // the signer of one key: an HMAC for sending and one for checking, each with an MD5 context
    // made in place by Setup (name is ours; not a RootObject: its first Hmac is at 4 in the
    // manager, right after the manager's empty base)
    class SignatureContext
    {
    public:
        // checks the signature at the end of the data (true without a key)
        bool Check(const void* pData, unsigned int size); // 0x004275B0
        // signs the data, the signature goes behind it; the signature size
        unsigned int Append(void* pData, unsigned int bufferSize, unsigned int dataSize); // 0x00427638

        unsigned int GetSignatureSize() const
        {
            return m_SendHmac.m_pHashContext != nullptr ? m_SendHmac.m_pHashContext->GetHashSize() : 0;
        }
        void Reset()
        {
            m_SendHmac.Initialize(nullptr, nullptr, 0);
            m_ReceiveHmac.Initialize(nullptr, nullptr, 0);
        }
        void SetKey(const void* pKey, unsigned int keySize)
        {
            Md5Context* pSendMd5 = ::new (m_SendMd5) Md5Context();
            Md5Context* pReceiveMd5 = ::new (m_ReceiveMd5) Md5Context();
            m_SendHmac.Initialize(pSendMd5, pKey, keySize);
            m_ReceiveHmac.Initialize(pReceiveMd5, pKey, keySize);
        }

        Hmac m_SendHmac;                                // 0x000
        u32 m_SendMd5[sizeof(Md5Context) / 4];          // 0x084
        Hmac m_ReceiveHmac;                             // 0x0DC
        u32 m_ReceiveMd5[sizeof(Md5Context) / 4];       // 0x160
    };

    // a station with its own key (the class name and the constructor / destructor are from the
    // fefates symbols, the members are ours)
    class Specified : public RootObject
    {
    public:
        Specified(); // 0x00427E6C | fefates:bytes [tier B]
        ~Specified(); // 0x00427E90 | fefates:bytes [tier B]

        StationAddress m_StationAddress; // 0x04
        SignatureContext m_Context;      // 0x14
    };

    // the argument of Setup (session::Mesh builds it on the stack; the names are ours)
    struct StationSetting
    {
        StationSetting(); // 0x00427B34
        ~StationSetting(); // 0x00427B48

        StationAddress m_StationAddress;  // 0x00
        const SignatureSetting* m_pSetting; // 0x10
    };
    struct Setting
    {
        const SignatureSetting* m_pDefaultSetting;  // 0x00
        StationSetting m_StationSettings[SPECIFIED_NUM]; // 0x04
    };

    static SignatureManager* GetInstance() { return s_pInstance; }
    static nn::Result CreateInstance(); // 0x004277FC | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x004279F0 | fefates:bytes [tier B]

    // whether the packets must be signed (before Setup)
    void SetNecessity(bool isNecessary); // 0x00427680 | fefates:bytes [tier B]
    void ResetNecessity(); // 0x004278B8 | fefates:bytes [tier B]
    // the keys (name is ours)
    nn::Result Setup(const Setting& setting); // 0x00427B64
    void Cleanup(); // 0x00427AC0 | fefates:bytes [tier B]

    bool CheckSignature(const nn::pia::common::StationAddress& address, const void* pData, unsigned int size, unsigned int* pPayloadSize); // 0x004276A8 | fefates:bytes [tier B]
    unsigned int AppendSignature(const nn::pia::common::StationAddress& address, void* pData, unsigned int bufferSize, unsigned int dataSize); // 0x00427930 | fefates:bytes [tier B]
    unsigned int UpdateSignature(const nn::pia::common::StationAddress& address, void* pData, unsigned int size); // 0x00427A38 | fefates:bytes [tier B]

    // (inline in CreateInstance)
    SignatureManager() : m_State(STATE_NONE), m_IsNecessary(false) {}
    // (inline in DestroyInstance)
    ~SignatureManager() {}

    // m_State
    enum State : u8
    {
        STATE_NONE = 0,
        STATE_NECESSITY_SET = 1,
        STATE_SETUP = 2,
    };

    SignatureContext m_DefaultContext;   // 0x004
    pead::FixedPtrArray<Specified, SPECIFIED_NUM> m_SpecifiedArray; // 0x1BC, sorted by the station address
    Specified m_Specified[SPECIFIED_NUM];  // 0x1D8
    State m_State;                       // 0x908
    bool m_IsNecessary;                  // 0x909

    static SignatureManager* s_pInstance; // 0x0097E3D4
};
ASSERT_SIZE(SignatureManager::SignatureContext, 0x1B8);
ASSERT_SIZE(SignatureManager::Specified, 0x1CC);
ASSERT_SIZE(SignatureManager::StationSetting, 0x14);
ASSERT_SIZE(SignatureManager::Setting, 0x54);
ASSERT_OFFSET(SignatureManager, m_DefaultContext, 0x4);
ASSERT_OFFSET(SignatureManager, m_Specified, 0x1D8);
ASSERT_SIZE(SignatureManager, 0x90C);
} // namespace common
} // namespace pia
} // namespace nn
