#pragma once

// Result codes of pia and the pointer check its functions use (the file name is ours).
//
// All pia results are in module 82. Level and summary after 3dbrew ("Error codes"); the meaning
// of the descriptions is ours, taken from the places that return them.

#include "types.h"

namespace nn {
namespace pia {
namespace common {

// usage, invalid state, 33: Initialize called again
const bit32 RESULT_ALREADY_INITIALIZED = 0xE0A14821;
// usage, out of resource, 32: zlib could not get memory
const bit32 RESULT_OUT_OF_MEMORY = 0xE0614820;
// permanent, internal, 34: a packet or message is not in the expected format
const bit32 RESULT_INVALID_FORMAT = 0xD9614822;
// usage, invalid argument, 35: the output buffer is too small
const bit32 RESULT_BUFFER_SHORTAGE = 0xE0E14823;
// permanent, canceled, 36: the call was canceled (CallContext::SignalCancel)
const bit32 RESULT_CANCELED = 0xD9214824;
// permanent, invalid state, 37: the socket service lost the network or the connection
// (inet::Socket; name is ours)
const bit32 RESULT_SOCKET_UNAVAILABLE = 0xD8A14825;
// permanent, invalid state, 194: error number 3 of the socket service (inet::Socket; name is ours)
const bit32 RESULT_SOCKET_ERROR_194 = 0xD8A148C2;
// usage, invalid argument, 38
const bit32 RESULT_INVALID_ARGUMENT = 0xE0E14826;
// permanent, invalid state, 193: the NAT check got no usable replies, or the socket of the NAT
// session could not be opened (inet::NatDetectionJob, NexFacade; name is ours)
const bit32 RESULT_NAT_CHECK_FAILED = 0xD8A148C1;
// permanent, invalid state, 195: the addresses of the NAT check servers could not be resolved
// (inet::NatTraverser, NatServerAddressResolveJob; name is ours)
const bit32 RESULT_NAT_SERVER_NOT_FOUND = 0xD8A148C3;
// usage, invalid state, 39: e.g. not in setup mode
const bit32 RESULT_INVALID_STATE = 0xE0A14827;
// temporary, invalid state, 48: one of the states of transport::Transport (Transport::SetState)
const bit32 RESULT_INVALID_STATE_TEMPORARY = 0xD0A14830;
// usage, internal, 42: an error of a library pia uses (zlib) or an unsupported mode
const bit32 RESULT_INTERNAL_ERROR = 0xE161482A;
// usage, invalid state, 43: the module is not initialized
const bit32 RESULT_NOT_INITIALIZED = 0xE0A1482B;
// temporary, internal, 40: no complete data to receive yet (transport::ReliableSlidingWindow)
const bit32 RESULT_NO_DATA = 0xD1614828;
// permanent, not found, 41: no entry with the key (transport::StationIdTable)
const bit32 RESULT_NOT_FOUND = 0xD8814829;
// permanent, out of resource, 44: no room for the data (transport::ReliableSlidingWindow)
const bit32 RESULT_BUFFER_IS_FULL = 0xD861482C;
// permanent, invalid state, 46: the instance is already created
const bit32 RESULT_ALREADY_EXISTS = 0xD8A1482E;
// permanent, invalid state, 47: no free entry left (transport::StationIdTable)
const bit32 RESULT_NO_FREE_ENTRY = 0xD8A1482F;
// permanent, status changed, 45: the station did not answer in time (transport::ConnectStationJob)
const bit32 RESULT_TIMEOUT = 0xD941482D;
// permanent, internal, 129: the station denied the connection for another reason
// (transport::ConnectStationJob)
const bit32 RESULT_CONNECTION_FAILED = 0xD9614881;
// permanent, internal, 130: the station refused the connection (deny reason 1: no room, or it
// knows the station by another address; transport::ConnectStationJob)
const bit32 RESULT_CONNECTION_REFUSED = 0xD9614882;
// permanent, invalid state, 131: the station has another protocol version (deny reason 2;
// transport::ConnectStationJob)
const bit32 RESULT_INCOMPATIBLE_VERSION = 0xD8A14883;
// permanent, invalid state, 132: no communication with the station (transport::ReliableProtocol
// before Startup or to a station whose window does not run)
const bit32 RESULT_NOT_IN_COMMUNICATION = 0xD8A14884;
// usage, invalid result value, module 255: a result that is not set yet (after 3dbrew)
const bit32 RESULT_NOT_SET = 0xE7E3FFFF;

// the results of session::Session and its jobs (all permanent, invalid state; names are ours)
// 37 (the code of RESULT_SOCKET_UNAVAILABLE): not in a session (Session::GetStatus 5)
const bit32 RESULT_NOT_IN_SESSION = 0xD8A14825;
// 243: the matchmake session does not exist any more (inet::NexMatchDestroySessionJob ignores it)
const bit32 RESULT_MATCHMAKE_SESSION_GONE = 0xD8A148F3;
// 254: the session was left with an error (Session::GetStatus 4)
const bit32 RESULT_SESSION_DISCONNECTED = 0xD8A148FE;
// 256: the stations of the joint session are not reachable any more (inet::NexJointSessionJob)
const bit32 RESULT_JOINT_SESSION_STATION_LOST = 0xD8A14900;
// 258: the owner of the session left
const bit32 RESULT_SESSION_OWNER_LEFT = 0xD8A14902;
// 259: the session could not be unregistered (inet::NexMatchDestroySessionJob)
const bit32 RESULT_UNREGISTER_FAILED = 0xD8A14903;
// status, invalid state, 103: the other stations did not confirm the closed participation in
// time (CloseParticipationJob); also a canceled scan of the local network (LocalScanNetworkJob)
const bit32 RESULT_INVALID_STATE_103 = 0xD0A14867;
// 261 and 262: the change of the participation failed (ConfigParticipationJobBase: 261 after a
// timeout as the host or with a request of the host, 262 otherwise and on a mode conflict)
const bit32 RESULT_CONFIG_PARTICIPATION_FAILED_261 = 0xD8A14905;
const bit32 RESULT_CONFIG_PARTICIPATION_FAILED_262 = 0xD8A14906;
// fatal, invalid state, 196: kept by inet::NexMatchDestroySessionJob
const bit32 RESULT_FATAL_196 = 0xF8A148C4;
// the matchmaking on the server failed (inet::NexMatchmakeSession maps the nex results
// 0x800300C8, CD, CE, CF, D5, D6, D1 / DB, D7 / D4 and DA to them)
const bit32 RESULT_MATCHMAKE_FAILED_244 = 0xD8A148F4;
const bit32 RESULT_MATCHMAKE_FAILED_245 = 0xD8A148F5;
const bit32 RESULT_MATCHMAKE_FAILED_246 = 0xD8A148F6;
const bit32 RESULT_MATCHMAKE_FAILED_247 = 0xD8A148F7;
const bit32 RESULT_MATCHMAKE_FAILED_249 = 0xD8A148F9;
const bit32 RESULT_MATCHMAKE_FAILED_250 = 0xD8A148FA;
const bit32 RESULT_MATCHMAKE_FAILED_251 = 0xD8A148FB;
const bit32 RESULT_MATCHMAKE_FAILED_252 = 0xD8A148FC;
const bit32 RESULT_MATCHMAKE_FAILED_253 = 0xD8A148FD;
// 260: the local station is the host but not the owner of the session or the other way round
// (inet::NexMatchmakeSession)
const bit32 RESULT_HOST_OWNER_MISMATCH = 0xD8A14904;

// the results of session::JoinMeshJob (all permanent, invalid state)
// 224: the host denied the join (response code 1)
const bit32 RESULT_JOIN_DENIED = 0xD8A148E0;
// 225: the join failed (no connection to the host, transport error, no request sent)
const bit32 RESULT_JOIN_FAILED = 0xD8A148E1;
// 229: the host refused the join (response code 0) or the connection to it
const bit32 RESULT_JOIN_REFUSED = 0xD8A148E5;
// 230: the join response cannot be used
const bit32 RESULT_INVALID_JOIN_RESPONSE = 0xD8A148E6;
// 231 to 237, 242: the connection to a station of the mesh failed (the mesh update sets them;
// the reasons are not known yet)
const bit32 RESULT_STATION_CONNECTION_FAILED_E7 = 0xD8A148E7;
const bit32 RESULT_STATION_CONNECTION_FAILED_E8 = 0xD8A148E8;
const bit32 RESULT_STATION_CONNECTION_FAILED_E9 = 0xD8A148E9;
const bit32 RESULT_STATION_CONNECTION_FAILED_EA = 0xD8A148EA;
const bit32 RESULT_STATION_CONNECTION_FAILED_EB = 0xD8A148EB;
const bit32 RESULT_STATION_CONNECTION_FAILED_EC = 0xD8A148EC;
const bit32 RESULT_STATION_CONNECTION_FAILED_ED = 0xD8A148ED;
const bit32 RESULT_STATION_CONNECTION_FAILED_F2 = 0xD8A148F2;
// 238 to 241: the host kicked us out while we joined (kickout reason 4, 5, 6, others;
// session::KickoutManageJob)
const bit32 RESULT_JOIN_KICKED_OUT_4 = 0xD8A148EE;
const bit32 RESULT_JOIN_KICKED_OUT_5 = 0xD8A148EF;
const bit32 RESULT_JOIN_KICKED_OUT_6 = 0xD8A148F0;
const bit32 RESULT_JOIN_KICKED_OUT = 0xD8A148F1;
// 248: not in the mesh (session::Mesh)
const bit32 RESULT_NOT_JOINED = 0xD8A148F8;
// status, invalid state, 49: the station is being kicked out already (session::KickoutManageJob)
const bit32 RESULT_ALREADY_KICKED_OUT = 0xC8A14831;

// the results of the local network (local::LocalNetwork, its managers and jobs; names are ours)
// permanent, not found, 101: the destination of a send is not in the network
const bit32 RESULT_LOCAL_DESTINATION_NOT_FOUND = 0xD8814865;
// permanent, invalid state, 104: the network is lost (also the UDS result 0xC9411002)
const bit32 RESULT_LOCAL_NETWORK_LOST = 0xD8A14868;
// permanent, invalid state, 106: the network to connect to does not take participants
const bit32 RESULT_LOCAL_CONNECT_FAILED_106 = 0xD8A1486A;
// permanent, invalid state, 107: the wireless communication is not available (UDS 0xC8A113EA)
const bit32 RESULT_LOCAL_NETWORK_UNAVAILABLE = 0xD8A1486B;
// permanent, invalid state, 108 and 109: the connection failed (UDS 0xC88113FA and 0xC8611001)
const bit32 RESULT_LOCAL_CONNECT_FAILED_108 = 0xD8A1486C;
const bit32 RESULT_LOCAL_CONNECT_FAILED_109 = 0xD8A1486D;

// pia accepts only pointers into the application's address range [0x00100000, 0x40000000)
// (inline everywhere: "sub rX, p, #0x100000; cmp rX, #0x3FF00000"; the name is ours)
inline bool IsValidPointer(const void* p)
{
    return reinterpret_cast<uptr>(p) - 0x00100000 < 0x3FF00000;
}

} // namespace common
} // namespace pia
} // namespace nn
