#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_DataStoreProtocolClient.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003AF880 (unverified)
nn::nex::DataStoreProtocolClient::DataStoreProtocolClient()
{
}

// 0x003AF978 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::DataStoreProtocolClient::~DataStoreProtocolClient()
{
}

// 0x003A79F8 slot 0x50 | slot vf_0x50 of nn::nex::ClientProtocol
void nn::nex::DataStoreProtocolClient::ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x0072D0E4 slot 0x54 | fefates:callseq
void nn::nex::DataStoreProtocolClient::CreateResponder() const
{
}

// 0x003A88D4 | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetSpecificMeta(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003A9F10 | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_PrepareGetObject(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003A9F8C | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetSpecificMetaV1(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003AA830 | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_PreparePostObject(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003AAE98 | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetNotificationUrl(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003AB0D0 | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetPersistenceInfo(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003AB14C | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_PrepareGetObjectV1(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003ADF8C | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetNewArrivedNotifications(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003AED98 | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetNewArrivedNotificationsV1(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x003AF5B8 | fefates:bytes [tier B]
void nn::nex::DataStoreProtocolClient::ProtoReturn_PrepareGetObjectOrMetaBinary(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

} // namespace nex
} // namespace nn
