#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace os {
class Event;
}
} // namespace nn

// the settings of a connection (C struct, the name is from the symbols; ac:u passes it as a 0x200
// byte buffer, the contents are not used by the code here)
struct nnacConfig
{
    u8 data[0x200];
};

namespace nn {
namespace ac {
namespace CTR {
// a kind of access point (the type name is from the symbols; ConnectAsyncWithoutEula passes 0x40,
// the value names are ours)
enum ApType : u32
{
    AP_TYPE_0x40 = 0x40,
};
// the priority of the connection to an access point (the type name is from the symbols; the values
// are not named: 0 makes ConnectAsyncWithoutEula take the network exclusively)
enum InfraPriority : u8
{
    INFRA_PRIORITY_0 = 0,
};

nn::Result Initialize(); // 0x003454A4 | fefates:bytes [tier B]
nn::Result Finalize(); // 0x00345E0C | fefates:bytes [tier B]
bool IsConnected(); // 0x003455BC | nintendogs:bytes [tier A]
// the session of ac:i (never opened by ACNL)
DECOMP_NOINLINE bool IsInitializedInternal(); // 0x00345730 | nintendogs:callgraph [tier A]

nn::Result CreateDefaultConfig(nnacConfig* pConfig); // 0x00345718 (name is ours, after the command)
// connects (blocking); Connect asks for the EULA first
nn::Result Connect(nnacConfig& config); // 0x00345DD4 | fefates:bytes [tier B]
nn::Result ConnectWithoutEula(nnacConfig& config); // 0x00345634 | fefates:bytes [tier B]
nn::Result ConnectAsyncWithoutEula(nnacConfig& config, nn::os::Event* pEvent); // 0x00345784 | fefates:bytes [tier B]
nn::Result Close(); // 0x003458DC | fefates:bytes [tier B]
nn::Result RegisterDisconnectEvent(nn::os::Event* pEvent); // 0x00345864 | fefates:bytes [tier B]
nn::Result GetLastErrorCode(unsigned int* pCode); // 0x0034561C (name is ours, after the command)
nn::Result GetLastDetailErrorCode(unsigned int* pCode); // 0x0034576C (name is ours, after the command)
} // namespace CTR
} // namespace ac
} // namespace nn
