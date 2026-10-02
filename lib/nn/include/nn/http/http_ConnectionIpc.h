#pragma once

#include "decomp.h"

namespace nn {
namespace http {
class ConnectionIpc
{
public:
    void GetResultCode(int, int*); // 0x0046F728 | fefates:bytes [tier B]
    void AddHeaderField(int, const char*, unsigned int, const char*, unsigned int); // 0x0046F764 | fefates:bytes [tier B]
    void FinalizeClient(); // 0x0046F7C4 | fefates:bytes [tier B]
    void GetHeaderField(int, const char*, unsigned int, char*, unsigned int, unsigned int*); // 0x0046F7EC | fefates:bytes [tier B]
    void SendPostDataRaw(int, const unsigned char*, unsigned int); // 0x0046F858 | fefates:bytes [tier B]
    void SetProxyDefault(int); // 0x0046F89C | mk7dlp:callseq-callee [tier A]
    void CancelConnection(int); // 0x0046F92C | fefates:bytes [tier B]
    void CreateConnection(const char*, unsigned, nn::http::RequestMethod, int*); // 0x0046F95C | mk7dlp:callseq-callee [tier A]
    void DestroyConnection(int); // 0x0046FA14 | fefates:bytes [tier B]
    void SetInternalRootCA(int, unsigned int); // 0x0046FA44 | fefates:bytes [tier B]
    void StartConnectionAsync(int); // 0x0046FA78 | fefates:bytes [tier B]
    void GetConnectionProgress(int, unsigned int*, unsigned int*); // 0x0046FAA8 | fefates:bytes [tier B]
    void GetConnectionSslError(int, int*); // 0x0046FAF0 | fefates:bytes [tier B]
    void SetInternalClientCert(int, unsigned int); // 0x0046FB2C | fefates:bytes [tier B]
    void SetLazyPostDataSetting(int, nn::http::PostDataType); // 0x0046FB60 | fefates:bytes [tier B]
    void NofityFinishSendPostData(int); // 0x0046FBE0 | fefates:bytes [tier B]
    void InitializeConnectionSession(int); // 0x0046FC10 | mk7dlp:callseq-callee [tier A]
    void SetLazyPostDataSettingWithSize(int, nn::http::PostDataType, unsigned int); // 0x0046FC48 | fefates:bytes [tier B]
    void ReadBody(int, unsigned char*, unsigned int); // 0x0046FC88 | fefates:bytes [tier B]
    void SetRootCA(int, const unsigned char*, unsigned int); // 0x0046FCCC | fefates:bytes [tier B]
};
} // namespace http
} // namespace nn
