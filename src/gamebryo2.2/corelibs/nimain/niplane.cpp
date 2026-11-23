// gamebryo2.2/corelibs/nimain/niplane.cpp: NiSphere, NiPlane (4 functions)
#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiColor.h"
#include "../../../../include/NiPlane.h"
#include "../../../../include/NiPoint3.h"
#include "../../../../include/NiSphere.h"
#include "../../../../include/NiStream.h"

inline void NiBinaryStream::Read(float* value)
{
    unsigned int size = 4;
    m_uiAbsoluteCurrentPos += m_pfnRead(this, value, 4, &size, 1);
}

inline void NiBinaryStream::Write(float* value)
{
    unsigned int size = 4;
    m_uiAbsoluteCurrentPos += m_pfnWrite(this, value, 4, &size, 1);
}

// Constructs a NiPlane with normal set to NiPoint3::ZERO and constant set to 0.0f.
// @fnv 0x00a69940
// pdb: NiPlane::NiPlane | NiPlane.obj | debug, call graph, low
NiPlane::NiPlane()
{
    *reinterpret_cast<NiPoint3*>(&m_kNormal) = NiPoint3::ZERO;
    m_fConstant = 0.0f;
}

// Loads the sphere's center and radius from the given stream.
// @fnv 0x00a69ac0
// pdb: NiSphere::LoadBinary | NiSphere.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiSphere::LoadBinary(NiStream& kStream)
{
    reinterpret_cast<NiColor*>(&m_kCenter)->LoadBinary(kStream);
    kStream.m_pkIstr->Read(&m_fRadius);
}

// Saves the sphere's center and radius to the stream's output.
// @fnv 0x00eb5410
// pdb: NiSphere::SaveBinary | NiSphere.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiSphere::SaveBinary(NiStream& kStream)
{
    reinterpret_cast<NiColor*>(&m_kCenter)->SaveBinary(kStream);
    kStream.m_pkOstr->Write(&m_fRadius);
}

extern "C" size_t __cdecl strlen(const char*);
void* __cdecl _NiMalloc(unsigned int);
int __cdecl _NiSprintf(char*, unsigned int, const char*, ...);
// Allocates a string sized from the prefix length and formats the four floats as "(%g,%g,%g), %g", returning the buffer.
// @fnv 0x00a69a60
// src: XBOX_RETAIL_PDB 0x00a69a60
// pdb: NiPlane::GetViewerString | NiPlane.obj | retail, string-set bridge, high
// pdb params: pPrefix
// pdb locals: pString, uiLen
char* __fastcall NiPlane_GetViewerString(const NiPlane* plane, void*, const char* pPrefix)
{
    char* pBuffer;
    unsigned int uiPrefixLen;
    uiPrefixLen = strlen(pPrefix);
    pBuffer = (char*)_NiMalloc(uiPrefixLen + 0x38);
    _NiSprintf(pBuffer, uiPrefixLen + 0x38, "(%g,%g,%g), %g", plane->m_kNormal, plane->y, plane->z, plane->m_fConstant);
    return pBuffer;
}
