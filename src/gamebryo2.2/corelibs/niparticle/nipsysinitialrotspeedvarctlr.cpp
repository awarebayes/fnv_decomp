
// src/gamebryo2.2/corelibs/niparticle/nipsysinitialrotspeedvarctlr.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/niparticle/nipsysinitialrotspeedvarctlr.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

extern "C" void* NiPSysInitialRotSpeedVarCtlr_ms_RTTI; // src: XBOX_RETAIL_PDB 0x012026d8 (old: g_012026d8)

// @fnv 0x00c29e90
// src: COMMUNITY 0x00c29e90
void* __cdecl NiPSysInitialRotSpeedVarCtlr_GetRTTI()
{
    return &NiPSysInitialRotSpeedVarCtlr_ms_RTTI;
}

void __cdecl NiPSysModifierCtlr_NiPSysModifierCtlr(void);
extern int DAT_010c0e44;

struct FUN_00c32d80
{
    FUN_00c32d80();
};

// @fnv 0x00c32d80
// pdb: NiPSysModifierFloatCtlr::NiPSysModifierFloatCtlr | NiPSysModifierFloatCtlr.obj | retail, call graph, medium
// pdb params: pcModifierName
FUN_00c32d80::FUN_00c32d80()
{
    NiPSysModifierCtlr_NiPSysModifierCtlr();
    *(int*)this = (int)&DAT_010c0e44;
}

extern "C" void* NiPSysInitialRotSpeedVarCtlr_ms_RTTI; // src: XBOX_RETAIL_PDB 0x012026d8 (old: g_012026d8)
void __stdcall NiPSysModifierFloatCtlr_GetViewerStrings(void* p);
void* __cdecl NiViewerStrings_NiGetViewerString_Title(void* p);

struct FUN_00c29f00_Array
{
    void* unk00;
    void* unk04;
    unsigned short unk08;
    unsigned short unk0A;
    unsigned short unk0C;
    unsigned short unk0E;
    void FUN_0096ad30(unsigned int size);
    void FUN_0096ae90(unsigned int index, void** item);
};

// Grows the array if its count has reached capacity, then stores the item obtained from g_012026d8 at the current count.
// @fnv 0x00c29f00
// src: XBOX_PDB 0x00c29f00
// pdb: NiPSysInitialRotSpeedVarCtlr::GetViewerStrings | NiPSysInitialRotSpeedVarCtlr.obj | retail, upstream name already matches, high
// pdb params: pkStrings
void __stdcall NiPSysInitialRotSpeedVarCtlr_GetViewerStrings(FUN_00c29f00_Array* pkStrings)
{
    NiPSysModifierFloatCtlr_GetViewerStrings(pkStrings);
    void* item = NiViewerStrings_NiGetViewerString_Title(NiPSysInitialRotSpeedVarCtlr_ms_RTTI);
    unsigned int count = pkStrings->unk0A;
    if (count >= pkStrings->unk08)
        pkStrings->FUN_0096ad30(count + pkStrings->unk0E);
    pkStrings->FUN_0096ae90(count, &item);
}

extern "C" char DAT_010becbc;
extern "C" void* __cdecl Object_DispatchCommandFour(unsigned int size);

struct SpawnedCtlr
{
    void FUN_00c32d80();
    virtual void v00();
};

struct SpawnedHost
{
    void FUN_00c32de0(SpawnedCtlr* obj, void* a1);
    SpawnedCtlr* CreateSpawnedCtlr(void* a1);
};

// Allocates a 0x40-byte SpawnedCtlr, initialises it and its vtable, passes it to FUN_00c32de0 with a1, and returns it.
// @fnv 0x00c29eb0
// pdb: NiPSysInitialRotSpeedVarCtlr::CreateClone | NiPSysInitialRotSpeedVarCtlr.obj | retail, object-file run extension, low
// pdb note: debug build links it to NiPSysInitialRotSpeedVarCtlr::IsEqual
// pdb params: kCloning
// pdb locals: pkObject
SpawnedCtlr* SpawnedHost::CreateSpawnedCtlr(void* a1)
{
    SpawnedCtlr* obj = (SpawnedCtlr*)Object_DispatchCommandFour(0x40);
    if (obj != 0)
    {
        obj->FUN_00c32d80();
        *(void* volatile*)obj = &DAT_010becbc;
    }
    else
    {
        obj = 0;
    }
    this->FUN_00c32de0(obj, a1);
    return obj;
}

void* __cdecl Object_DispatchCommandFour(unsigned int size);

struct SpawnedFloat
{
    unsigned int unk00[6];
    static void* operator new(size_t size)
    {
        return Object_DispatchCommandFour(size);
    }

    SpawnedFloat(float value) throw();
};

struct FUN_00c32da0_cls
{
    virtual void vfn00();
    virtual void vfn01();
    virtual void vfn02();
    virtual void vfn03();
    virtual void vfn04();
    virtual void vfn05();
    virtual void vfn06();
    virtual void vfn07();
    virtual void vfn08();
    virtual void vfn09();
    virtual void vfn0A();
    virtual void vfn0B();
    virtual void vfn0C();
    virtual void vfn0D();
    virtual void vfn0E();
    virtual void vfn0F();
    virtual void vfn10();
    virtual void vfn11();
    virtual void vfn12();
    virtual void vfn13();
    virtual void vfn14();
    virtual void vfn15();
    virtual void vfn16();
    virtual void vfn17();
    virtual void vfn18();
    virtual void vfn19();
    virtual void vfn1A();
    virtual void vfn1B();
    virtual void vfn1C();
    virtual void vfn1D();
    virtual void vfn1E();
    virtual void vfn1F();
    virtual void vfn20();
    virtual void vfn21();
    virtual void vfn22();
    virtual void vfn23();
    virtual void vfn24();
    virtual void vfn25();
    virtual void vfn26();
    virtual void vfn27();
    virtual void vfn28();
    virtual void vfn29();
    virtual void vfn2A();
    virtual void vfn2B();
    virtual void vfn2C();
    virtual void vfn2D();
    virtual void vfn2E();
    virtual void vfn2F();
    virtual void vfn30();
    virtual void vfn31();
    virtual void vfn32();
    virtual void vfn33();
    virtual void vfn34();
    virtual void vfn35();
    virtual void vfn36();
    virtual void vfn37();
    virtual void vfn38();
    virtual void vfn39();
    virtual void vfn3A();
    virtual void vfn3B();
    virtual void FetchValue(float* out);

    void* CreateSpawnedFloat(float unused);
};

// Fetches a float via FetchValue and allocates a SpawnedFloat initialized with that value, returning it.
// @fnv 0x00c32da0
// pdb: NiPSysModifierFloatCtlr::CreatePoseInterpolator | NiPSysModifierFloatCtlr.obj | retail, object-file run extension, low
// pdb params: usIndex
// pdb locals: fValue
void* FUN_00c32da0_cls::CreateSpawnedFloat(float unused)
{
    float value;
    this->FetchValue(&value);
    SpawnedFloat* obj = 0;
    obj = new SpawnedFloat(value);
    return obj;
}
