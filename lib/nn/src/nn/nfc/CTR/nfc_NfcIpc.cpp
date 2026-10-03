#include "nn/nfc/CTR/nfc_NfcIpc.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

#include <string.h>

namespace nn {
namespace nfc {
namespace CTR {

namespace {
// command headers (3dbrew "NFC Services")
const bit32 COMMAND_INITIALIZE = 0x00010040;
const bit32 COMMAND_SHUTDOWN = 0x00020040;
const bit32 COMMAND_START_COMMUNICATION = 0x00030000;
const bit32 COMMAND_START_TAG_SCANNING = 0x00050040;
const bit32 COMMAND_STOP_TAG_SCANNING = 0x00060000;
const bit32 COMMAND_RESET_TAG_SCAN_STATE = 0x00080000;
const bit32 COMMAND_0A = 0x000A0000;
const bit32 COMMAND_GET_TAG_STATE = 0x000D0000;
const bit32 COMMAND_COMMUNICATION_GET_STATUS = 0x000F0000;
const bit32 COMMAND_COMMUNICATION_GET_RESULT = 0x00120000;
const bit32 COMMAND_1A = 0x001A0000;
const bit32 COMMAND_GET_MODEL_INFO = 0x001B0000;
} // namespace

// 0x003DA4B4 | fefates:bytes [tier B]
nn::Result nn::nfc::CTR::NfcIpc::Initialize(nn::nfc::CTR::Mode mode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INITIALIZE;
    *reinterpret_cast<u8*>(&command[1]) = mode;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x003DA4EC | fefates:bytes [tier B]
nn::Result nn::nfc::CTR::NfcIpc::GetNfpRomInfo(nn::nfp::RomInfo* info)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_MODEL_INFO;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    memcpy(info, &command[2], sizeof(*info));
    return nn::Result(command[1]);
}

// 0x003DA528 | fefates:bytes [tier B]
nn::Result nn::nfc::CTR::NfcIpc::StopDetection()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_STOP_TAG_SCANNING;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x003DA550 | tier C (confirmed by the code)
nn::Result nn::nfc::CTR::NfcIpc::StartDetection(u16 protocol)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_START_TAG_SCANNING;
    *reinterpret_cast<u16*>(&command[1]) = protocol;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x003DA588 | fefates:bytes [tier B]
nn::Result nn::nfc::CTR::NfcIpc::GetConnectResult(nn::Result* connectResult)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_COMMUNICATION_GET_RESULT;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *connectResult = nn::Result(command[2]);
    return nn::Result(command[1]);
}

// 0x003DA5BC | fefates:bytes [tier B]
nn::Result nn::nfc::CTR::NfcIpc::GetTargetConnectionStatus(s32* status)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_COMMUNICATION_GET_STATUS;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *status = command[2];
    return nn::Result(command[1]);
}

// 0x003DA5F0 (name after 3dbrew)
nn::Result nn::nfc::CTR::NfcIpc::StartCommunication()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_START_COMMUNICATION;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x003DA618 (name is ours: command 0x000A, unnamed on 3dbrew)
nn::Result nn::nfc::CTR::NfcIpc::Command0A()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_0A;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x003DA640 (name after 3dbrew)
nn::Result nn::nfc::CTR::NfcIpc::ResetTagScanState()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RESET_TAG_SCAN_STATE;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x003DA668 | fefates:bytes [tier B]
nn::Result nn::nfc::CTR::NfcIpc::Finalize(nn::nfc::CTR::Mode mode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SHUTDOWN;
    *reinterpret_cast<u8*>(&command[1]) = mode;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x003DA6A0 (name is ours: command 0x001A, unnamed on 3dbrew)
nn::Result nn::nfc::CTR::NfcIpc::Command1A()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_1A;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x003DA6C8 | fefates:bytes [tier B]
nn::Result nn::nfc::CTR::NfcIpc::GetStatus(nn::nfc::CTR::NfcState* state)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TAG_STATE;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *state = static_cast<nn::nfc::CTR::NfcState>(*reinterpret_cast<u8*>(&command[2]));
    return nn::Result(command[1]);
}

} // namespace CTR
} // namespace nfc
} // namespace nn
