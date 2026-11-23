#pragma once

#include "hkDataArrayImpl.h"

class NativeDBC;

// pdb: class hkDataArrayNative, size 0x2c, bases: hkDataArrayImpl, 8 members
class hkDataArrayNative : public hkDataArrayImpl
{
public:
    hkDataArrayNative(int world, void* address, int arraySize, int elementStride, int stype, int klass, int cArrayCount) throw();
    void* operator new(size_t nbytes);

    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual void vf05();
    virtual void vf06();
    virtual hkDataArrayNative* swizzleObjectMember(int name);
    virtual int asVec(int a);
    virtual void vf09();
    virtual int asString(int index);
    virtual void vf11();
    virtual float asReal(int index);
    virtual void vf13();
    virtual void vf14();
    virtual void vf15();
    virtual int GetElementAsInt(int index);
    virtual void vf17();
    virtual __int64 GetEntry64(int a);
    virtual void vf19();
    virtual NativeDBC* asObject(int index);
    virtual void vf21();
    virtual hkDataArrayNative* GetElement(int index);
    virtual void vf23();
    virtual void vf24();
    virtual void vf25();
    virtual void vf26();
    virtual void vf27();
    virtual void vf28();
    virtual void vf29();
    virtual void vf30();
    virtual void vf31();
    virtual void vf32();
    virtual void vf33();
    virtual void vf34();
    virtual void vf35();

    int m_00;
    int m_04;
    int m_world; // src: XBOX_RETAIL_PDB +0xc (old: m_08)
    int m_address; // src: XBOX_RETAIL_PDB +0x10 (old: m_0C)
    int m_arraySize; // src: XBOX_RETAIL_PDB +0x14 (old: m_10)
    int m_elementStride; // src: XBOX_RETAIL_PDB +0x18 (old: m_14)
    int m_nativeType; // src: XBOX_RETAIL_PDB +0x1c (old: m_18)
    int m_class; // src: XBOX_RETAIL_PDB +0x20 (old: m_1c)
    int m_dataType; // src: XBOX_RETAIL_PDB +0x24 (old: m_24)
    int m_tupleCount; // src: XBOX_RETAIL_PDB +0x28 (old: m_28)
};
