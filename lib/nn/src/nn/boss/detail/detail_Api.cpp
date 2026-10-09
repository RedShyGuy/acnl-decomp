// The internal functions of the boss library: the sessions, sending the task configuration as
// properties, the checks of the configuration and the results of the service.
#include "nn/boss/detail/detail_Api.h"
#include <string.h>
#include "nn/boss/boss_TaskPolicy.h"
#include "nn/boss/detail/boss_IpcManager.h"

namespace nn {
namespace boss {
namespace detail {
namespace {
// the id prefix of the tasks that only run while the program is in the foreground
const char FG_ONLY_TASK_ID[] = "FGONLYT";

// codes of ChangeBossRetCodeToResult used here (the names are ours)
const nn::boss::ResultCode CODE_INVALID_OPTION = static_cast<nn::boss::ResultCode>(3);
const nn::boss::ResultCode CODE_INVALID_ACTION_DATA = static_cast<nn::boss::ResultCode>(30);
const nn::boss::ResultCode CODE_INVALID_URL = static_cast<nn::boss::ResultCode>(29);
const nn::boss::ResultCode CODE_INVALID_PRIORITY = static_cast<nn::boss::ResultCode>(31);
const nn::boss::ResultCode CODE_INVALID_TARGET_DURATION = static_cast<nn::boss::ResultCode>(32);
const nn::boss::ResultCode CODE_INVALID_ACTION_CODE = static_cast<nn::boss::ResultCode>(33);
const nn::boss::ResultCode CODE_INVALID_SCHEDULING_POLICY = static_cast<nn::boss::ResultCode>(38);
const nn::boss::ResultCode CODE_INVALID_AP_INFO_TYPE = static_cast<nn::boss::ResultCode>(39);
const nn::boss::ResultCode CODE_INVALID_PERMISSION = static_cast<nn::boss::ResultCode>(40);
const nn::boss::ResultCode CODE_NOT_INITIALIZED = static_cast<nn::boss::ResultCode>(43);
const nn::boss::ResultCode CODE_SIZE_MISMATCH = static_cast<nn::boss::ResultCode>(44);
const nn::boss::ResultCode CODE_INVALID_CFG_INFO_TYPE = static_cast<nn::boss::ResultCode>(193);
// the result of an unknown code
const bit32 RESULT_UNKNOWN = 0xC960F84D;

const u32 TARGET_DURATION_MAX = 0x10000;
const u8 ACTION_CODE_MAX = 14;
const u8 AP_INFO_TYPE_MAX = 7;
const u8 CFG_INFO_TYPE_MAX = 7;
const u8 PERMISSION_MAX = 3;

// keeps the first failure of a property
inline void KeepFailure(nn::Result result);
} // namespace

// the first failure of SendPropertyUserInternal / ReceivePropertyUserInternal since the last
// Send* / Receive* (the initial value has all description bits set)
// 0x0097E81C
nn::Result s_PropertyResult(0xE7E3FFFF);

namespace {
inline void KeepFailure(nn::Result result)
{
    if (result.IsFailure() && s_PropertyResult.IsSuccess()) {
        s_PropertyResult = result;
    }
}
} // namespace

// 0x0046DA44 | nintendogs:bytes [tier A]
bool CheckTaskIdOk(const char* pTaskId)
{
    return pTaskId != 0 && *pTaskId != 0;
}

// 0x0046DA5C | nintendogs:bytes [tier A]
bool IsFgOnlyTaskId(const char* pTaskId)
{
    bool isFgOnly = false;
    if (strncmp(pTaskId, FG_ONLY_TASK_ID, strlen(FG_ONLY_TASK_ID)) == 0) {
        isFgOnly = true;
    }
    return isFgOnly;
}

// 0x0046DA94 | nintendogs:callgraph [tier A]
nn::Result FinalizeUserIpc()
{
    return s_IpcManager.FinalizeUserIpc();
}

// 0x0046DAA0 | nintendogs:callgraph [tier A]
nn::Result InitilizeUserIpc()
{
    return s_IpcManager.InitializeUserIpc();
}

// 0x0046DAAC | nintendogs:bytes [tier A]
nn::Result GetUserIpcInstance(nn::boss::detail::User*& pUser)
{
    pUser = s_IpcManager.m_IsUserInitialized ? &s_IpcManager.m_User : 0;
    if (pUser == 0) {
        return ChangeBossRetCodeToResult(CODE_NOT_INITIALIZED);
    }
    return nn::Result();
}

// 0x0046DAE0 | nintendogs:bytes-fuzzy [tier A]
nn::Result SendUserTaskAction(nn::boss::TaskActionConfig* pConfig)
{
    s_PropertyResult = nn::Result();
    SendPropertyUserInternal(PROPERTY_ACTION_CODE, &pConfig->actionCode, sizeof(pConfig->actionCode));
    SendPropertyUserInternal(PROPERTY_URL, reinterpret_cast<unsigned char*>(pConfig->url), sizeof(pConfig->url));
    SendPropertyUserInternal(PROPERTY_ACTION_DATA_TYPE, &pConfig->actionDataType, sizeof(pConfig->actionDataType));
    switch (pConfig->actionDataType) {
    case 0:
        SendPropertyUserInternal(PROPERTY_ACTION_DATA_0A, pConfig->actionData, 0x100);
        break;
    case 1:
        SendPropertyUserInternal(PROPERTY_ACTION_DATA_0B, pConfig->actionData, 0x200);
        break;
    case 2:
    case 3:
    case 4:
        SendPropertyUserInternal(PROPERTY_FILE_HANDLE, pConfig->fileHandle);
        break;
    case 5:
        SendPropertyUserInternal(PROPERTY_DATA_STORE_DOWNLOAD_ACTION_DATA, pConfig->actionData, 0x200);
        break;
    case 6:
        SendPropertyUserInternal(PROPERTY_FILE_HANDLE, pConfig->fileHandle);
        SendPropertyUserInternal(PROPERTY_DATA_STORE_DOWNLOAD_ACTION_DATA, pConfig->actionData, 0x200);
        break;
    default:
        return nn::Result(RESULT_UNKNOWN);
    }
    SendPropertyUserInternal(PROPERTY_HEADER_FIELDS, pConfig->headerFields, sizeof(pConfig->headerFields));
    SendPropertyUserInternal(PROPERTY_16, reinterpret_cast<unsigned char*>(&pConfig->property16), sizeof(pConfig->property16));
    SendPropertyUserInternal(PROPERTY_08, reinterpret_cast<unsigned char*>(&pConfig->property08), sizeof(pConfig->property08));
    SendPropertyUserInternal(PROPERTY_CLIENT_CERTS, reinterpret_cast<unsigned char*>(pConfig->clientCerts), sizeof(pConfig->clientCerts));
    SendPropertyUserInternal(PROPERTY_ROOT_CAS, reinterpret_cast<unsigned char*>(pConfig->rootCas), sizeof(pConfig->rootCas));
    SendPropertyUserInternal(PROPERTY_3B, reinterpret_cast<unsigned char*>(&pConfig->property3B), sizeof(pConfig->property3B));
    SendPropertyUserInternal(PROPERTY_AP_INFO_TYPE, &pConfig->apInfoType, sizeof(pConfig->apInfoType));
    SendPropertyUserInternal(PROPERTY_CFG_INFO_TYPE, &pConfig->cfgInfoType, sizeof(pConfig->cfgInfoType));
    SendPropertyUserInternal(PROPERTY_FS_CLIENT_CERT, &pConfig->fsClientCert, sizeof(pConfig->fsClientCert));
    SendPropertyUserInternal(PROPERTY_FS_ROOT_CA, &pConfig->fsRootCa, sizeof(pConfig->fsRootCa));
    SendPropertyUserInternal(PROPERTY_ROOT_CA_COUNT, reinterpret_cast<unsigned char*>(&pConfig->rootCaCount), sizeof(pConfig->rootCaCount));
    SendPropertyUserInternal(PROPERTY_CLIENT_CERT_COUNT, reinterpret_cast<unsigned char*>(&pConfig->clientCertCount), sizeof(pConfig->clientCertCount));
    SendPropertyUserInternal(PROPERTY_15, pConfig->property15, sizeof(pConfig->property15));
    return s_PropertyResult;
}

// 0x0046DCB4 | nintendogs:bytes [tier A]
nn::Result SendUserTaskOption(nn::boss::TaskOptionConfig* pConfig)
{
    s_PropertyResult = nn::Result();
    SendPropertyUserInternal(PROPERTY_18, &pConfig->property18, sizeof(pConfig->property18));
    SendPropertyUserInternal(PROPERTY_19, &pConfig->property19, sizeof(pConfig->property19));
    SendPropertyUserInternal(PROPERTY_1A, &pConfig->property1A, sizeof(pConfig->property1A));
    SendPropertyUserInternal(PROPERTY_1B, reinterpret_cast<unsigned char*>(&pConfig->property1B), sizeof(pConfig->property1B));
    SendPropertyUserInternal(PROPERTY_1C, reinterpret_cast<unsigned char*>(&pConfig->property1C), sizeof(pConfig->property1C));
    return s_PropertyResult;
}

// 0x0046DD24 | nintendogs:bytes [tier A]
nn::Result SendUserTaskPolicy(nn::boss::TaskPolicyConfig* pConfig)
{
    s_PropertyResult = nn::Result();
    SendPropertyUserInternal(PROPERTY_PRIORITY, &pConfig->priority, sizeof(pConfig->priority));
    SendPropertyUserInternal(PROPERTY_SCHEDULING_POLICY, &pConfig->schedulingPolicy, sizeof(pConfig->schedulingPolicy));
    SendPropertyUserInternal(PROPERTY_TARGET_DURATION, reinterpret_cast<unsigned char*>(&pConfig->targetDuration), sizeof(pConfig->targetDuration));
    SendPropertyUserInternal(PROPERTY_INTERVAL, reinterpret_cast<unsigned char*>(&pConfig->interval), sizeof(pConfig->interval));
    SendPropertyUserInternal(PROPERTY_COUNT, reinterpret_cast<unsigned char*>(&pConfig->count), sizeof(pConfig->count));
    SendPropertyUserInternal(PROPERTY_PERMISSION, &pConfig->permission, sizeof(pConfig->permission));
    return s_PropertyResult;
}

// 0x0046DDA0 | fefates:bytes-fuzzy [tier B]
nn::Result ReceiveUserTaskStatus(nn::boss::TaskStatusInfo* pInfo)
{
    s_PropertyResult = nn::Result();
    ReceivePropertyUserInternal(PROPERTY_TASK_STATE_CODE, &pInfo->stateCode, sizeof(pInfo->stateCode));
    ReceivePropertyUserInternal(PROPERTY_1E, &pInfo->property1E, sizeof(pInfo->property1E));
    ReceivePropertyUserInternal(PROPERTY_1F, &pInfo->property1F, sizeof(pInfo->property1F));
    ReceivePropertyUserInternal(PROPERTY_TASK_RESULT_CODE, &pInfo->resultCode, sizeof(pInfo->resultCode));
    ReceivePropertyUserInternal(PROPERTY_TASK_SERVICE_STATUS, &pInfo->serviceStatus, sizeof(pInfo->serviceStatus));
    ReceivePropertyUserInternal(PROPERTY_COMM_ERROR_CODE, reinterpret_cast<unsigned char*>(&pInfo->commErrorCode), sizeof(pInfo->commErrorCode));
    ReceivePropertyUserInternal(PROPERTY_25, reinterpret_cast<unsigned char*>(&pInfo->property25), sizeof(pInfo->property25));
    ReceivePropertyUserInternal(PROPERTY_26, reinterpret_cast<unsigned char*>(&pInfo->property26), sizeof(pInfo->property26));
    ReceivePropertyUserInternal(PROPERTY_27, reinterpret_cast<unsigned char*>(&pInfo->property27), sizeof(pInfo->property27));
    ReceivePropertyUserInternal(PROPERTY_LAST_SUCCESSFUL_TIMESTAMP, reinterpret_cast<unsigned char*>(&pInfo->lastSuccessfulTimestamp), sizeof(pInfo->lastSuccessfulTimestamp));
    ReceivePropertyUserInternal(PROPERTY_29, reinterpret_cast<unsigned char*>(&pInfo->property29), sizeof(pInfo->property29));
    ReceivePropertyUserInternal(PROPERTY_2A, reinterpret_cast<unsigned char*>(&pInfo->property2A), sizeof(pInfo->property2A));
    ReceivePropertyUserInternal(PROPERTY_2B, reinterpret_cast<unsigned char*>(&pInfo->property2B), sizeof(pInfo->property2B));
    ReceivePropertyUserInternal(PROPERTY_2C, &pInfo->property2C, sizeof(pInfo->property2C));
    ReceivePropertyUserInternal(PROPERTY_2D, reinterpret_cast<unsigned char*>(&pInfo->property2D), sizeof(pInfo->property2D));
    ReceivePropertyUserInternal(PROPERTY_2E, reinterpret_cast<unsigned char*>(&pInfo->property2E), sizeof(pInfo->property2E));
    ReceivePropertyUserInternal(PROPERTY_22, &pInfo->property22, sizeof(pInfo->property22));
    ReceivePropertyUserInternal(PROPERTY_24, &pInfo->property24, sizeof(pInfo->property24));
    ReceivePropertyUserInternal(PROPERTY_LAST_MODIFIED_HEADER, reinterpret_cast<unsigned char*>(pInfo->lastModified), sizeof(pInfo->lastModified));
    return s_PropertyResult;
}

// 0x0046DEF0 | nintendogs:callgraph [tier A]
nn::Result VerifyTaskActionConfig(nn::boss::TaskActionConfig* pConfig)
{
    u8 code = pConfig->actionCode;
    if (code == 0 || code >= ACTION_CODE_MAX) {
        return ChangeBossRetCodeToResult(CODE_INVALID_ACTION_CODE);
    }
    if (code == 1 || code == 2 || code == 3 || code == 6 || code == 7 || code == 8 || code == 9) {
        if (pConfig->url[0] == 0) {
            return ChangeBossRetCodeToResult(CODE_INVALID_URL);
        }
        if (pConfig->apInfoType > AP_INFO_TYPE_MAX) {
            return ChangeBossRetCodeToResult(CODE_INVALID_AP_INFO_TYPE);
        }
        if (pConfig->cfgInfoType > CFG_INFO_TYPE_MAX) {
            return ChangeBossRetCodeToResult(CODE_INVALID_CFG_INFO_TYPE);
        }
        if (code == 1 || code == 3) {
            switch (pConfig->actionDataType) {
            case 0:
                if (pConfig->actionData[0] == 0) {
                    return ChangeBossRetCodeToResult(CODE_INVALID_ACTION_DATA);
                }
                break;
            case 1:
                if (*reinterpret_cast<u16*>(pConfig->actionData) == 0) {
                    return ChangeBossRetCodeToResult(CODE_INVALID_ACTION_DATA);
                }
                break;
            case 2:
                break;
            default:
                if (code != 3 || (pConfig->actionDataType != 3 && pConfig->actionDataType != 4)) {
                    return ChangeBossRetCodeToResult(CODE_INVALID_ACTION_DATA);
                }
                break;
            }
        }
    }
    return nn::Result();
}

// 0x0046DFE4 | nintendogs:bytes [tier A]
nn::Result VerifyTaskOptionConfig(nn::boss::TaskOption* pOption)
{
    u8 value = pOption->m_Config.property18;
    if (value == 0 || value >= 2) {
        return ChangeBossRetCodeToResult(CODE_INVALID_OPTION);
    }
    return nn::Result();
}

// 0x0046E00C | nintendogs:bytes [tier A]
nn::Result VerifyTaskPolicyConfig(nn::boss::TaskPolicy* pPolicy)
{
    nn::boss::TaskPolicyConfig* pConfig = &pPolicy->m_Config;
    switch (pConfig->priority) {
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 35:
    case 80:
    case 125:
    case 170:
    case 215:
    case 221:
    case 222:
    case 223:
    case 224:
    case 225:
    case 226:
    case 227:
    case 228:
    case 255:
        break;
    default:
        return ChangeBossRetCodeToResult(CODE_INVALID_PRIORITY);
    }
    if (pConfig->permission > PERMISSION_MAX) {
        return ChangeBossRetCodeToResult(CODE_INVALID_PERMISSION);
    }
    if (pConfig->schedulingPolicy == 0 || pConfig->schedulingPolicy >= 2) {
        return ChangeBossRetCodeToResult(CODE_INVALID_SCHEDULING_POLICY);
    }
    if (pConfig->targetDuration >= TARGET_DURATION_MAX) {
        return ChangeBossRetCodeToResult(CODE_INVALID_TARGET_DURATION);
    }
    return nn::Result();
}

// 0x0046E108 | nintendogs:bytes [tier A]
nn::Result GetPrivilegedIpcInstance(nn::boss::detail::Privileged*& pPrivileged)
{
    pPrivileged = s_IpcManager.m_IsPrivilegedInitialized ? &s_IpcManager.m_Privileged : 0;
    if (pPrivileged == 0) {
        return ChangeBossRetCodeToResult(CODE_NOT_INITIALIZED);
    }
    return nn::Result();
}

// 0x0046E13C | fefates:bytes [tier B]
nn::Result SendPropertyUserInternal(nn::boss::PropertyType type, nn::Handle handle)
{
    nn::Result result;
    Privileged* pPrivileged;
    result = GetPrivilegedIpcInstance(pPrivileged);
    if (result.IsSuccess()) {
        result = pPrivileged->SendPropertyHandle(type, handle);
    } else {
        User* pUser;
        result = GetUserIpcInstance(pUser);
        if (result.IsSuccess()) {
            result = pUser->SendPropertyHandle(type, handle);
        }
    }
    KeepFailure(result);
    return result;
}

// 0x0046E1E8 | fefates:bytes [tier B]
nn::Result SendPropertyUserInternal(nn::boss::PropertyType type, unsigned char* pValue, unsigned int size)
{
    nn::Result result;
    Privileged* pPrivileged;
    result = GetPrivilegedIpcInstance(pPrivileged);
    if (result.IsSuccess()) {
        result = pPrivileged->SendProperty(type, pValue, size);
    } else {
        User* pUser;
        result = GetUserIpcInstance(pUser);
        if (result.IsSuccess()) {
            result = pUser->SendProperty(type, pValue, size);
        }
    }
    KeepFailure(result);
    return result;
}

// 0x0046E2A0 | nintendogs:bytes [tier A]
nn::Result ChangeBossRetCodeToResult(nn::boss::ResultCode code)
{
    switch (code) {
    case RESULT_CODE_SUCCESS:
        return nn::Result();
    case 1:
        return nn::Result(0xD8E0F801);
    case 2:
        return nn::Result(0xD8E0F802);
    case 3:
        return nn::Result(0xD8E0F803);
    case 4:
        return nn::Result(0xD8E0F804);
    case 5:
        return nn::Result(0xD8E0F805);
    case 6:
        return nn::Result(0xD8E0F806);
    case 7:
        return nn::Result(0xD8E0F807);
    case 8:
        return nn::Result(0xC8A0F808);
    case 9:
        return nn::Result(0xD8E0F809);
    case 10:
        return nn::Result(0xD8E0F80A);
    case 11:
        return nn::Result(0xD8E0F80B);
    case 12:
        return nn::Result(0xD8E0F80C);
    case 13:
        return nn::Result(0xC8A0F80D);
    case 14:
        return nn::Result(0xD8E0F80E);
    case 15:
        return nn::Result(0xD8E0F80F);
    case 16:
        return nn::Result(0xD8E0F810);
    case 17:
        return nn::Result(0xD8E0F811);
    case 18:
        return nn::Result(0xD8E0F812);
    case 19:
        return nn::Result(0xD8E0F813);
    case 20:
        return nn::Result(0xD8E0F814);
    case 21:
        return nn::Result(0xD8E0F815);
    case 26:
        return nn::Result(0xD8E0F81A);
    case 27:
        return nn::Result(0xD8E0F81B);
    case 28:
        return nn::Result(0xD8E0F81C);
    case 29:
        return nn::Result(0xD8E0F81D);
    case 30:
        return nn::Result(0xD8E0F81E);
    case 31:
        return nn::Result(0xD8E0F81F);
    case 32:
        return nn::Result(0xD8E0F820);
    case 33:
        return nn::Result(0xD8E0F821);
    case 34:
        return nn::Result(0xD8E0F822);
    case 35:
        return nn::Result(0xD860F823);
    case 36:
        return nn::Result(0xD860F824);
    case 37:
        return nn::Result(0xD860F825);
    case 38:
        return nn::Result(0xD8E0F826);
    case 39:
        return nn::Result(0xD8E0F827);
    case 40:
        return nn::Result(0xD8E0F828);
    case 41:
        return nn::Result(0xD840F829);
    case 42:
        return nn::Result(0xD840F82A);
    case 43:
        return nn::Result(0xC8A0F82B);
    case 44:
        return nn::Result(0xD860F82C);
    case 45:
        return nn::Result(0xD860F82D);
    case 46:
        return nn::Result(0xC8A0F82E);
    case 47:
        return nn::Result(0xD860F82F);
    case 48:
        return nn::Result(0xD860F830);
    case 49:
        return nn::Result(0xD860F831);
    case 50:
        return nn::Result(0xC8A0F832);
    case 51:
        return nn::Result(0xC8A0F833);
    case 52:
        return nn::Result(0xC8A0F834);
    case 53:
        return nn::Result(0xC8A0F835);
    case 54:
        return nn::Result(0xC8A0F836);
    case 55:
        return nn::Result(0xC8A0F837);
    case 56:
        return nn::Result(0xC8A0F838);
    case 57:
        return nn::Result(0xC8A0F839);
    case 58:
        return nn::Result(0xC8A0F83A);
    case 59:
        return nn::Result(0xC8A0F83B);
    case 60:
        return nn::Result(0xC8A0F83C);
    case 61:
        return nn::Result(0xC8A0F83D);
    case 62:
        return nn::Result(0xC8A0F83E);
    case 63:
        return nn::Result(0xC8A0F83F);
    case 64:
        return nn::Result(0xC8A0F840);
    case 65:
        return nn::Result(0xC8A0F841);
    case 66:
        return nn::Result(0xC8A0F842);
    case 67:
        return nn::Result(0xC8A0F843);
    case 68:
        return nn::Result(0xC960F844);
    case 69:
        return nn::Result(0xD840F845);
    case 70:
        return nn::Result(0xD840F846);
    case 71:
        return nn::Result(0xC8A0F847);
    case 72:
        return nn::Result(0xC8A0F848);
    case 73:
        return nn::Result(0xC8A0F849);
    case 74:
        return nn::Result(0xC960F84A);
    case 75:
        return nn::Result(0xC960F84B);
    case 76:
        return nn::Result(0xC960F84C);
    case 192:
        return nn::Result(0xD8E0F8C0);
    case 193:
        return nn::Result(0xD8E0F8C1);
    case 194:
        return nn::Result(0xD860F8C2);
    case 195:
        return nn::Result(0xD860F8C3);
    case 196:
        return nn::Result(0xD860F8C4);
    case 197:
        return nn::Result(0xC8A0F8C5);
    case 198:
        return nn::Result(0xC8A0F8C6);
    case 1004:
        return nn::Result(0xD8E0FBEC);
    case 1014:
        return nn::Result(0xD8E0FBF6);
    case 1018:
        return nn::Result(0xC8A0FBFA);
    case 1020:
        return nn::Result(0xC8A0FBFC);
    case 1021:
        return nn::Result(0xD8E0FBFD);
    default:
        return nn::Result(RESULT_UNKNOWN);
    }
}

// the received size must be the asked one
// 0x0046E860 (name is ours)
nn::Result ReceivePropertyUserInternal(nn::boss::PropertyType type, unsigned char* pValue, unsigned int size)
{
    nn::Result result;
    unsigned receivedSize;
    Privileged* pPrivileged;
    result = GetPrivilegedIpcInstance(pPrivileged);
    if (result.IsSuccess()) {
        result = pPrivileged->ReceiveProperty(type, pValue, size, &receivedSize);
        if (result.IsSuccess() && receivedSize != size) {
            result = ChangeBossRetCodeToResult(CODE_SIZE_MISMATCH);
        }
    } else {
        User* pUser;
        result = GetUserIpcInstance(pUser);
        if (result.IsSuccess()) {
            result = pUser->ReceiveProperty(type, pValue, size, &receivedSize);
            if (result.IsSuccess() && receivedSize != size) {
                result = ChangeBossRetCodeToResult(CODE_SIZE_MISMATCH);
            }
        }
    }
    KeepFailure(result);
    return result;
}

// 0x0046F134 | nintendogs:bytes [tier A]
size_t strnlen(const char* pString, unsigned maxLength)
{
    size_t length = 0;
    while (pString[length] != 0 && maxLength > length) {
        length++;
    }
    return length;
}

// 0x0046F154 | fefates:bytes [tier B]
size_t wcsnlen(const wchar_t* pString, unsigned int maxLength)
{
    size_t length = 0;
    while (pString[length] != 0 && maxLength > length) {
        length++;
    }
    return length;
}

} // namespace detail
} // namespace boss
} // namespace nn
