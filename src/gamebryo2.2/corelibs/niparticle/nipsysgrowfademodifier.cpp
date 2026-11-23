// gamebryo2.2/corelibs/niparticle/nipsysgrowfademodifier.cpp: NiPSysGrowFadeModifier (8 functions)

#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiPSysModifier.h"
#include "../../../../include/NiStream.h"
#include "nipsysgrowfademodifier_decls.h"
#include "../../../../include/NiObject.h"
#include "../../../../include/StrArr.h"
// Loads the modifier's base data, then four fields and, for stream version above 16, one more field from the stream.
// @fnv 0x00c2a530
// pdb: NiPSysGrowFadeModifier::LoadBinary | NiPSysGrowFadeModifier.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiPSysGrowFadeModifier::LoadBinary(NiStream& kStream)
{
    NiPSysModifier::LoadBinary(kStream);
    NiBinaryStreamLoad(*kStream.m_pkIstr, &m_fGrowTime);
    NiBinaryStreamLoad(*kStream.m_pkIstr, &m_usGrowGeneration);
    NiBinaryStreamLoad(*kStream.m_pkIstr, &m_fFadeTime);
    NiBinaryStreamLoad(*kStream.m_pkIstr, &m_usFadeGeneration);
    if (kStream.m_BSStreamHeader.uiVersion > 16)
        NiBinaryStreamLoad(*kStream.m_pkIstr, &m_fBaseScale);
}

struct GlobalObjectO2_00a7b610;
extern GlobalObjectO2_00a7b610 g_012026f8;

// @fnv 0x00c2a360
// src: COMMUNITY 0x00c2a360
GlobalObjectO2_00a7b610* __cdecl NiPSysGrowFadeModifier_GetRTTI()
{
    return &g_012026f8;
}

void __cdecl FUN_00c31750();
void __cdecl operator delete(void*, size_t);
struct FUN_00c2a370_shell : NiPSysGrowFadeModifier
{
    FUN_00c2a370_shell* FUN_00c2a370(unsigned int flags);
};
// Calls FUN_00c31750, then deletes this with operator delete when flags has bit 0 set, and returns this.
// @fnv 0x00c2a370
FUN_00c2a370_shell* FUN_00c2a370_shell::FUN_00c2a370(unsigned int flags)
{
    FUN_00c31750();
    if (flags & 1)
        operator delete(this, 0x2c);
    return this;
}

struct FUN_00c2a2f0_shell : NiPSysGrowFadeModifier
{
    bool NiPSysGrowFadeModifier_IsEqual(NiObject* pkObject);
};

// @fnv 0x00c2a2f0
// src: XBOX_PDB 0x00c2a2f0
// pdb: NiPSysGrowFadeModifier::IsEqual | NiPSysGrowFadeModifier.obj | retail, upstream name already matches, high
// pdb params: pkObject
// pdb locals: pkDest
bool FUN_00c2a2f0_shell::NiPSysGrowFadeModifier_IsEqual(NiObject* pkObject)
{
    FUN_00c2a2f0_shell* pkDest = (FUN_00c2a2f0_shell*)pkObject;
    if (NiPSysModifier::IsEqual(pkObject) && m_fGrowTime == pkDest->m_fGrowTime &&
        m_usGrowGeneration == pkDest->m_usGrowGeneration && m_fFadeTime == pkDest->m_fFadeTime &&
        m_fBaseScale == pkDest->m_fBaseScale && m_usFadeGeneration == pkDest->m_usFadeGeneration)
        return true;
    return false;
}

extern float DAT_010c09c0;

struct ParticleInfo_00c2a390
{
    float unk00;
    float unk04;
    float unk08;
    float age;
    float lifeSpan;
    unsigned int unk14;
    unsigned short generation;
    unsigned short unk1a;
};
struct ParticleData_00c2a390
{
    unsigned int unk00, unk04, unk08, unk0c;
    unsigned int unk10, unk14, unk18, unk1c;
    unsigned int unk20, unk24, unk28, unk2c;
    unsigned int unk30, unk34, unk38, unk3c;
    unsigned int unk40, unk44, unk48, unk4c;
    unsigned short activeCount;
    unsigned short maxCount;
    float* scales;
    unsigned int unk58, unk5c, unk60, unk64, unk68, unk6c;
    ParticleInfo_00c2a390* particles;
};
struct FUN_00c2a390_shell : NiPSysGrowFadeModifier
{
    void ComputeParticleScales(unsigned int, ParticleData_00c2a390*);
};

// For each active particle, computes a fade/grow ratio from age and lifespan and writes the clamped scale into data->scales.
// @fnv 0x00c2a390
// pdb: NiPSysGrowFadeModifier::Update | NiPSysGrowFadeModifier.obj | debug, object-file run extension, low
// pdb params: fTime, pkData
// pdb locals: finvBaseScale, us, pkCurrentParticle, fTimeLeft, fFade, fScale, fGrow
void FUN_00c2a390_shell::ComputeParticleScales(unsigned int, ParticleData_00c2a390* data)
{
    float scaleRange = 1.0f - m_fBaseScale;
    for (unsigned short i = 0; i < data->activeCount; ++i)
    {
        const ParticleInfo_00c2a390& particle = data->particles[i];
        float fade = 1.0f;
        if (particle.generation == m_usGrowGeneration && particle.age < m_fGrowTime && m_fGrowTime != 0.0f)
        {
            fade = particle.age / m_fGrowTime;
        }

        float grow = 1.0f;
        float remaining = particle.lifeSpan - particle.age;
        if (particle.generation == m_usFadeGeneration && remaining < m_fFadeTime && m_fFadeTime != 0.0f)
        {
            grow = remaining / m_fFadeTime;
        }
        float ratio = grow > fade ? fade : grow;
        float scale = ratio * scaleRange + m_fBaseScale;
        if (scale < DAT_010c09c0)
        {
            scale = DAT_010c09c0;
        }
        data->scales[i] = scale;
    }
}

template <class T> inline void NiBinaryStreamSave(NiBinaryStream& stream, T* value)
{
    unsigned int size = sizeof(T);
    stream.m_uiAbsoluteCurrentPos += stream.m_pfnWrite(&stream, value, sizeof(T), &size, 1);
}

struct FUN_00c2a610_cls : NiPSysGrowFadeModifier
{
    void LoadGrowFadeModifier(NiStream& kStream);
};

// Saves the base modifier and the five grow/fade fields to the output stream.
// @fnv 0x00c2a610
// pdb: NiPSysGrowFadeModifier::SaveBinary | NiPSysGrowFadeModifier.obj | retail, object-file gap fill, high
// pdb params: kStream
void FUN_00c2a610_cls::LoadGrowFadeModifier(NiStream& kStream)
{
    NiPSysModifier::SaveBinary(kStream);
    NiBinaryStreamSave(*kStream.m_pkOstr, &m_fGrowTime);
    NiBinaryStreamSave(*kStream.m_pkOstr, &m_usGrowGeneration);
    NiBinaryStreamSave(*kStream.m_pkOstr, &m_fFadeTime);
    NiBinaryStreamSave(*kStream.m_pkOstr, &m_usFadeGeneration);
    NiBinaryStreamSave(*kStream.m_pkOstr, &m_fBaseScale);
}

struct GlobalObjectO2_00a7b610;
extern GlobalObjectO2_00a7b610 g_012026f8;
char* __cdecl NiViewerStrings_NiGetViewerString_Title(char* pcText);
char* __cdecl NiGetViewerString_00a7d550(const char* pcName, float fValue);
char* __cdecl NiGetViewerString_00a7d490(const char* pcPrefix, unsigned short usValue);
inline void StrArr::AppendReference(char*& element)
{
    unsigned int uiIndex = m_usSize;
    if (uiIndex >= m_usMaxSize)
        SetSize(uiIndex + m_usGrowBy);
    SetAt(uiIndex, element);
}
// The shared base has no declaration for the modifier-specific viewer-string method.
// This member-only adapter uses the shared layout without duplicating its fields.
struct NiPSysModifierViewerStrings : NiPSysModifier
{
    void GetViewerStrings(StrArr* pkStrings);
};
struct FUN_00c2a6e0_shell : NiPSysGrowFadeModifier
{
    void AppendGrowFadeModifierStrings(StrArr* pkStrings);
};
// @fnv 0x00c2a6e0
// pdb: NiPSysGrowFadeModifier::GetViewerStrings | NiPSysGrowFadeModifier.obj | retail, string-set bridge, high
// pdb params: pkStrings
void FUN_00c2a6e0_shell::AppendGrowFadeModifierStrings(StrArr* pkStrings)
{
    ((NiPSysModifierViewerStrings*)this)->GetViewerStrings(pkStrings);
    char* pcString = NiViewerStrings_NiGetViewerString_Title(*(char**)&g_012026f8);
    pkStrings->AppendReference(pcString);
    pcString = NiGetViewerString_00a7d550("Grow Time", m_fGrowTime);
    pkStrings->AppendReference(pcString);
    pcString = NiGetViewerString_00a7d490("Grow Generation", m_usGrowGeneration);
    pkStrings->AppendReference(pcString);
    pcString = NiGetViewerString_00a7d550("Fade Time", m_fFadeTime);
    pkStrings->AppendReference(pcString);
    pcString = NiGetViewerString_00a7d490("Fade Generation", m_usFadeGeneration);
    pkStrings->AppendReference(pcString);
    pcString = NiGetViewerString_00a7d550("Base Scale", m_fBaseScale);
    pkStrings->AppendReference(pcString);
}

inline void* operator new(size_t, void* p)
{
    return p;
}
extern "C" void* __cdecl Object_DispatchCommandFour(unsigned int size);
class A4b0Cls : public NiPSysModifier
{
  public:
    virtual void v00();
    float m_18;
    short m_1c;
    float m_20;
    short m_24;
    float m_28;
    A4b0Cls()
    {
        m_18 = 0.0f;
        m_1c = 0;
        m_20 = 0.0f;
        m_24 = 0;
        m_28 = 0.0f;
    }
    NiPSysModifier* __thiscall NiPSysGrowFadeModifier_CreateClone(CloningProcess* kCloning);
};
// Allocates an A4b0Cls, copies the base modifier and grow/fade fields, and returns it.
// @fnv 0x00c2a4b0
// src: XBOX_PDB 0x00c2a4b0
// pdb: NiPSysGrowFadeModifier::CreateClone | NiPSysGrowFadeModifier.obj | retail, upstream name already matches, high
// pdb params: kCloning
// pdb locals: pkObject
NiPSysModifier* A4b0Cls::NiPSysGrowFadeModifier_CreateClone(CloningProcess* kCloning)
{
    void* mem = Object_DispatchCommandFour(sizeof(A4b0Cls));
    A4b0Cls* p = mem ? ::new (mem) A4b0Cls : 0;
    NiPSysModifier::CopyMembers(p, *kCloning);
    p->m_18 = m_18;
    p->m_1c = m_1c;
    p->m_20 = m_20;
    p->m_24 = m_24;
    p->m_28 = m_28;
    return p;
}
