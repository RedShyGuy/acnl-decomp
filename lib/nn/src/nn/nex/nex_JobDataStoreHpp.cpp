#include "nn/nex/nex_JobDataStoreHpp.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_DataStorePostHttpEventListener.h"
#include "nn/nex/nex_HMACChecksum.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_Protocol.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_INVALID_RESPONSE = static_cast<int>(0x80050001);
const int RESULT_REQUEST_ERROR = static_cast<int>(0x8005000B);
const int RESULT_TOO_MANY_REQUESTS = static_cast<int>(0x80050019);
const int RESULT_HTTP_ERROR = static_cast<int>(0x8005001A);
const int RESULT_SERVICE_UNAVAILABLE = static_cast<int>(0x8005001B);
// the HTTP status codes
const int STATUS_TEMPORARY_REDIRECT = 307;
const int STATUS_BAD_REQUEST = 400;
const int STATUS_FORBIDDEN = 403;
const int STATUS_REQUEST_ENTITY_TOO_LARGE = 413;
const int STATUS_INTERNAL_SERVER_ERROR = 500;
const int STATUS_SERVICE_UNAVAILABLE = 503;
// the requests of a redirect and after a server error
const u32 MAX_REDIRECT_COUNT = 5;
const u32 MAX_SERVER_ERROR_COUNT = 1;
// the shortest response (the size and the outcome)
const u32 MIN_RESPONSE_SIZE = 5;
// the length of the HMAC keys (padded with '0')
const u32 HMAC_KEY_LENGTH = 16;
// the buffer of the post
const u32 POST_BUFFER_SIZE = 0x4000;

// (inline) the call context of the caller
inline CallContext* GetCallerContext(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id);
}

// (inline) the status code is not an error
inline bool IsSuccessStatus(int statusCode)
{
    return static_cast<u32>(statusCode - 1) < 299;
}

// (inline) the HMAC of the message with the key (padded with '0')
inline void ComputeSignature(const Buffer& message, const String& key, Buffer* pSignature)
{
    HMACChecksum hmac;
    String paddedKey(key);
    while (paddedKey.GetLength() < HMAC_KEY_LENGTH) {
        paddedKey += String(L"0");
    }
    hmac.SetKey(Key(paddedKey));
    hmac.ComputeChecksum(message, pSignature);
}
} // namespace

// 0x00378700 | fefates:bytes [tier B]
void nn::nex::JobDataStoreHpp::StepWaitingForHttp()
{
    int result;
    if (GetCallerContext(m_CallContextId) == nullptr) {
        result = RESULT_CANCELLED;
    } else {
        if (m_CallContext.m_State != CallContext::STATE_SUCCESS) {
            CompleteJob(m_CallContext.m_Result);
            return;
        }
        if (IsSuccessStatus(m_StatusCode)) {
            CompleteJob(ProcessFinalResponse(m_Response));
            return;
        }
        m_Response.push_back(0);
        result = RESULT_REQUEST_ERROR;
        if (!m_IsTooLarge) {
            switch (m_StatusCode) {
                case STATUS_REQUEST_ENTITY_TOO_LARGE:
                case STATUS_BAD_REQUEST:
                case STATUS_FORBIDDEN:
                    break;
                case STATUS_TEMPORARY_REDIRECT:
                    if (String::IsEqual(m_Location.m_pString, L"") || m_RequestCount >= MAX_REDIRECT_COUNT) {
                        CompleteJob(qResult(RESULT_TOO_MANY_REQUESTS));
                        return;
                    }
                    m_URL = m_Location;
                    SetStep(Step(&JobDataStoreHpp::StepHttp, ""));
                    return;
                case STATUS_INTERNAL_SERVER_ERROR:
                    if (m_RequestCount >= MAX_SERVER_ERROR_COUNT) {
                        CompleteJob(qResult(RESULT_TOO_MANY_REQUESTS));
                        return;
                    }
                    SetStep(Step(&JobDataStoreHpp::StepHttp, ""));
                    return;
                case STATUS_SERVICE_UNAVAILABLE:
                    result = RESULT_SERVICE_UNAVAILABLE;
                    break;
                default:
                    result = RESULT_HTTP_ERROR;
                    break;
            }
        }
    }
    CompleteJob(qResult(result));
}

// 0x00378918 | fefates:bytes [tier B]
nn::nex::qResult nn::nex::JobDataStoreHpp::ProcessFinalResponse(const nn::nex::qVector<unsigned char>& response)
{
    u32 size = response.size();
    if (size < MIN_RESPONSE_SIZE) {
        return qResult(RESULT_INVALID_RESPONSE);
    }
    Buffer buffer(size);
    buffer.AppendData(response.begin(), response.size(), 0xFFFFFFFF);
    Message message(&buffer);
    qResult result;
    Protocol::ExtractCallOutcome(&message, &result);
    ProtocolCallContext* pContext = static_cast<ProtocolCallContext*>(Protocol::RetrieveCallContext(&message));
    if (pContext == nullptr || pContext->m_Id != m_CallContextId) {
        // the ID of the call is read again (for a message)
        u32 position = message.m_Position - sizeof(u32);
        if (position <= message.m_pBuffer->m_ContentSize) {
            message.m_Position = position;
        }
        u32 callId;
        message.ExtractPrimitive(&callId);
        return qResult(RESULT_INVALID_RESPONSE);
    }
    if (result) {
        m_pProtocol->ExtractCallSpecificResults(&message, pContext);
    }
    return result;
}

// 0x00378A88 | fefates:bytes-fuzzy [tier B]
void nn::nex::JobDataStoreHpp::PrepareRequestHeaders()
{
    m_HeaderFields.clear();
    Buffer signature1(Buffer::GetDefaultBufferSize());
    ComputeSignature(m_Message, m_AccessKey, &signature1);
    Buffer signature2(Buffer::GetDefaultBufferSize());
    ComputeSignature(m_Message, m_Key, &signature2);
    String principalId;
    principalId.Format(L"%u", m_PrincipalId);
    m_HeaderFields.push_back(DataStoreKeyValue(String("pid"), principalId));
    m_HeaderFields.push_back(DataStoreKeyValue(String("version"), String("NEX_3_10_1")));
    m_HeaderFields.push_back(DataStoreKeyValue(String("token"), m_Token));
    m_HeaderFields.push_back(DataStoreKeyValue(String("signature1"), signature1.ToString()));
    m_HeaderFields.push_back(DataStoreKeyValue(String("signature2"), signature2.ToString()));
    if (m_PriorityType != 0) {
        StringStream stream;
        stream << static_cast<int>(m_PriorityType);
        m_HeaderFields.push_back(DataStoreKeyValue(String("Priority-Type"), String(stream.CStr())));
    }
}

// 0x003792DC | fefates:bytes [tier B]
void nn::nex::JobDataStoreHpp::StepRetrieveGameAuthToken()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_CallContext.Reset();
    if (!m_AuthTokenClient.RetrieveGameAuthToken(&m_CallContext, m_PrincipalId, m_Password, &m_Token, &m_Key)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_CallContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobDataStoreHpp::StepWaitingRetrieveGameAuthToken, ""));
}

// 0x00379400 | fefates:bytes [tier B]
void nn::nex::JobDataStoreHpp::StepWaitingRetrieveGameAuthToken()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_CallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(m_CallContext.m_Result);
        return;
    }
    SetStep(Step(&JobDataStoreHpp::StepHttp, ""));
}

// 0x003794BC | fefates:bytes [tier B]
void nn::nex::JobDataStoreHpp::StepHttp()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_StatusCode = 0;
    m_Response.clear();
    m_IsTooLarge = false;
    m_Location = L"";
    m_RequestCount++;
    m_CallContext.Reset();
    m_CallContext.m_Deadline = Time::ConvertTimeoutToDeadline(m_Timeout);
    PrepareRequestHeaders();
    if (m_pHttpListener != nullptr) {
        delete m_pHttpListener;
        m_pHttpListener = nullptr;
    }
    m_pHttpListener = new DataStorePostHttpEventListener(m_CallContextId, m_HeaderFields, m_FormFields, m_RootCa, m_ThreadPriority, POST_BUFFER_SIZE,
                                                         m_Message.m_pData + m_Message.m_HeaderSize, m_Message.m_ContentSize, nullptr);
    qResult result = m_HttpClient.Start(&m_CallContext, m_URL, static_cast<HttpConnection::Method>(m_Method), this, POST_BUFFER_SIZE);
    if (!result) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_CallContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobDataStoreHpp::StepWaitingForHttp, ""));
}

// 0x003CC5B0 | fefates:bytes [tier B]
void nn::nex::JobDataStoreHpp::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallerContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    } else {
        m_CallContext.Cancel(CallContext::STATE_CANCELLED);
    }
    SetToComplete();
}

} // namespace nex
} // namespace nn
