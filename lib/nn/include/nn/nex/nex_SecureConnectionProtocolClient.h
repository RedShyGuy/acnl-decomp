#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
class ConnectionData;
class Data;
class ProtocolCallContext;
class StationURL;
class String;
class qBuffer;

// The client of the secure connection protocol (the secure server): the registration of the
// station URLs, the connection data of stations, the replacement of URLs and the reports. The
// method names are from the DDL strings in the binary; the names of the functions without a
// reference name are ours.
// RTTI N2nn3nex30SecureConnectionProtocolClientE @ 0x008CF1DC
// vtable 0x008FF464 (vptr 0x008FF46C), offset_to_top 0, 23 entries
class SecureConnectionProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    enum Method
    {
        METHOD_REGISTER = 1,
        METHOD_REQUEST_CONNECTION_DATA = 2,
        METHOD_REQUEST_URLS = 3,
        METHOD_REGISTER_EX = 4,
        METHOD_TEST_CONNECTIVITY = 5,
        METHOD_UPDATE_URLS = 6,
        METHOD_REPLACE_URL = 7,
        METHOD_SEND_REPORT = 8,
        METHOD_COUNT = 8
    };
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);

    // id: the protocol id (0: the responder, not registered)
    explicit SecureConnectionProtocolClient(unsigned short id); // 0x003C42BC
    virtual ~SecureConnectionProtocolClient(); // 0x003C4338 slot 0x00
    // 0x003C4328 (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C3C20 slot 0x50 | fefates:bytes
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072DEB0 slot 0x54

    bool CallRegister(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::qList<nn::nex::StationURL>& urls, unsigned int* pConnectionId,
                      nn::nex::StationURL* pPublicURL); // 0x003C3188
    bool CallRequestConnectionData(nn::nex::ProtocolCallContext* pContext, bool* pIsSuccess, const unsigned int& connectionId, const unsigned int& principalId,
                                   nn::nex::qList<nn::nex::ConnectionData>* pConnectionData); // 0x003C3A88
    bool CallRegisterEx(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::qList<nn::nex::StationURL>& urls, unsigned int* pConnectionId,
                        nn::nex::StationURL* pPublicURL, const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& data); // 0x003C3318
    bool CallReplaceURL(nn::nex::ProtocolCallContext* pContext, const nn::nex::StationURL& target, const nn::nex::StationURL& url); // 0x003C34CC
    bool CallSendReport(nn::nex::ProtocolCallContext* pContext, const unsigned int& reportId, const nn::nex::qBuffer& data); // 0x003C3638

    static void ProtoReturn_Register(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C37A8
    static void ProtoReturn_RegisterEx(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C3914
    static void ProtoReturn_ReplaceURL(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C3A80
    static void ProtoReturn_SendReport(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C3A84
    static void ProtoReturn_RequestConnectionData(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003C3CAC | fefates:bytes [tier B]

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
} // namespace nex
} // namespace nn
