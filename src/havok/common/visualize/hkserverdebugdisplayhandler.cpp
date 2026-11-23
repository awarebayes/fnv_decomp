#include "../../../../include/hkArray.h"

// src/havok/common/visualize/hkserverdebugdisplayhandler.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/projects/fallout/code/havok/source/common/visualize/hkserverdebugdisplayhandler.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#define FNV_VEC4_SIMD
#include "../../../../include/Vec4.h"

extern int DAT_010d7cfc;
void __cdecl _Deallocate(void*);
struct FUN_00dd97c0
{
    int* m_p;
    FUN_00dd97c0(char f);
};
// @fnv 0x00dd97c0
// pdb: hkDebugDisplayHandler::`scalar deleting destructor' | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
FUN_00dd97c0::FUN_00dd97c0(char acFlags)
{
    m_p = &DAT_010d7cfc;
    if (acFlags & 1)
        _Deallocate(this);
}

extern "C" __declspec(dllimport) void __stdcall _fnv_EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall _fnv_LeaveCriticalSection(void* section);
struct FUN_00d605d0_Class
{
    void hkOArchive_write32u(unsigned int u);
    void hkOArchive_write8u(unsigned int value);
    void hkDisplaySerializeOStream_writeQuadVector4(unsigned int value);
    void hkOArchive_WriteBinaryData_Thunk_00d609e0(__int64 value);
    void WriteInt(unsigned int value);
    const char* ForwardValueToObject(unsigned int* value);
};
struct Record
{
    int unk00;
    FUN_00d605d0_Class* config;
    void* criticalSection;
    bool SetConfigValues(unsigned int start, unsigned int end, unsigned int color, int id, unsigned int tag);
};
// Locks the critical section, writes five values to config, reads back a string, and returns whether the read-back string is empty.
// @fnv 0x00dd9b40
// pdb: hkServerDebugDisplayHandler::displayLine | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: start, end, color, id, tag
bool Record::SetConfigValues(unsigned int start, unsigned int end, unsigned int color, int id,
                             unsigned int tag)
{
    _fnv_EnterCriticalSection(criticalSection);
    if (config)
    {
        config->hkOArchive_write32u(0x29);
        config->hkOArchive_write8u(8);
        config->hkDisplaySerializeOStream_writeQuadVector4(start);
        config->hkDisplaySerializeOStream_writeQuadVector4(end);
        config->hkOArchive_write32u(color);
        config->hkOArchive_WriteBinaryData_Thunk_00d609e0(id);
        config->WriteInt(tag);
    }
    bool hasValue;
    if (config)
    {
        const char* value = config->ForwardValueToObject(&start);
        if (value[0] == 0)
            goto noValue;
        hasValue = true;
        goto valueChecked;
    noValue:
        hasValue = false;
    valueChecked:;
    }
    else
        hasValue = false;
    _fnv_LeaveCriticalSection(criticalSection);
    return !hasValue;
}

struct FUN_00dd9ab0_cls
{
    int unk00;
    FUN_00d605d0_Class* m_pConfiguration;
    void* m_pWorldLock;
    bool hkServerDebugDisplayHandler_displayPoint(unsigned int position, unsigned int color, int id, unsigned int tag);
};

// @fnv 0x00dd9ab0
// src: HAVOK_LIB 0x00dd9ab0
// pdb: hkServerDebugDisplayHandler::displayPoint | hkServerDebugDisplayHandler.obj | retail, upstream name already matches, high
// pdb params: position, color, id, tag
bool FUN_00dd9ab0_cls::hkServerDebugDisplayHandler_displayPoint(unsigned int position, unsigned int color, int id, unsigned int tag)
{
    bool ok;
    _fnv_EnterCriticalSection(m_pWorldLock);
    if (m_pConfiguration != 0)
    {
        m_pConfiguration->hkOArchive_write32u(0x1d);
        m_pConfiguration->hkOArchive_write8u(7);
        m_pConfiguration->hkDisplaySerializeOStream_writeQuadVector4(position);
        m_pConfiguration->hkOArchive_write32u(color);
        m_pConfiguration->hkOArchive_WriteBinaryData_Thunk_00d609e0(id);
        m_pConfiguration->WriteInt(tag);
    }
    if (m_pConfiguration == 0)
    {
        goto not_found;
    }
    {
        const char* s = m_pConfiguration->ForwardValueToObject(&position);
        if (*s != 0)
        {
            ok = true;
            goto done;
        }
    }
not_found:
    ok = false;
done:
    _fnv_LeaveCriticalSection(m_pWorldLock);
    return !ok;
}

struct ConfigExt
{
    void unk607b0(unsigned int a, unsigned int b);
};

struct FUN_00dd9be0_cls
{
    int unk00;
    FUN_00d605d0_Class* m_pConfiguration;
    void* m_cs;
    bool hkServerDebugDisplayHandler_sendMemStatsDump(unsigned int data, unsigned int length);
};

// @fnv 0x00dd9be0
// src: HAVOK_LIB 0x00dd9be0
// pdb: hkServerDebugDisplayHandler::sendMemStatsDump | hkServerDebugDisplayHandler.obj | retail, upstream name already matches, high
// pdb params: data, length
bool FUN_00dd9be0_cls::hkServerDebugDisplayHandler_sendMemStatsDump(unsigned int data, unsigned int length)
{
    _fnv_EnterCriticalSection(m_cs);
    if (m_pConfiguration != 0)
    {
        unsigned int v = length;
        m_pConfiguration->hkOArchive_write32u(v + 5);
        m_pConfiguration->hkOArchive_write8u(0xf);
        m_pConfiguration->WriteInt(v);
        ((ConfigExt*)m_pConfiguration)->unk607b0(data, v);
    }
    bool result;
    if (m_pConfiguration != 0)
    {
        const char* s = m_pConfiguration->ForwardValueToObject(&length);
        if (*s == 0)
        {
            goto zero;
        }
        result = true;
        goto done;
    }
zero:
    result = false;
done:
    _fnv_LeaveCriticalSection(m_cs);
    return !result;
}

struct S_00dd9c60
{
    int unk00;
    FUN_00d605d0_Class* unk04;
    void* unk08;
    bool hkServerDebugDisplayHandler_holdImmediate();
};
// @fnv 0x00dd9c60
// src: HAVOK_LIB 0x00dd9c60
// pdb: hkServerDebugDisplayHandler::holdImmediate | hkServerDebugDisplayHandler.obj | retail, upstream name already matches, high
bool S_00dd9c60::hkServerDebugDisplayHandler_holdImmediate()
{
    _fnv_EnterCriticalSection(unk08);
    if (unk04)
    {
        unk04->hkOArchive_write32u(1);
        unk04->hkOArchive_write8u(0xb);
    }
    bool r;
    char c;
    if (unk04 && *unk04->ForwardValueToObject((unsigned int*)&c) != 0)
        r = true;
    else
        r = false;
    _fnv_LeaveCriticalSection(unk08);
    return !r;
}

int __cdecl hkDisplaySerializeOStream_computeBytesRequired_00dda850(const char* str);

typedef hkArray<const char*> DD9D70StringArray;

// @fnv 0x00dd9d70
// src: HAVOK_LIB 0x00dd9d70
// pdb: _getGeometryByteSize | hkServerDebugDisplayHandler.obj | retail, upstream name already matches, high
// pdb params: geometries
static __declspec(noinline) int _getGeometryByteSize(const DD9D70StringArray& geometries)
{
    int size = 4;
    for (int i = 0; i < geometries.m_iElementCount; i++)
    {
        const char* const& s = geometries.m_data[i];
        size += hkDisplaySerializeOStream_computeBytesRequired_00dda850(s);
    }
    return size;
}

// Caller so the file-static helper is emitted with its register calling convention.
int DD9D70ComputeSize(const DD9D70StringArray* a, const DD9D70StringArray* b)
{
    return _getGeometryByteSize(*a) + _getGeometryByteSize(*b);
}

extern "C" __declspec(dllimport) void __stdcall _fnv_EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall _fnv_LeaveCriticalSection(void* section);

class ConfigStub
{
  public:
    void WriteShape(unsigned int value);
};

struct ItemList
{
    unsigned int* m_pItems;
    int m_iArrayLength;
};

class FUN_00dd9da0_cls
{
  public:
    int unk00;
    int unk04;
    int unk08;
    FUN_00d605d0_Class* m_pConfig;
    void* m_pUserData;

    void hkServerDebugDisplayHandler_sendGeometryData(ItemList* geometries);
};

// @fnv 0x00dd9da0
// src: XBOX_RETAIL_PDB 0x00dd9da0
// pdb: hkServerDebugDisplayHandler::sendGeometryData | hkServerDebugDisplayHandler.obj | retail, call graph, high
// pdb params: geometries
void FUN_00dd9da0_cls::hkServerDebugDisplayHandler_sendGeometryData(ItemList* geometries)
{
    int i;

    _fnv_EnterCriticalSection(m_pUserData);
    m_pConfig->WriteInt(geometries->m_iArrayLength);
    for (i = 0; i < geometries->m_iArrayLength; i++)
    {
        ((ConfigStub*)m_pConfig)->WriteShape(geometries->m_pItems[i]);
    }
    _fnv_LeaveCriticalSection(m_pUserData);
}

extern int DAT_010d7cfc;
void __cdecl _Deallocate(void*);

extern "C" __declspec(dllimport) void __stdcall _fnv_EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall _fnv_LeaveCriticalSection(void* section);

struct ParentRec
{
    void hkServerDebugDisplayHandler_sendGeometryData(unsigned int geometries);
};

struct ConfigAux
{
    void hkDisplaySerializeOStream_writeTransform(unsigned int value);
};

unsigned int _getGeometryByteSize();

struct FUN_00dd9df0_cls
{
    int unk00;
    FUN_00d605d0_Class* config;
    void* criticalSection;
    bool hkServerDebugDisplayHandler_addGeometry(unsigned int geometries, unsigned int transform, unsigned int id, unsigned int tag, unsigned int shapeIdHint);
};

// Locks the critical section, applies the given values to config and parent, then returns whether the config's check fails.
// @fnv 0x00dd9df0
// src: HAVOK_LIB 0x00dd9df0
// pdb: hkServerDebugDisplayHandler::addGeometry | hkServerDebugDisplayHandler.obj | retail, upstream name already matches, high
// pdb params: geometries, transform, id, tag, shapeIdHint
bool FUN_00dd9df0_cls::hkServerDebugDisplayHandler_addGeometry(unsigned int geometries, unsigned int transform, unsigned int id, unsigned int tag, unsigned int shapeIdHint)
{
    _fnv_EnterCriticalSection(this->criticalSection);
    if (this->config != 0)
    {
        unsigned int uiValue1Copy = geometries;
        unsigned int v = _getGeometryByteSize();
        this->config->WriteInt(v + 0x29);
        this->config->hkOArchive_write8u(1);
        ((ParentRec*)((char*)this - 8))->hkServerDebugDisplayHandler_sendGeometryData(uiValue1Copy);
        ((ConfigAux*)this->config)->hkDisplaySerializeOStream_writeTransform(transform);
        this->config->hkOArchive_WriteBinaryData_Thunk_00d609e0(id);
        this->config->WriteInt(tag);
    }
    bool ok;
    if (this->config == 0)
    {
        goto notok;
    }
    if (*this->config->ForwardValueToObject(&geometries) != 0)
    {
        ok = true;
        goto done;
    }
notok:
    ok = false;
done:
    _fnv_LeaveCriticalSection(this->criticalSection);
    return !ok;
}

unsigned int _getGeometryByteSize();

struct FUN_00dd9da0_ext
{
    void hkServerDebugDisplayHandler_sendGeometryData(unsigned int geometries);
};

struct FUN_00dd9f40_cls
{
    int unk00;
    FUN_00d605d0_Class* m_pConfiguration;
    void* m_pWorldLock;
    bool SetAndCheck(unsigned int geometries, unsigned int color, int id, unsigned int tag);
};

// @fnv 0x00dd9f40
// pdb: hkServerDebugDisplayHandler::displayGeometry | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: geometries, transform, color, id, tag
bool FUN_00dd9f40_cls::SetAndCheck(unsigned int geometries, unsigned int color, int id, unsigned int tag)
{
    bool ok;
    _fnv_EnterCriticalSection(m_pWorldLock);
    if (m_pConfiguration != 0)
    {
        unsigned int v = geometries;
        m_pConfiguration->WriteInt(_getGeometryByteSize() + 0x11);
        m_pConfiguration->hkOArchive_write8u(6);
        reinterpret_cast<FUN_00dd9da0_ext*>((char*)this - 8)->hkServerDebugDisplayHandler_sendGeometryData(v);
        m_pConfiguration->hkOArchive_write32u(color);
        m_pConfiguration->hkOArchive_WriteBinaryData_Thunk_00d609e0((__int64)id);
        m_pConfiguration->WriteInt(tag);
    }
    if (m_pConfiguration == 0)
    {
        goto not_found;
    }
    {
        const char* s = m_pConfiguration->ForwardValueToObject(&geometries);
        if (*s != 0)
        {
            ok = true;
            goto done;
        }
    }
not_found:
    ok = false;
done:
    _fnv_LeaveCriticalSection(m_pWorldLock);
    return !ok;
}

struct FUN_00d605d0_ext
{
    void hkOArchive_writeRaw(unsigned int a, unsigned int len);
    void WriteUInt16(unsigned int len);
};

int hkString_strLen(unsigned int value);

struct FUN_00dda0e0_cls
{
    int unk00;
    FUN_00d605d0_Class* config;
    void* criticalSection;
    int hkServerDebugDisplayHandler_displayText(unsigned int text, unsigned int color, int id, unsigned int tag);
};

// @fnv 0x00dda0e0
// src: HAVOK_LIB 0x00dda0e0
// pdb: hkServerDebugDisplayHandler::displayText | hkServerDebugDisplayHandler.obj | retail, upstream name already matches, high
// pdb params: text, color, id, tag
int FUN_00dda0e0_cls::hkServerDebugDisplayHandler_displayText(volatile unsigned int text, unsigned int color, int id, unsigned int tag)
{
    _fnv_EnterCriticalSection(criticalSection);
    if (config != 0)
    {
        unsigned int v = text;
        int len = hkString_strLen(v);
        int n = (len < 0xffff) ? len : 0xffff;
        config->hkOArchive_write32u(n + 0x13);
        config->hkOArchive_write8u(9);
        reinterpret_cast<FUN_00d605d0_ext*>(config)->WriteUInt16(n);
        reinterpret_cast<FUN_00d605d0_ext*>(config)->hkOArchive_writeRaw(v, n);
        config->WriteInt(color);
        config->hkOArchive_WriteBinaryData_Thunk_00d609e0((__int64)id);
        config->WriteInt(tag);
    }
    bool ok = (config != 0) && (config->ForwardValueToObject((unsigned int*)&text)[0] != 0);
    _fnv_LeaveCriticalSection(criticalSection);
    return !ok;
}

int __cdecl hkString_strLen(const char* text);

struct FUN_00d605d0_ext_cls
{
    void WriteUInt16(unsigned int value);
    void hkOArchive_writeRaw(const char* text, unsigned int length);
    void HashUInt(float value);
};

struct FUN_00dda1a0_cls
{
    int unk00;
    FUN_00d605d0_Class* config;
    void* criticalSection;

    bool WriteConfigEntry(const char* name, const float* pos, unsigned int color, int b, unsigned int tag);
};

// @fnv 0x00dda1a0
// pdb: hkServerDebugDisplayHandler::display3dText | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: text, pos, color, id, tag
bool FUN_00dda1a0_cls::WriteConfigEntry(const char* name, const float* pos, unsigned int color, int iValueB,
                                        unsigned int tag)
{
    _fnv_EnterCriticalSection(criticalSection);
    if (config != 0)
    {
        int n = hkString_strLen(name);
        int length = (n < 0xffff) ? n : 0xffff;
        config->hkOArchive_write32u(length + 0x1f);
        config->hkOArchive_write8u(0x11);
        reinterpret_cast<FUN_00d605d0_ext_cls*>(config)->WriteUInt16(length);
        reinterpret_cast<FUN_00d605d0_ext_cls*>(config)->hkOArchive_writeRaw(name, length);
        config->WriteInt(color);
        config->hkOArchive_WriteBinaryData_Thunk_00d609e0((__int64)iValueB);
        config->WriteInt(tag);
        reinterpret_cast<FUN_00d605d0_ext_cls*>(config)->HashUInt(pos[0]);
        reinterpret_cast<FUN_00d605d0_ext_cls*>(config)->HashUInt(pos[1]);
        reinterpret_cast<FUN_00d605d0_ext_cls*>(config)->HashUInt(pos[2]);
    }
    bool result;
    if (config == 0)
    {
        goto done;
    }
    {
        unsigned int id;
        const char* text = config->ForwardValueToObject(&id);
        if (text[0] != 0)
        {
            result = true;
            goto out;
        }
    }
done:
    result = false;
out:
    _fnv_LeaveCriticalSection(criticalSection);
    return !result;
}

struct FUN_00dda290_ext
{
    void hkOArchive_writeRaw(unsigned int a, unsigned int len);
    void WriteUInt16(unsigned int len);
};

int hkString_strLen(unsigned int value);

struct FUN_00dda290_cls
{
    int unk00;
    FUN_00d605d0_Class* config;
    void* criticalSection;
    int WriteConfigValue(unsigned int text, int id, unsigned int tag);
};

// @fnv 0x00dda290
// pdb: hkServerDebugDisplayHandler::displayAnnotation | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: text, id, tag
int FUN_00dda290_cls::WriteConfigValue(volatile unsigned int text, int id, unsigned int tag)
{
    _fnv_EnterCriticalSection(criticalSection);
    if (config != 0)
    {
        unsigned int v = text;
        int len = hkString_strLen(v);
        int n = (len < 0xffff) ? len : 0xffff;
        config->hkOArchive_write32u(n + 0xf);
        config->hkOArchive_write8u(0x14);
        reinterpret_cast<FUN_00dda290_ext*>(config)->WriteUInt16(n);
        reinterpret_cast<FUN_00dda290_ext*>(config)->hkOArchive_writeRaw(v, n);
        config->hkOArchive_WriteBinaryData_Thunk_00d609e0((__int64)id);
        config->WriteInt(tag);
    }
    bool ok = (config != 0) && (config->ForwardValueToObject((unsigned int*)&text)[0] != 0);
    _fnv_LeaveCriticalSection(criticalSection);
    return !ok;
}

#include <xmmintrin.h>
#include <windows.h>
struct DD97E0Vector4
{
    __m128 m_vComponents;
};

struct __declspec(align(16)) DD97E0Point
{
    float x;
    float y;
    float z;
    float w;
};

struct DD97E0Bool
{
    char m_cBooleanValue;

    operator bool() const
    {
        return m_cBooleanValue != 0;
    }
};

struct DD97E0OStream
{
    void writeCommand(unsigned int value);
    void writeCount(unsigned int value);
    void writeVector(const DD97E0Point& v);
    void writeUint64(unsigned __int64 value);
    void writeTag(int value);
    DD97E0Bool IsStreamValid();
};

struct DD97E0CriticalSection
{
    CRITICAL_SECTION m_kCriticalSection;

    void enter()
    {
        EnterCriticalSection(&m_kCriticalSection);
    }
    void leave()
    {
        LeaveCriticalSection(&m_kCriticalSection);
    }
};

struct DD97E0DisplayHandler
{
    int unk00;
    DD97E0OStream* m_pDisplayStream;
    DD97E0CriticalSection* m_pSynchronizationLock;

    int hkServerDebugDisplayHandler_addGeometryInstance(int originalInstanceID, int transform, int id, int tag, int shapeIdHint);
};

// Locks m_pSynchronizationLock, checks m_pDisplayStream and its IsStreamValid, then returns 0 if valid or 1 otherwise.
// @fnv 0x00dd97e0
// src: HAVOK_LIB 0x00dd97e0
// pdb: hkServerDebugDisplayHandler::addGeometryInstance | hkServerDebugDisplayHandler.obj | retail, upstream name already matches, high
// pdb params: originalInstanceID, transform, id, tag, shapeIdHint
int DD97E0DisplayHandler::hkServerDebugDisplayHandler_addGeometryInstance(int originalInstanceID, int transform, int id, int tag, int shapeIdHint)
{
    m_pSynchronizationLock->enter();
    bool ok = m_pDisplayStream && m_pDisplayStream->IsStreamValid();
    m_pSynchronizationLock->leave();
    return ok ? 0 : 1;
}

struct FUN_00d60940_Class
{
    void WriteInt(unsigned int);
};
struct Stream : FUN_00d605d0_Class
{
    void hkOArchive_write32u(unsigned int);
    void hkOArchive_WriteBinaryData_Thunk_00d609e0(unsigned int, int);
    char* ForwardValueToObject(void*);
};
struct Output
{
    int state;
    Stream* stream;
    CRITICAL_SECTION* lock;
    int WriteRecord(unsigned int, unsigned int, unsigned int);
};
// Under the lock, writes command 17, count 3, and the three arguments, then returns the negation of the forwarded value's first byte.
// @fnv 0x00dd9830
// pdb: hkServerDebugDisplayHandler::setGeometryColor | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: color, id, tag
int Output::WriteRecord(unsigned int color, unsigned int id, unsigned int tag)
{
    EnterCriticalSection(lock);
    if (stream)
    {
        stream->hkOArchive_write32u(17);
        stream->hkOArchive_write8u(3);
        stream->hkOArchive_write32u(color);
        stream->hkOArchive_WriteBinaryData_Thunk_00d609e0(id, 0);
        ((FUN_00d60940_Class*)stream)->WriteInt(tag);
    }
    bool failed = stream && *stream->ForwardValueToObject(&color);
    LeaveCriticalSection(lock);
    return !failed;
}

enum S_00dd98c0_Result
{
    S_00dd98c0_SUCCESS = 0,
    S_00dd98c0_FAILURE = 1
};
struct S_00dd98c0_Bool
{
    char m_cBooleanValue;
    S_00dd98c0_Bool()
    {}
};
struct S_00dd98c0_Target
{
    void hkOArchive_write32u(int u);
    void hkOArchive_write8u(int a);
    void hkDisplaySerializeOStream_writeTransform(int a);
    void hkOArchive_WriteBinaryData_Thunk_00d609e0(int a, int b);
    void WriteInt(int a);
    S_00dd98c0_Bool ForwardValueToObject();
};
struct S_00dd98c0
{
    int unk0;
    S_00dd98c0_Target* unk4;
    CRITICAL_SECTION* unk8;
    S_00dd98c0_Result WriteValue(int transform, int id, int tag);
};
// Writes a1, a2, a3 to unk4 under unk8, then returns SUCCESS if unk4 and ForwardValueToObject() are true, else FAILURE.
// @fnv 0x00dd98c0
// pdb: hkServerDebugDisplayHandler::updateGeometry | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: transform, id, tag
S_00dd98c0_Result S_00dd98c0::WriteValue(int transform, int id, int tag)
{
    EnterCriticalSection(unk8);
    if (unk4)
    {
        unk4->hkOArchive_write32u(0x29);
        unk4->hkOArchive_write8u(2);
        unk4->hkDisplaySerializeOStream_writeTransform(transform);
        unk4->hkOArchive_WriteBinaryData_Thunk_00d609e0(id, 0);
        unk4->WriteInt(tag);
    }
    bool ok = unk4 && unk4->ForwardValueToObject().m_cBooleanValue;
    LeaveCriticalSection(unk8);
    return ok ? S_00dd98c0_SUCCESS : S_00dd98c0_FAILURE;
}

struct DD9950Bool
{
    char m_cBooleanValue;

    DD9950Bool(bool b)
    {
        m_cBooleanValue = (char)b;
    }
    operator bool() const
    {
        return m_cBooleanValue != 0;
    }
};

enum DD9950Result
{
    DD9950_SUCCESS = 0,
    DD9950_FAILURE = 1
};

struct DD9950CriticalSection
{
    CRITICAL_SECTION m_kCriticalSection;

    void enter()
    {
        EnterCriticalSection(&m_kCriticalSection);
    }
    void leave()
    {
        LeaveCriticalSection(&m_kCriticalSection);
    }
};

struct DD9950OArchive
{
    void WriteUint8(unsigned char u);
    void WriteUint32(unsigned int v);
    void WriteUint64(unsigned __int64 v);
    void WriteInt32(int v);
    DD9950Bool IsStreamValid() const;
};

struct DD9950DisplayServer
{
    int unk00;
    DD9950OArchive* m_pDisplayStream;
    DD9950CriticalSection* m_outStreamLock;

    DD9950Result hkServerDebugDisplayHandler_removeGeometry(unsigned long id, int tag, unsigned long shapeIdHint);
};

// @fnv 0x00dd9950
// src: HAVOK_LIB 0x00dd9950
// pdb: hkServerDebugDisplayHandler::removeGeometry | hkServerDebugDisplayHandler.obj | retail, upstream name already matches, high
// pdb params: id, tag, shapeIdHint
DD9950Result DD9950DisplayServer::hkServerDebugDisplayHandler_removeGeometry(unsigned long id, int tag, unsigned long shapeIdHint)
{
    m_outStreamLock->enter();
    if (m_pDisplayStream)
    {
        m_pDisplayStream->WriteUint8(0xd);
        m_pDisplayStream->WriteUint32(4);
        m_pDisplayStream->WriteUint64(id);
        m_pDisplayStream->WriteInt32(tag);
    }
    bool streamOk = m_pDisplayStream && m_pDisplayStream->IsStreamValid();
    m_outStreamLock->leave();
    return streamOk ? DD9950_SUCCESS : DD9950_FAILURE;
}

namespace fnv_00dd99d0 {
struct DD99D0Bool
{
    char m_cBooleanValue;

    operator bool() const
    {
        return m_cBooleanValue != 0;
    }
};

struct DD99D0OStream
{
    void writeUint32(unsigned int u);
    void writeCommand(unsigned int value);
    void writeVector(const void* v);
    void writeFloat(float value);
    void writeInt32(int value);
    void WriteByteArray(const char* data, int length);
    DD99D0Bool IsStreamValid();
};

struct DD99D0CriticalSection
{
    CRITICAL_SECTION m_kCriticalSection;

    void enter()
    {
        EnterCriticalSection(&m_kCriticalSection);
    }
    void leave()
    {
        LeaveCriticalSection(&m_kCriticalSection);
    }
};

extern "C" int __cdecl hkString_strLen(const char* str);

struct DD99D0DisplayHandler
{
    int unk00;
    DD99D0OStream* m_pDisplayStream;
    DD99D0CriticalSection* m_pSynchronizationLock;

    int WriteDisplayCommand(const void* from, const void* to, const void* up, float nearPlane, float farPlane, float fov, const char* name);
};

// Writes a display command with three vectors, three floats, and a text string to m_pDisplayStream under m_pSynchronizationLock.
// @fnv 0x00dd99d0
// pdb: hkServerDebugDisplayHandler::updateCamera | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: from, to, up, nearPlane, farPlane, fov, name
int DD99D0DisplayHandler::WriteDisplayCommand(const void* from, const void* to, const void* up,
                                              float nearPlane, float farPlane, float fov, const char* name)
{
    m_pSynchronizationLock->enter();
    if (m_pDisplayStream)
    {
        int length = hkString_strLen(name);
        m_pDisplayStream->writeUint32(length + 0x33);
        m_pDisplayStream->writeCommand(0xd);
        m_pDisplayStream->writeVector(from);
        m_pDisplayStream->writeVector(to);
        m_pDisplayStream->writeVector(up);
        m_pDisplayStream->writeFloat(nearPlane);
        m_pDisplayStream->writeFloat(farPlane);
        m_pDisplayStream->writeFloat(fov);
        m_pDisplayStream->writeInt32(length);
        m_pDisplayStream->WriteByteArray(name, length);
    }
    bool ok = m_pDisplayStream && m_pDisplayStream->IsStreamValid();
    m_pSynchronizationLock->leave();
    return ok ? 0 : 1;
}
}  // namespace fnv_00dd99d0

struct DD9FE0Vector4
{
    __m128 m_vComponents;
};

struct __declspec(align(16)) DD9FE0Point
{
    float x;
    float y;
    float z;
    float w;
};

struct DD9FE0Bool
{
    char m_cBooleanValue;

    operator bool() const
    {
        return m_cBooleanValue != 0;
    }
};

struct DD9FE0OStream
{
    void writeCommand(unsigned int u);
    void writeCount(unsigned int value);
    void writeVector(const DD9FE0Point& v);
    void writeUint64(unsigned __int64 value);
    void writeTag(int value);
    DD9FE0Bool IsStreamValid();
};

struct DD9FE0CriticalSection
{
    CRITICAL_SECTION m_kCriticalSection;

    void enter()
    {
        EnterCriticalSection(&m_kCriticalSection);
    }
    void leave()
    {
        LeaveCriticalSection(&m_kCriticalSection);
    }
};

struct DD9FE0DisplayHandler
{
    int unk00;
    DD9FE0OStream* m_pDisplayStream;
    DD9FE0CriticalSection* m_pSynchronizationLock;

    int WriteTriangle(const DD9FE0Vector4& a, const DD9FE0Vector4& b, const DD9FE0Vector4& c, unsigned int color,
                      int id, int tag);
};

// Writes three line segments forming a triangle to the display stream under a critical section, returning 0 on success or 1 if the stream is missing or invalid.
// @fnv 0x00dd9fe0
// pdb: hkServerDebugDisplayHandler::displayTriangle | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: a, b, c, color, id, tag
// pdb locals: p
int DD9FE0DisplayHandler::WriteTriangle(const DD9FE0Vector4& a, const DD9FE0Vector4& b, const DD9FE0Vector4& c,
                                        unsigned int color, int id, int tag)
{
    m_pSynchronizationLock->enter();
    if (m_pDisplayStream)
    {
        DD9FE0Point verts[4];
        *(__m128*)&verts[0] = a.m_vComponents;
        *(__m128*)&verts[1] = b.m_vComponents;
        *(__m128*)&verts[2] = c.m_vComponents;
        *(__m128*)&verts[3] = a.m_vComponents;
        unsigned __int64 id64 = id;
        for (int i = 0; i < 3; i++)
        {
            m_pDisplayStream->writeCommand(0x29);
            m_pDisplayStream->writeCount(8);
            m_pDisplayStream->writeVector(verts[i]);
            m_pDisplayStream->writeVector(verts[i + 1]);
            m_pDisplayStream->writeCommand(color);
            m_pDisplayStream->writeUint64(id64);
            m_pDisplayStream->writeTag(tag);
        }
    }
    bool ok = m_pDisplayStream && m_pDisplayStream->IsStreamValid();
    m_pSynchronizationLock->leave();
    return ok ? 0 : 1;
}

struct __declspec(align(16)) Transform_dda340
{
    Vec4 rotation0;
    Vec4 rotation1;
    Vec4 rotation2;
    Vec4 translation;
};

struct QsTransform_dda340
{
    Vec4 translation;
    Vec4 rotation;
    Vec4 scale;

    void Object_ForwardCall_00ce5300(Transform_dda340* out);
};

struct MemoryAllocator_dda340
{
    virtual void* vfn0() = 0;
    virtual void* vfn1() = 0;
    virtual void* vfn2() = 0;
    virtual void* vfn3() = 0;
    virtual void ReleaseBlock(void* p, int numBytes) = 0;
};

struct ThreadMemory_dda340
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    MemoryAllocator_dda340* m_pMemoryAllocator;
};

ThreadMemory_dda340* hkMemoryRouter_getInstance();
void __cdecl ResizeAudioStream(void* mem, void* array, int sizeElem);



class Array_dda340LocalForm
{ // local form of hkArray: 0x00dda340 needs the original inline/out-of-line helper split; shared replacement changes bytes.
  public:
    int* m_data;
    int m_iElementCount;
    int m_iCapacityState;

    Array_dda340LocalForm() : m_data(0), m_iElementCount(0), m_iCapacityState(0x80000000)
    {}

    void ClearAndRelease()
    {
        ThreadMemory_dda340* mem = hkMemoryRouter_getInstance();
        MemoryAllocator_dda340* alloc = mem->m_pMemoryAllocator;
        int cap = m_iCapacityState;
        m_iElementCount = 0;
        if ((cap & 0x80000000) == 0)
        {
            alloc->ReleaseBlock(m_data, (cap & 0x3fffffff) * sizeof(int));
        }
    }

    void AddBack(int val)
    {
        ThreadMemory_dda340* mem = hkMemoryRouter_getInstance();
        if (m_iElementCount == (m_iCapacityState & 0x3fffffff))
        {
            ResizeAudioStream(mem->m_pMemoryAllocator, this, sizeof(int));
        }
        m_data[m_iElementCount++] = val;
    }
};

struct Config_dda340 : public FUN_00d605d0_Class
{
    void WriteUInt16(unsigned int value);
    void hkOArchive_writeRaw(const char* text, unsigned int length);
    void hkDisplaySerializeOStream_writeTransform(const Transform_dda340* t);
};

// 0x00dda340 serializes behavior arrays via the display stream at +4 under the lock at +8.
struct BehaviorGraphDisplaySerializer
{
    int unk00;
    Config_dda340* config;
    void* criticalSection;

    bool Write(const hkArrayBase<unsigned int>& wordVarIdx, const hkArrayBase<unsigned int>& wordStack,
               const hkArrayBase<unsigned int>& quadVarIdx, const hkArrayBase<Vec4>& quadStack,
               const hkArrayBase<const char*>& activeNodes, const hkArrayBase<unsigned int>& activeStateIds,
               const hkArrayBase<unsigned int>& activeTransitions, const hkArrayBase<QsTransform_dda340>& transforms);
};

// Writes supplied data when a config exists; returns false only when the forwarded value is nonzero.
// @fnv 0x00dda340
// pdb: hkServerDebugDisplayHandler::updateBehavior | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: wordVarIdx, wordStack, quadVarIdx, quadStack, activeNodes, activeStateIds, activeTransitions, transforms
// pdb locals: strsizes, ss, t
bool BehaviorGraphDisplaySerializer::Write(const hkArrayBase<unsigned int>& wordVarIdx, const hkArrayBase<unsigned int>& wordStack,
                          const hkArrayBase<unsigned int>& quadVarIdx, const hkArrayBase<Vec4>& quadStack,
                          const hkArrayBase<const char*>& activeNodes, const hkArrayBase<unsigned int>& activeStateIds,
                          const hkArrayBase<unsigned int>& activeTransitions, const hkArrayBase<QsTransform_dda340>& transforms)
{
    _fnv_EnterCriticalSection(this->criticalSection);
    if (this->config)
    {
        int size2 = wordStack.m_iElementCount;
        int iVec4Count = quadStack.m_iElementCount;
        int totalSize = ((size2 + iVec4Count * 2) * 2 + transforms.m_iElementCount * 7 + activeStateIds.m_iElementCount + activeTransitions.m_iElementCount) * 4 + 25;
        Array_dda340LocalForm stringLengths;
        for (int i = 0; i < activeNodes.m_iElementCount; ++i)
        {
            if (activeNodes.m_data[i] != 0)
            {
                totalSize += 2;
                int len = hkString_strLen(activeNodes.m_data[i]);
                stringLengths.AddBack(len);
                totalSize += len;
            }
            else
            {
                stringLengths.AddBack(0);
            }
        }

        this->config->hkOArchive_write32u(totalSize);
        this->config->hkOArchive_write8u(0x12);
        this->config->hkOArchive_write32u(size2);

        for (int i = 0; i < wordStack.m_iElementCount; ++i)
        {
            this->config->hkOArchive_write32u(wordVarIdx.m_data[i]);
            this->config->hkOArchive_write32u(wordStack.m_data[i]);
        }

        this->config->hkOArchive_write32u(iVec4Count);

        for (int i = 0; i < quadStack.m_iElementCount; ++i)
        {
            this->config->hkOArchive_write32u(quadVarIdx.m_data[i]);
            this->config->hkDisplaySerializeOStream_writeQuadVector4((unsigned int)&quadStack.m_data[i]);
        }

        this->config->hkOArchive_write32u((unsigned short)activeNodes.m_iElementCount);

        for (int i = 0; i < activeNodes.m_iElementCount; ++i)
        {
            if (activeNodes.m_data[i] != 0)
            {
                unsigned int len = stringLengths.m_data[i];
                this->config->WriteUInt16(len);
                this->config->hkOArchive_writeRaw(activeNodes.m_data[i], len);
            }
        }

        this->config->hkOArchive_write32u(transforms.m_iElementCount);

        for (int i = 0; i < transforms.m_iElementCount; ++i)
        {
            Transform_dda340 t;
            transforms.m_data[i].Object_ForwardCall_00ce5300(&t);
            this->config->hkDisplaySerializeOStream_writeTransform(&t);
        }

        this->config->hkOArchive_write32u(activeStateIds.m_iElementCount);

        for (int i = 0; i < activeStateIds.m_iElementCount; ++i)
        {
            this->config->hkOArchive_write32u(activeStateIds.m_data[i]);
        }

        this->config->hkOArchive_write32u(activeTransitions.m_iElementCount);

        for (int i = 0; i < activeTransitions.m_iElementCount; ++i)
        {
            this->config->hkOArchive_write32u(activeTransitions.m_data[i]);
        }

        stringLengths.ClearAndRelease();
    }

    char status;
    bool failed = (this->config != 0 && *this->config->ForwardValueToObject((unsigned int*)&status) != 0);

    _fnv_LeaveCriticalSection(this->criticalSection);
    return !failed;
}

// FUN_00dda650 : MSVC this-adjusting thunk  (sub ecx, 0x08 ; jmp <impl>)
struct A1_00dda650
{
    A1_00dda650();
    virtual void FUN_00dda650();
    char pad[0x04];
};
struct A2_00dda650
{
    A2_00dda650();
    virtual void FUN_00dda650();
};
struct Own_00dda650 : A1_00dda650, A2_00dda650
{
    Own_00dda650();
    virtual void key_fn();
// @fnv 0x00dda650
    void FUN_00dda650();
};
Own_00dda650::Own_00dda650()
{}
void Own_00dda650::key_fn()
{}
Own_00dda650* make_00dda650()
{
    static Own_00dda650 o;
    return &o;
}

namespace fnv_00dda660 {
struct FUN_00dda660_Allocator
{
    virtual void unk00();
    virtual void unk04();
    virtual void ReleaseBlock(void* p, int numBytes);
};

struct FUN_00dda660_MemoryRouter
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    FUN_00dda660_Allocator* m_pStreamAllocator;

    FUN_00dda660_Allocator& heap()
    {
        return *m_pStreamAllocator;
    }
};

extern "C" FUN_00dda660_MemoryRouter& __cdecl hkMemoryRouter_getInstance();

struct FUN_00dda660
{
    unsigned short m_usMemSizeAndFlags;
    short m_sReferenceCount;

    FUN_00dda660();
    virtual ~FUN_00dda660();

    static void operator delete(void* p, size_t nbytes)
    {
        FUN_00dda660* b = static_cast<FUN_00dda660*>(p);
        hkMemoryRouter_getInstance().heap().ReleaseBlock(p, b->m_usMemSizeAndFlags);
    }
};

// The constructor has an empty body.
// @fnv 0x00dda660
// pdb: hkServerDebugDisplayHandler::`scalar deleting destructor' | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
FUN_00dda660::FUN_00dda660()
{}
}  // namespace fnv_00dda660

unsigned int _getGeometryByteSize();

struct FUN_00dd9da0_owner
{
    void hkServerDebugDisplayHandler_sendGeometryData(unsigned int geometries);
};

struct ConfigQ : FUN_00d605d0_Class
{
    void WriteInt(unsigned int value);
    void hkDisplaySerializeOStream_writeTransform(unsigned int value);
    const char* ForwardValueToObject(unsigned int* value);
};

struct SecondaryDisplay
{
    int unk00;
    ConfigQ* config;
    void* criticalSection;

    bool hkServerDebugDisplayHandler_displayGeometry(unsigned int geometries, unsigned int transform, unsigned int color, int id, unsigned int tag);
};

// Locks criticalSection, writes values through config when present, and returns true if config is absent or the returned first character is zero.
// @fnv 0x00dd9e90
// src: HAVOK_LIB 0x00dd9e90
// pdb: hkServerDebugDisplayHandler::displayGeometry | hkServerDebugDisplayHandler.obj | retail, vtable slot, high
// pdb params: geometries, transform, color, id, tag
bool SecondaryDisplay::hkServerDebugDisplayHandler_displayGeometry(unsigned int geometries, unsigned int transform, unsigned int color, int id, unsigned int tag)
{
    _fnv_EnterCriticalSection(criticalSection);
    if (config)
    {
        unsigned int a2 = geometries;
        config->WriteInt(_getGeometryByteSize() + 0x2d);
        config->hkOArchive_write8u(5);
        reinterpret_cast<FUN_00dd9da0_owner*>((char*)this - 8)->hkServerDebugDisplayHandler_sendGeometryData(a2);
        config->hkDisplaySerializeOStream_writeTransform(transform);
        config->hkOArchive_write32u(color);
        config->hkOArchive_WriteBinaryData_Thunk_00d609e0((__int64)id);
        config->WriteInt(tag);
    }
    bool ok = config != 0 && config->ForwardValueToObject(&geometries)[0] != 0;
    _fnv_LeaveCriticalSection(criticalSection);
    return !ok;
}
