#include "nn/nfp/CTR/CTR_Api.h"
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/nfc/CTR/nfc_NfcIpc.h"
#include "nn/nfc/CTR/nfc_NfcIpcMaster.h"
#include "nn/os/os_TransferMemoryBlock.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

#include <string.h>

// nfp_Api.cpp (static initializer __sti___11_nfp_Api_cpp at 0x00785830)

namespace nn {
namespace nfp {
namespace CTR {
namespace {
// Status, InvalidState, NFP, 512: the library is not initialized (or already initialized)
const bit32 RESULT_INVALID_STATE = 0xC8A18600;
// Usage, InvalidArgument, NFP, 81: null pointer
const bit32 RESULT_INVALID_POINTER = 0xE0E18451;
// Fatal, Internal, NFP, 392: a result of another module than NFC
const bit32 RESULT_UNKNOWN = 0xF9618588;

// the module numbers of 3dbrew "Error codes"
const bit32 MODULE_NFC = 93;
const bit32 MODULE_NFP = 97;

// the applet id of amiibo Settings (value from the binary)
const u32 AMIIBO_SETTINGS_APPLET_ID = 0x119;

// Parameter::mode of InitializeParameter / InitializeParameterForUpdate (names are ours)
const s32 MODE_DEFAULT = 0;
const s32 MODE_UPDATE = 3;
const s32 MODE_SPECIAL = 100;   // also accepted by StartAmiiboSettings

// Parameter::result
const s32 PARAMETER_RESULT_FAILED = -1;
const s32 PARAMETER_RESULT_NFC_IN_USE = -2;

// the globals of this file (names are ours)
// 0x00975F5C
nn::Handle s_Session;
// 0x00975F60
nn::nfc::CTR::NfcIpc s_NfcIpc((nn::Handle()));
// 0x00975F64
nn::nfc::CTR::NfcIpcMaster s_NfcIpcMaster((nn::Handle()));
// never used in this program
// 0x00AE1F44
nn::os::TransferMemoryBlock s_TransferMemory;

DECOMP_NOINLINE nn::Result ConvertResult(nn::Result result);
DECOMP_NOIPA nn::Result InitializeCommon(const char* serviceName);

bool IsInitialized()
{
    return s_Session.IsValid();
}

// both parameter functions clear the same fields (inline)
inline void ClearParameter(nn::nfp::CTR::Parameter* parameter, s32 mode)
{
    parameter->mode = mode;
    memset(parameter->unknown04, 0, sizeof(parameter->unknown04));
    parameter->unknown58 = 0;
    parameter->unknown59 = 0;
    parameter->unknown5A = 0;
    parameter->unknown5B = 0;
    memset(parameter->unknown5C, 0, sizeof(parameter->unknown5C));
    for (s32 i = 0; i < 24; i++) {
        parameter->unknown104[i] = 0;
    }
}
} // namespace

// 0x003DA6FC | tier C (confirmed by the code)
nn::Result Disconnect()
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    return ConvertResult(s_NfcIpcMaster.Disconnect());
}

// 0x003DA730 | fefates:bytes [tier B]
nn::Result Initialize()
{
    return InitializeCommon("nfc:u");
}

// 0x003DA758 | fefates:bytes [tier B]
DECOMP_NOINLINE nn::nfc::CTR::NfcState GetNfpState()
{
    if (!IsInitialized()) {
        return nn::nfc::CTR::NfcState(0);
    }
    nn::nfc::CTR::NfcState state;
    s_NfcIpc.GetStatus(&state);
    return state;
}

// 0x003DA788 | fefates:bytes [tier B]
nn::Result GetNfpRomInfo(nn::nfp::RomInfo* info)
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    if (info == 0) {
        return nn::Result(RESULT_INVALID_POINTER);
    }
    return ConvertResult(s_NfcIpc.GetNfpRomInfo(info));
}

// 0x003DA7D0 | tier C (confirmed by the code)
nn::Result StopDetection()
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    return ConvertResult(s_NfcIpc.StopDetection());
}

// 0x003DA804 | fefates:bytes [tier B]
nn::Result StartDetection()
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    return ConvertResult(s_NfcIpc.StartDetection(0));
}

// 0x003DA83C | fefates:bytes [tier B]
nn::Result GetConnectResult(nn::Result* result)
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    if (result == 0) {
        return nn::Result(RESULT_INVALID_POINTER);
    }
    nn::Result ipcResult = ConvertResult(s_NfcIpc.GetConnectResult(result));
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = ConvertResult(*result);
    return nn::Result();
}

// 0x003DA8A0 (name is ours)
void InitializeParameter(nn::nfp::CTR::Parameter* parameter)
{
    if (parameter == 0) {
        return;
    }
    ClearParameter(parameter, MODE_DEFAULT);
}

// 0x003DA944 | fefates:bytes [tier B]
bool StartAmiiboSettings(nn::nfp::CTR::Parameter* parameter)
{
    if (parameter == 0) {
        return false;
    }
    if (GetNfpState() != 0) {
        parameter->result = PARAMETER_RESULT_NFC_IN_USE;
        return false;
    }
    u16 version;
    if (nn::applet::CTR::GetAppletVersion(AMIIBO_SETTINGS_APPLET_ID, &version).IsSuccess()) {
        s32 mode = parameter->mode;
        if (mode == 0 || mode == 1 || mode == 2 || mode == MODE_UPDATE || mode == MODE_SPECIAL) {
            if (nn::applet::CTR::detail::PrepareToStartSystemApplet(AMIIBO_SETTINGS_APPLET_ID).IsSuccess() &&
                nn::applet::CTR::detail::StartSystemApplet(AMIIBO_SETTINGS_APPLET_ID,
                                                           reinterpret_cast<const u8*>(parameter),
                                                           sizeof(*parameter),
                                                           nn::applet::CTR::INVALID_HANDLE)
                    .IsSuccess()) {
                u32 appletId;
                s32 size;
                nn::applet::CTR::detail::WaitForStarting(&appletId, reinterpret_cast<u8*>(parameter),
                                                         sizeof(*parameter), &size, 0,
                                                         nn::applet::CTR::WAIT_INFINITE);
                if (appletId == AMIIBO_SETTINGS_APPLET_ID && size == sizeof(*parameter)) {
                    return true;
                }
            }
        }
    }
    parameter->result = PARAMETER_RESULT_FAILED;
    return false;
}

// 0x003DAA3C | fefates:bytes [tier B]
nn::Result GetTargetConnectionStatus(nn::nfp::TargetConnectionStatus* status)
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    if (status == 0) {
        return nn::Result(RESULT_INVALID_POINTER);
    }
    s32 value;
    nn::Result result = ConvertResult(s_NfcIpc.GetTargetConnectionStatus(&value));
    if (result.IsFailure()) {
        return result;
    }
    *status = static_cast<nn::nfp::TargetConnectionStatus>(value);
    return nn::Result();
}

// 0x003DAA9C | fefates:bytes [tier B]
bool IsAmiiboSettingsAvailable()
{
    u16 version;
    return nn::applet::CTR::GetAppletVersion(AMIIBO_SETTINGS_APPLET_ID, &version).IsSuccess();
}

// 0x003DAABC | fefates:bytes [tier B]
void InitializeParameterForUpdate(nn::nfp::CTR::Parameter* parameter)
{
    if (parameter == 0) {
        return;
    }
    ClearParameter(parameter, MODE_UPDATE);
}

namespace {
// 0x003DAB60 | fefates:bytes [tier B]
// a failure of the NFC service as the same failure of this library, any other failure as
// RESULT_UNKNOWN
nn::Result ConvertResult(nn::Result result)
{
    if (result.IsSuccess()) {
        return nn::Result();
    }
    if (result.GetModule() != MODULE_NFC) {
        return nn::Result(RESULT_UNKNOWN);
    }
    return nn::Result((result.GetLevel() << 27) | (result.GetSummary() << 21) | (MODULE_NFP << 10) |
                      result.GetDescription());
}

// 0x003DABB4 | fefates:bytes [tier B]
nn::Result InitializeCommon(const char* serviceName)
{
    if (IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    nn::Result result = nn::srv::GetServiceHandle(&s_Session, serviceName, strlen(serviceName), 0);
    if (result.IsFailure()) {
        return result;
    }
    s_NfcIpc = nn::nfc::CTR::NfcIpc(s_Session);
    s_NfcIpcMaster = nn::nfc::CTR::NfcIpcMaster(s_Session);
    result = ConvertResult(s_NfcIpc.Initialize(nn::nfc::CTR::MODE_NFP));
    if (result.IsFailure()) {
        nn::svc::CloseHandle(s_Session);
        s_Session = nn::Handle();
        if (nn::Result::ConstRange<-5, 6, 97, 600, 600, 600>::Includes(result)) {
            nndbgPanic();
        }
    }
    return result;
}
} // namespace

// 0x003DAC50 (name is ours, after NFC:StartCommunication)
nn::Result StartCommunication()
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    return ConvertResult(s_NfcIpc.StartCommunication());
}

// 0x003DAC84 (name is ours, after NfcIpc::Command0A)
nn::Result Command0A()
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    return ConvertResult(s_NfcIpc.Command0A());
}

// 0x003DACB8 (name is ours, after NFC:ResetTagScanState)
nn::Result ResetTagScanState()
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    return ConvertResult(s_NfcIpc.ResetTagScanState());
}

// 0x003DACEC | fefates:bytes [tier B]
nn::Result Finalize()
{
    if (!IsInitialized()) {
        return nn::Result(RESULT_INVALID_STATE);
    }
    nn::Result result = ConvertResult(s_NfcIpc.Finalize(nn::nfc::CTR::MODE_NFP));
    nn::svc::CloseHandle(s_Session);
    s_Session = nn::Handle();
    return result;
}

} // namespace CTR
} // namespace nfp
} // namespace nn

// 0x007E5574 | fefates:bytes [tier B]
template bool nn::Result::ConstRange<-5, 6, 97, 600, 600, 600>::Includes(nn::Result);
