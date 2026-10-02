#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex23DataStoreProtocolClientE @ 0x008CECDC
// vtable 0x008FE4D0 (vptr 0x008FE4D8), offset_to_top 0, 23 entries
class DataStoreProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    DataStoreProtocolClient(); // ctor candidate(s) 0x003AF880 (unverified)
    virtual ~DataStoreProtocolClient(); // 0x003AF978 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003AF968 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003A79F8 slot 0x50 | slot vf_0x50 of nn::nex::ClientProtocol
    virtual void CreateResponder() const; // 0x0072D0E4 slot 0x54 | fefates:callseq
    void ProtoReturn_GetSpecificMeta(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003A88D4 | fefates:bytes [tier B]
    void ProtoReturn_PrepareGetObject(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003A9F10 | fefates:bytes [tier B]
    void ProtoReturn_GetSpecificMetaV1(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003A9F8C | fefates:bytes [tier B]
    void ProtoReturn_PreparePostObject(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003AA830 | fefates:bytes [tier B]
    void ProtoReturn_GetNotificationUrl(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003AAE98 | fefates:bytes [tier B]
    void ProtoReturn_GetPersistenceInfo(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003AB0D0 | fefates:bytes [tier B]
    void ProtoReturn_PrepareGetObjectV1(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003AB14C | fefates:bytes [tier B]
    void ProtoReturn_GetNewArrivedNotifications(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003ADF8C | fefates:bytes [tier B]
    void ProtoReturn_GetNewArrivedNotificationsV1(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003AED98 | fefates:bytes [tier B]
    void ProtoReturn_PrepareGetObjectOrMetaBinary(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003AF5B8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
