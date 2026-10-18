#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// Reads root CA certificates from the shared content of the certificates (cached in
// GlobalVariables::m_RootCaCache). The names of the members and of the constants are ours.
// RTTI N2nn3nex12RootCaReaderE @ 0x008CE194
// vtable 0x008FC660 (vptr 0x008FC668), offset_to_top 0, 2 entries
class RootCaReader : public ::nn::nex::RootObject
{
public:
    // the shared content with the certificates (and its content index)
    static const u64 CERTIFICATES_TITLE_ID = 0x0004009B00010602ULL;

    // (inline)
    RootCaReader() : m_UsesCache(true) {}
    virtual ~RootCaReader(); // 0x00360378 slot 0x00
    // 0x00360374 (deleting dtor)
    // the certificates of the files (all from the cache, or the content is mounted); false on a
    // failure
    bool Read(nn::nex::qVector<unsigned char>* pCertificates, const char** ppNames, unsigned int count); // 0x0035FAA4 | fefates:bytes-fuzzy [tier B]
    // the files of the mounted content (or of the cache)
    bool ReadImpl(nn::nex::qVector<unsigned char>* pCertificates, const char** ppNames, unsigned int count); // 0x0035FD78 | fefates:bytes-fuzzy [tier B]

    bool m_UsesCache; // 0x4
};
ASSERT_SIZE(RootCaReader, 0x8);

// The settings of the HPP server of DataStore and of the shared content (in .data; the names are
// ours).
struct HppSettings
{
    char m_Key[12];             // 0x00
    const char* m_ArchiveName;  // 0x0C (of the mounted content: "conf:")
    const char* m_Server;       // 0x10
    const char* m_Path;         // 0x14
};
extern HppSettings g_HppSettings;
} // namespace nex
} // namespace nn
