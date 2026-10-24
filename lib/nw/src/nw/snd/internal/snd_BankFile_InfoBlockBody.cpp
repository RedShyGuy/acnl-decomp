#include "nw/snd/internal/snd_BankFile_InfoBlockBody.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013D6C4 | nintendogs:bytes [tier A]
const nw::snd::internal::BinaryTable<nw::snd::internal::WaveId>* nw::snd::internal::BankFile::InfoBlockBody::GetWaveIdTable() const
{
    return static_cast<const BinaryTable<WaveId>*>(GetReferedPointer(this, waveIdTable));
}

// 0x00742D38 | nintendogs:bytes [tier A]
const nw::snd::internal::BankFile::Instrument* nw::snd::internal::BankFile::InfoBlockBody::GetInstrument(int programNo) const
{
    const BinaryReferenceTable* pTable = static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, instrumentTable));
    const BinaryReference& reference = pTable->items[programNo];
    switch (reference.typeId) {
    case TYPE_INSTRUMENT:
        return static_cast<const Instrument*>(pTable->GetReferedItem(programNo));
    case TYPE_NULL_INSTRUMENT:
    default:
        return nullptr;
    }
}

// 0x00742D7C | nintendogs:bytes [tier A]
const nw::snd::internal::BinaryReferenceTable* nw::snd::internal::BankFile::InfoBlockBody::GetInstrumentReferenceTable() const
{
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, instrumentTable));
}

} // namespace internal
} // namespace snd
} // namespace nw
