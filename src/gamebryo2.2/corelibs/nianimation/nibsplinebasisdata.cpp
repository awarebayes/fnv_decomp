// gamebryo2.2/corelibs/nianimation/nibsplinebasisdata.cpp: NiBSplineBasisData (10 functions)

#include "../../../../include/NiBSplineBasisData.h"
#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiObject.h"
#include "../../../../include/NiStream.h"

template <class T> inline void NiBinaryStream::ReadScalar(T& value)
{
    unsigned int componentSize = sizeof(T);
    unsigned int bytesRead = m_pfnRead(this, &value, sizeof(T), &componentSize, 1);
    m_uiAbsoluteCurrentPos += bytesRead;
}

// Calls the base NiObject::LoadBinary, then reads a scalar from the stream into the basis data's field.
// @fnv 0x00a52c10
// pdb: NiBSplineBasisData::LoadBinary | NiBSplineBasisData.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiBSplineBasisData::LoadBinary(NiStream& kStream)
{
    NiObject::LoadBinary(kStream);
    kStream.m_pkIstr->ReadScalar(m_kBasisDegree3.m_iNumControlPoints);
}

// @fnv 0x00a52bb0
bool __fastcall IsBSplineBasisDataEqual(NiBSplineBasisData* pkThis, void*, NiObject* pkObject)
{
    if (!pkThis->NiObject::IsSameAs(pkObject))
        return false;

    return static_cast<NiBSplineBasisData*>(pkObject)->m_kBasisDegree3.m_iNumControlPoints ==
           pkThis->m_kBasisDegree3.m_iNumControlPoints;
}

extern int DAT_01099f24;
void __fastcall FUN_00a5d3d0(void* pThis);
void __cdecl FUN_00aa1460(void* pMem, unsigned int uiSize);

// @fnv 0x00a52be0
void* __fastcall FUN_00a52be0(void* pThis, int, unsigned char flags)
{
    *reinterpret_cast<int**>(pThis) = &DAT_01099f24;
    FUN_00a5d3d0(pThis);
    if (flags & 1)
    {
        FUN_00aa1460(pThis, 0x28);
    }
    return pThis;
}

struct GlobalObjectO2_00a7b610
{
    int x;
};
extern GlobalObjectO2_00a7b610 g_011f403c;
// @fnv 0x00a52af0
// src: COMMUNITY 0x00a52af0
GlobalObjectO2_00a7b610* NiBSplineBasisData_GetRTTI()
{
    return &g_011f403c;
}

class ActorCombatGroup;
namespace fnv_00a52c90 {
extern int g_011f403c;
void __stdcall AppendCameraPathEntries(void*);
ActorCombatGroup* __cdecl NiViewerStrings_NiGetViewerString_Title(int);
struct ArrX
{
    void SetSize(unsigned);
    void SetAt(unsigned, ActorCombatGroup*&);
    unsigned char pad[8];
    unsigned short m_usMaxSize;
    unsigned short m_usSize;
    unsigned short m_usESize;
    unsigned short m_usGrowBy;
};
// @fnv 0x00a52c90
// src: XBOX_PDB 0x00a52c90
// pdb: NiBSplineBasisData::GetViewerStrings | NiBSplineBasisData.obj | retail, upstream name already matches, high
// pdb params: pStrings
void __stdcall NiBSplineBasisData_GetViewerStrings(ArrX* pStrings)
{
    AppendCameraPathEntries(pStrings);
    ActorCombatGroup* pObject = NiViewerStrings_NiGetViewerString_Title(g_011f403c);
    unsigned n = pStrings->m_usSize;
    if (n >= pStrings->m_usMaxSize)
        pStrings->SetSize(n + pStrings->m_usGrowBy);
    pStrings->SetAt(n, pObject);
}
}  // namespace fnv_00a52c90

extern int DAT_01099d3c;

struct FUN_00a51ab0_base
{
    void NiBSplineInterpolator_ctor(int a1, int a3);
};

struct FUN_00a51ab0_cls : FUN_00a51ab0_base
{
    int m_vptr00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    float m_1c;
    float m_20;
    float m_24;
    float m_28;
    int m_2c;

    FUN_00a51ab0_cls* NiBSplineColorInterpolator_NiBSplineColorInterpolator(int pkData, int kColorACPHandle, int pkBasisData);
};

// Calls InitBase, sets the vtable pointer, zeroes four floats and stores a2 into m_2c, then returns this.
// @fnv 0x00a51ab0
// src: GAMEBRYO_LIB 0x00a51ab0
// pdb: NiBSplineColorInterpolator::NiBSplineColorInterpolator | NiBSplineColorInterpolator.obj | retail, upstream name already matches, high
// pdb params: pkData, kColorACPHandle, pkBasisData
FUN_00a51ab0_cls* FUN_00a51ab0_cls::NiBSplineColorInterpolator_NiBSplineColorInterpolator(int pkData, int kColorACPHandle, int pkBasisData)
{
    NiBSplineInterpolator_ctor(pkData, pkBasisData);
    m_vptr00 = (int)&DAT_01099d3c;
    *(volatile float*)&m_1c = 0.0f;
    m_20 = 0.0f;
    m_24 = 0.0f;
    m_28 = 0.0f;
    *(volatile int*)&m_2c = kColorACPHandle;
    return this;
}

void* __cdecl Object_DispatchCommandFour(unsigned int uiSize);

inline void* operator new(size_t, void* pMemory) throw()
{
    return pMemory;
}

// Construct in separately allocated storage so constructor failure has no delete cleanup.
inline NiBSplineBasisData* AllocateBSplineBasisData()
{
    void* pMemory = Object_DispatchCommandFour(sizeof(NiBSplineBasisData));
    if (pMemory)
    {
        NiBSplineBasisData* pResult = ::new (pMemory) NiBSplineBasisData;
        *(volatile float*)&pResult->m_kBasisDegree3.m_fLastTime = -1.0f;
        pResult->m_kBasisDegree3.m_iNumControlPoints = 0;
        pResult->m_kBasisDegree3.m_iMinControlPoint = 0;
        pResult->m_kBasisDegree3.m_iMaxControlPoint = 3;
        return pResult;
    }
    return 0;
}

// @fnv 0x00a52b00
NiBSplineBasisData* __fastcall CloneBSplineBasisData(NiBSplineBasisData* pkThis, void*, CloningProcess& cloning)
{
    NiBSplineBasisData* pResult = AllocateBSplineBasisData();
    pkThis->CreateVertexBufferSet(pResult, cloning);
    pResult->m_kBasisDegree3 = pkThis->m_kBasisDegree3;
    return pResult;
}

// @fnv 0x00a52b60
NiBSplineBasisData* CreateNiBSplineBasisData()
{
    return AllocateBSplineBasisData();
}

template <class T> inline void WriteScalar(NiBinaryStream* pkOstr, const T& value)
{
    unsigned int componentSize = sizeof(T);
    unsigned int bytesWritten = pkOstr->m_pfnWrite(pkOstr, &value, sizeof(T), &componentSize, 1);
    pkOstr->m_uiAbsoluteCurrentPos += bytesWritten;
}
// Saves the base NiObject data and the BSplineBasis control point count to the given NiStream.
// @fnv 0x00a52c50
// pdb: NiBSplineBasisData::SaveBinary | NiBSplineBasisData.obj | retail, object-file gap fill, high
// pdb params: kStream
void __fastcall SaveBSplineBasisData(NiBSplineBasisData* pkThis, void*, NiStream& stream)
{
    pkThis->NiObject::SaveBinary(stream);
    WriteScalar(stream.m_pkOstr, pkThis->m_kBasisDegree3.m_iNumControlPoints);
}

extern int DAT_0109a2a4;
void* NiMalloc(unsigned int size);

struct FUN_00a54190_cls
{
    int m_vt;
    void* m_04;
    unsigned short m_08;
    unsigned short m_0a;
    unsigned short m_0c;
    unsigned short m_0e;
    FUN_00a54190_cls* UpdateState_00a54190(int n, short a2);
};

// Sets object fields and allocates 4 times m_08 bytes when m_08 is positive.
// @fnv 0x00a54190
// src: GUESS 0x00a54190 (confidence: low)
// pdb: NiTPrimitiveArray<NiTPrimitiveSet<NiNode *> *>::`scalar deleting destructor' | NiBSBoneLODController.obj | debug, object-file run extension, low
FUN_00a54190_cls* FUN_00a54190_cls::UpdateState_00a54190(int n, short a2)
{
    m_vt = (int)&DAT_0109a2a4;
    m_08 = n;
    m_0e = a2;
    m_0a = 0;
    m_0c = 0;
    if (m_08 > 0)
    {
        unsigned int sz = m_08;
        m_04 = NiMalloc(sz * 4);
    }
    else
    {
        m_04 = 0;
    }
    return this;
}
