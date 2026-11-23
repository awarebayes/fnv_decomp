#include "../../../../include/BSTreadTransfInterpolator.h"
#include "../../../../include/MemoryManager.h"
#include "../../../../include/NiFixedString.h"
#include "../../../../include/NiGlobalStringTable.h"
#include "../../../../include/NiInterpolator.h"
#include "../../../../include/NiObject.h"
#include "../../../../include/NiQuatTransform.h"
#include "../../../../include/NiRefObject.h"
#include <string.h>

extern MemoryManager _g_memoryManager;

struct TreadTransformEntry
{
    NiQuatTransform first;
    NiQuatTransform second;
    NiFixedString name;
    TreadTransformEntry();
    static void* operator new[](size_t size)
    {
        return _g_memoryManager.Allocate(size);
    }
    static void operator delete[](void* p)
    {
        _g_memoryManager.Deallocate(p);
    }
};

// These inline methods are declared, but not defined, in the shared headers.
inline NiFixedString& NiFixedString::operator=(const NiFixedString& other)
{
    if (m_kHandle != other.m_kHandle)
    {
        GlobalStringHandle next = other.m_kHandle;
        NiGlobalStringTable::IncRefCount(next);
        NiGlobalStringTable::DecRefCount(m_kHandle);
        m_kHandle = next;
    }
    return *this;
}
inline void NiRefObject::DecRefCount()
{
    if (InterlockedDecrement(reinterpret_cast<volatile long*>(&m_uiRefCount)) == 0)
        DeleteThis();
}
template <class T> inline NiPointer<T>& NiPointer<T>::operator=(const NiPointer& kSrc)
{
    if (m_pObject != kSrc.m_pObject)
    {
        if (m_pObject != 0)
            m_pObject->DecRefCount();
        m_pObject = kSrc.m_pObject;
        if (m_pObject)
            InterlockedIncrement(reinterpret_cast<volatile long*>(&m_pObject->m_uiRefCount));
    }
    return *this;
}
// The base copy helper has no declaration in the shared NiInterpolator header.
extern "C" void NiKeyBasedInterpolator_CopyMembers();

// @fnv 0x00a43690
// pdb: BSTreadTransfInterpolator::CopyMembers | BSTreadTransfInterpolator.obj | retail, upstream name already matches, high
// pdb params: pkDest, kCloning
// pdb locals: ui
void BSTreadTransfInterpolator::CopyMembers(BSTreadTransfInterpolator* pkDest, void* kCloning)
{
    reinterpret_cast<void (__thiscall *)(NiInterpolator*, NiInterpolator*, void*)>(NiKeyBasedInterpolator_CopyMembers)(this, pkDest, kCloning);
    delete[] pkDest->Links;
    pkDest->uiNumLinks = uiNumLinks;
    pkDest->Links = new TreadTransformEntry[uiNumLinks];
    for (unsigned int i = 0; i < uiNumLinks; i++)
    {
        pkDest->Links[i].name = Links[i].name;
        memcpy(&pkDest->Links[i].first, &Links[i].first, sizeof(NiQuatTransform));
        memcpy(&pkDest->Links[i].second, &Links[i].second, sizeof(NiQuatTransform));
    }
    pkDest->spLerpData = spLerpData;
    pkDest->fCurrLerp = fCurrLerp;
    pkDest->fDiffLerp = fDiffLerp;
}
