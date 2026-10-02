#pragma once

#include "decomp.h"

namespace nn {
namespace ac {
namespace CTR {
void Initialize(); // 0x003454A4 | fefates:bytes [tier B]
void IsConnected(); // 0x003455BC | nintendogs:bytes [tier A]
void ConnectWithoutEula(nnacConfig&); // 0x00345634 | fefates:bytes [tier B]
void IsInitializedInternal(); // 0x00345730 | nintendogs:callgraph [tier A]
void ConnectAsyncWithoutEula(nnacConfig&, nn::os::Event*); // 0x00345784 | fefates:bytes [tier B]
void RegisterDisconnectEvent(nn::os::Event*); // 0x00345864 | fefates:bytes [tier B]
void Close(); // 0x003458DC | fefates:bytes [tier B]
void Connect(nnacConfig&); // 0x00345DD4 | fefates:bytes [tier B]
void Finalize(); // 0x00345E0C | fefates:bytes [tier B]
void IsInitialized(); // 0x0048A9E0 | fefates:bytes [tier B]
} // namespace CTR
} // namespace ac
} // namespace nn
