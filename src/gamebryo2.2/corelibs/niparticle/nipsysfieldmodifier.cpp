#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/NiPSysFieldModifier.h"
#include "../../../../include/NiAVObject.h"


// src/gamebryo2.2/corelibs/niparticle/nipsysfieldmodifier.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/niparticle/nipsysfieldmodifier.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

extern char const* const g_01202688_arg1;

// @fnv 0x00c320f0
// src: COMMUNITY 0x00c320f0
char const* const* __cdecl NiPSysFieldModifier_GetRTTI()
{
    return &g_01202688_arg1;
}

extern "C" void __cdecl operator delete(void*, size_t);

// Calls FUN_00c31750, then deletes this with size 48 if flags has bit 0 set, and returns this.
// @fnv 0x00c32100
NiPSysFieldModifier* __thiscall NiPSysFieldModifier::Destroy(unsigned int flags)
{
    NiPSysFieldModifier::~NiPSysFieldModifier();
    if (flags & 1)
        ::operator delete(this, 48);
    return this;
}

// @fnv 0x00c32060
// pdb: NiPSysFieldModifier::IsEqual | NiPSysFieldModifier.obj | retail, upstream name already matches, high
// pdb params: pkObject
// pdb locals: pkDest
bool NiPSysFieldModifier::IsEqual(NiObject* pkObject) const
{
    if (!NiPSysModifier::IsEqual(pkObject))
        return false;

    NiPSysFieldModifier* pkDest = static_cast<NiPSysFieldModifier*>(pkObject);

    if ((m_pkFieldObj && !pkDest->m_pkFieldObj) || (!m_pkFieldObj && pkDest->m_pkFieldObj) ||
        (m_pkFieldObj && pkDest->m_pkFieldObj && !m_pkFieldObj->IsEqual(pkDest->m_pkFieldObj)))
    {
        return false;
    }

    if (pkDest->m_fMagnitude != m_fMagnitude || pkDest->m_fAttenuation != m_fAttenuation || pkDest->m_bUseMaxDistance != m_bUseMaxDistance ||
        pkDest->m_fMaxDistance != m_fMaxDistance)
        return false;

    return true;
}

// The shared declaration leaves stream storage opaque; retain the original
// inline transfers at the game's stream-position and callback offsets.
inline unsigned int NiBinaryStream::BinaryRead(void* pvBuffer, unsigned int uiBytes,
                                               unsigned int* puiSizes, unsigned int uiCount)
{
    unsigned int uiRead = (*(ReadFn*)((char*)this + 8))(this, pvBuffer, uiBytes, puiSizes, uiCount);
    *(unsigned int*)((char*)this + 4) += uiRead;
    return uiRead;
}

inline void NiBinaryStream::BinaryWrite(const void* pvBuffer, unsigned int uiBytes,
                                       unsigned int uiComponentSize)
{
    unsigned int uiSize = uiComponentSize;
    unsigned int* puiSize = &uiSize;
    *(unsigned int*)((char*)this + 4) +=
        (*(WriteFn*)((char*)this + 0xC))(this, pvBuffer, uiBytes, puiSize, 1);
}

// NiStream keeps its storage opaque in the shared declaration.
inline NiBinaryStream& NiStream::Istr()
{
    return **reinterpret_cast<NiBinaryStream**>(reinterpret_cast<char*>(this) + 0x24c);
}

inline NiBinaryStream* NiStream::GetBinaryStream()
{
    return *reinterpret_cast<NiBinaryStream**>(reinterpret_cast<char*>(this) + 0x250);
}

// Calls the base loader, then reads m_fInfluence, m_fFieldAttenuation, a byte flag into m_bEnableMaxDistance, and m_fDistanceLimit from the stream, and sets m_fFieldStrength to m_fDistanceLimit squared.
// @fnv 0x00c32120
// pdb: NiPSysFieldModifier::LoadBinary | NiPSysFieldModifier.obj | retail, upstream name already matches, high
// pdb params: kStream
// pdb locals: kBool
void NiPSysFieldModifier::LoadBinary(NiStream& kStream)
{
    NiStream* apOwner = &kStream;
    NiPSysModifier::LoadBinary(*apOwner);
    apOwner->ReadLinkID();
    NiBinaryStream* s = &apOwner->Istr();
    {
        unsigned int n = 4;
        s->BinaryRead(&m_fMagnitude, 4, &n, 1);
    }
    s = &apOwner->Istr();
    {
        unsigned int n = 4;
        s->BinaryRead(&m_fAttenuation, 4, &n, 1);
    }
    unsigned char flag;
    s = &apOwner->Istr();
    {
        unsigned int n = 1;
        s->BinaryRead(&flag, 1, &n, 1);
    }
    m_bUseMaxDistance = (flag != 0);
    s = &apOwner->Istr();
    {
        unsigned int n = 4;
        s->BinaryRead(&m_fMaxDistance, 4, &n, 1);
    }
    m_fMaxDistanceSqr = m_fMaxDistance * m_fMaxDistance;
}

inline void SerializeValue(NiStream& kStream, const float& fValue)
{
    kStream.GetBinaryStream()->BinaryWrite(&fValue, sizeof(float), sizeof(float));
}

inline void SerializeValue(NiStream& kStream, const unsigned char& ucValue)
{
    kStream.GetBinaryStream()->BinaryWrite(&ucValue, sizeof(unsigned char), sizeof(unsigned char));
}

// Calls FUN_00c31950, then writes the link ID and the float, float, bool and float fields to the stream.
// @fnv 0x00c321f0
// pdb: NiPSysFieldModifier::SaveBinary | NiPSysFieldModifier.obj | retail, upstream name already matches, high
// pdb params: kStream
// pdb locals: kBool
void NiPSysFieldModifier::SaveBinary(NiStream& kStream)
{
    FUN_00c31950(kStream);
    kStream.SaveLinkID(reinterpret_cast<const NiObject*>(m_pkFieldObj));
    SerializeValue(kStream, m_fMagnitude);
    SerializeValue(kStream, m_fAttenuation);
    unsigned char ucUseMaxDistance = m_bUseMaxDistance;
    SerializeValue(kStream, ucUseMaxDistance);
    SerializeValue(kStream, m_fMaxDistance);
}

struct S_00c322b0_Arr
{
    int unk00;
    int unk04;
    unsigned short size;
    unsigned short esize;
    unsigned short unk0c;
    unsigned short growBy;
    void FUN_0096ad30(unsigned int);
    void FUN_0096ae90(unsigned int, char*&);
};
// The shared NiPSysModifier declaration has no viewer-string override yet.
// Retain only the base call declaration; all field storage uses the shared class.
struct FieldModifierViewerBase
{
    void GetViewerStrings(S_00c322b0_Arr* a);
};
extern const char* DAT_012027e0;
char* __cdecl NiViewerStrings_NiGetViewerString_Title(const char* s);
char* __cdecl NiGetViewerString(const char* pcPrefix, ...);
char* __cdecl NiGetViewerString_00a7d550(const char* name, float value);
char* __cdecl NiGetViewerString_00a7d5e0(const char* pcPrefix, const char* pcValue);
__forceinline void S_00c322b0_Add(S_00c322b0_Arr* a, char* s)
{
    unsigned int i = a->esize;
    if (i >= a->size)
        a->FUN_0096ad30(i + a->growBy);
    a->FUN_0096ae90(i, s);
}
// @fnv 0x00c322b0
// pdb: NiPSysFieldModifier::GetViewerStrings | NiPSysFieldModifier.obj | retail, string-set bridge, high
// pdb params: pkStrings
// pdb locals: pcFieldObj
// A free fastcall helper preserves the thiscall register and stack arguments
// until the shared class declares its viewer-string override.
void __fastcall GetFieldModifierViewerStrings(NiPSysFieldModifier* self, void*, S_00c322b0_Arr* a)
{
    reinterpret_cast<FieldModifierViewerBase*>(self)->GetViewerStrings(a);
    S_00c322b0_Add(a, NiViewerStrings_NiGetViewerString_Title(DAT_012027e0));
    S_00c322b0_Add(a, NiGetViewerString_00a7d5e0("Field Object", self->m_pkFieldObj ? self->m_pkFieldObj->m_kName.m_kHandle : "None"));
    S_00c322b0_Add(a, NiGetViewerString_00a7d550("Magnitude", self->m_fMagnitude));
    S_00c322b0_Add(a, NiGetViewerString_00a7d550("Attenuation", self->m_fAttenuation));
    S_00c322b0_Add(a, NiGetViewerString("Use Max Distance", self->m_bUseMaxDistance));
    S_00c322b0_Add(a, NiGetViewerString_00a7d550("Max Distance", self->m_fMaxDistance));
}
