#pragma once

// The functions of the transport module (the file name is ours).

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace transport {
// makes the transport heap ("pia transport heap", all the rest of the pia memory)
nn::Result Initialize(); // 0x0044D378 | fefates:bytes [tier B]
void Finalize(); // 0x0045F958 | fefates:bytes [tier B]
// begin / end of the setup of the module: the instances of the transport classes are created in
// between, on the transport heap
nn::Result BeginSetup(); // 0x0044D32C | fefates:bytes [tier B]
nn::Result EndSetup(); // 0x0045F8FC | fefates:bytes [tier B]
bool IsInitialized(); // 0x0044DE84 | fefates:callgraph [tier C]
bool IsInSetupMode(); // 0x0044DE74 | fefates:callgraph [tier C]

// conversion between the station index and the station id with the Transport instance; without
// it (or for an unknown station) the result is the unidentified one
StationId Conv2StationId(nn::pia::StationIndex stationIndex); // 0x0044EE84 | fefates:callseq [tier C]
StationIndex Conv2StationIndex(nn::pia::StationId stationId); // 0x00454150 | fefates:bytes [tier B]
} // namespace transport
} // namespace pia
} // namespace nn
