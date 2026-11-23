#pragma once

#include "TESObjectACTI.h"
#include "NiPoint3.h"

// pdb: class TESFurniture, size 0x94, bases: TESObjectACTI, 1 members
class TESFurniture : public TESObjectACTI
{
public:
    TESFurniture();
    virtual ~TESFurniture();

    virtual void InitializeComponent();
    virtual bool LoadForm(TESFile* modInfo);
    virtual void SaveForm(void);
    virtual void InitItem(void);
    virtual void CopyFrom(const TESForm* form);
    virtual bool Compare(TESForm* form);

    void ResetFormData();
    bool Load(TESFile* apFile);
    bool GetCanSitOn();
    void SetCanSitOn(bool bSit);
    bool GetCanSleepOn();
    void SetCanSleepOn(bool bSleep);
    bool IsMarkerEnabled(unsigned int aiMarker);
    void SetMarkerEnabled(unsigned int aiMarker, bool bEnabled);
    void UpdateSitSleepFlags(NiAVObject* apMarker);
    static unsigned int GetMarkerCount(NiAVObject* apMarker);
    static unsigned int GetMarkerNumber(NiAVObject* apMarker, unsigned int auMarker);
    static bool __cdecl IsFurnitureSitMarker(unsigned int auMarker);
    static bool __cdecl IsFurnitureSleepMarker(unsigned int auMarker);
    void InitializeMarkerFlags();
    NiPoint3 GetMarkerOffset(unsigned int aiIndex, float afScale);
    float GetMarkerRotation(unsigned int auMarker);
    NiPoint3 FUN_00509920(unsigned int aiIndex, float afScale);
    float GetFurnitureHeading(unsigned int auMarker);
    int _FNV_GetFormType();

    int m_iMarkerCount;
};

typedef char TESFurniture_size_check[1];
typedef char TESFurniture_marker_count_offset_check[1];
