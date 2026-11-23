#include "../../include/BSStringT.h"
// fallout_shared/mediaset.cpp: MediaSet, MediaSetLoad, MediaSetBattle and more (22 functions)

#include "../../include/MediaSet.h"
#include "../../include/BSAudio.h"
#include "mediaset_decls.h"
#include "../../include/BSSoundHandle.h"
#include "../../include/GO.h"
#include "../../include/Sky.h"
#include "../../include/TES.h"

// @fnv 0x00598a70
// pdb: MediaSet::InitItem | MediaSet.obj | retail, string-set bridge, high
// pdb locals: itempInt
void MediaSet::InitItem()
{
    unsigned int formID;
    if (GetInitialized())
        return;

    formID = 0;
    formID = reinterpret_cast<unsigned int>(pSoundOne);
    if (formID)
    {
        RemapFormIDForFile(formID, GetFile(-1));
        pSoundOne = dynamic_cast<TESSound*>(TESForm::GetFormByID(formID));
        if (!pSoundOne)
            Warning("MASTERFILE: Could not find open sound (%08X) for MediaSet '%s' (%08X).", formID, GetName(),
                    GetFormID());
    }

    formID = reinterpret_cast<unsigned int>(pSoundTwo);
    if (formID)
    {
        RemapFormIDForFile(formID, GetFile(-1));
        pSoundTwo = dynamic_cast<TESSound*>(TESForm::GetFormByID(formID));
        if (!pSoundTwo)
            Warning("MASTERFILE: Could not find open sound (%08X) for MediaSet '%s' (%08X).", formID, GetName(),
                    GetFormID());
    }

    SetInitializationState(true);
}

// Resets six entries, then zeroes two sound members.
// @fnv 0x00599bd0
// pdb: MediaSet::ClearData | MediaSet.obj | retail, upstream name already matches, high
void MediaSet::ClearData()
{
    pMLOne.Reset();
    pMLTwo.Reset();
    pMLThree.Reset();
    pMLFour.Reset();
    pMLFive.Reset();
    pMLSix.Reset();
    pSoundOne = 0;
    pSoundTwo = 0;
}

// @fnv 0x00599b30
// pdb: MediaSet::InitializeData | MediaSet.obj | retail, vtable slot, high
void MediaSet::ResetFormData()
{
    pMLOne.Reset();
    pMLTwo.Reset();
    pMLThree.Reset();
    pMLFour.Reset();
    pMLFive.Reset();
    pMLSix.Reset();
    fOne = 0.0f;
    fTwo = 0.0f;
    fThree = 0.0f;
    fFour = 0.0f;
    pSoundOne = 0;
    pSoundTwo = 0;
}

// @fnv 0x00597990
// pdb: MediaSet::Update | MediaSet.obj | retail, upstream name already matches, high
// pdb params: ticks, abInCombat, pController
bool MediaSet::Update(unsigned int ticks, bool abInCombat, MediaLocationController* pController)
{
    if (pController)
    {
        switch (Type)
        {
        case LAYER_LOCATION:
            return UpdateLayerLocation(ticks, abInCombat, pController);
            break;
        case LAYER_DUNGEON:
            return PointLocation(ticks, abInCombat, pController);
            break;
        case BATTLE:
            return UpdateBattleState(abInCombat, ticks, 4);
        }
    }
    else if (!pController && Type == INCIDENTAL)
    {
        if (DAT_011ca9fc > ticks)
            return true;
        UpdateIncidental(ticks);
    }
    else if (!pController && Type == BATTLE)
    {
        return UpdateBattleState(abInCombat, ticks, 4);
    }
    return false;
}

// @fnv 0x00597610
// pdb: MediaSet::MediaSet | MediaSet.obj | retail, upstream name already matches, high
MediaSet::MediaSet()
{
    SetFormType(ENUM_FORM_ID_MSET_ID);
    Type = static_cast<MEDIASETTYPE>(-1);
    pMLOne.Reset();
    pMLTwo.Reset();
    pMLThree.Reset();
    pMLFour.Reset();
    pMLFive.Reset();
    pMLSix.Reset();
    fOne = 0.0f;
    fTwo = 0.0f;
    fThree = 0.0f;
    fFour = 0.0f;
    pSoundOne = 0;
    pSoundTwo = 0;
    DAT_011ca9fc = 0;
    uiLoopUpdate = 0;
    uiSeekTime = 0;
    bStarted = false;
    bInCombat = false;
    bIsDay = false;
    cCurrentLayer = 0;
}

// Selects an incidental media layer for the controller based on scale and layer percentages, then starts or stops playback and updates cached state.
// @fnv 0x00597a60
// pdb: MediaSet::UpdateDungeon | MediaSet.obj | retail, call graph, low
// pdb note: debug build links it to MediaSet::UpdateLocation
// pdb params: ticks, abInCombat, pController
// pdb locals: cLayer, bMusicPlaying, dur, bReturn, bLoop, newLayer, uiSeekTime
bool MediaSet::UpdateLayerLocation(unsigned int time, bool force, MediaLocationController* controller)
{
    bool bReturn;
    bool bPlaying = FalloutAudioMedia::IsMediaPlaying(MEDIATYPE_MEDIA_LOCATION, false);
    bReturn = true;
    char cLayer = 0;
    if (force)
    {
        return false;
    }
    else
    {
        bool a;  // Restart after the previous incidental reaches its fade window.
        unsigned int fMinPercent = FalloutAudioMedia_MediaGetSubDuration(MEDIATYPE_MEDIA_LOCATION);
        a = false;
        if (uiLoopUpdate && fMinPercent && time > uiLoopUpdate + fMinPercent - fThree * 1000.0)
        {
            FalloutAudioMedia::FadeOutMedia(static_cast<int>(fThree * 1000.0), MEDIATYPE_MEDIA_LOCATION);
            uiLoopUpdate = time;
            if (!controller->GetLoop())
                a = true;
            else
                return false;
        }
        float fNormalizedPosition = reinterpret_cast<TESObjectREFR*>(controller)->GetScale() * 100.0;
        float m_uiPlaybackFlags = 1000.0f;  // Lowest eligible layer percentage.
        if (controller->GetByteAtOffset55())
        {
            if ((cEnableFlags & 1) && pMLOne.Percent > fNormalizedPosition &&
                pMLOne.Percent < m_uiPlaybackFlags)
            {
                cLayer = 1;
                m_uiPlaybackFlags =
                    pMLOne.Percent > m_uiPlaybackFlags ? m_uiPlaybackFlags : pMLOne.Percent;
            }
            if ((cEnableFlags & 2) && pMLTwo.Percent > fNormalizedPosition &&
                pMLTwo.Percent < m_uiPlaybackFlags)
            {
                cLayer = 2;
                m_uiPlaybackFlags =
                    pMLTwo.Percent > m_uiPlaybackFlags ? m_uiPlaybackFlags : pMLTwo.Percent;
            }
            if ((cEnableFlags & 4) && pMLThree.Percent > fNormalizedPosition &&
                pMLThree.Percent < m_uiPlaybackFlags)
            {
                cLayer = 3;
                m_uiPlaybackFlags =
                    pMLThree.Percent > m_uiPlaybackFlags ? m_uiPlaybackFlags : pMLThree.Percent;
            }
        }
        else
        {
            if ((cEnableFlags & 8) && pMLFour.Percent > fNormalizedPosition &&
                pMLFour.Percent < m_uiPlaybackFlags)
            {
                cLayer = 1;
                m_uiPlaybackFlags =
                    pMLFour.Percent > m_uiPlaybackFlags ? m_uiPlaybackFlags : pMLFour.Percent;
            }
            if ((cEnableFlags & 16) && pMLFive.Percent > fNormalizedPosition &&
                pMLFive.Percent < m_uiPlaybackFlags)
            {
                cLayer = 2;
                m_uiPlaybackFlags =
                    pMLFive.Percent > m_uiPlaybackFlags ? m_uiPlaybackFlags : pMLFive.Percent;
            }
            if ((cEnableFlags & 32) && pMLSix.Percent > fNormalizedPosition &&
                pMLSix.Percent < m_uiPlaybackFlags)
            {
                cLayer = 3;
                m_uiPlaybackFlags =
                    pMLSix.Percent > m_uiPlaybackFlags ? m_uiPlaybackFlags : pMLSix.Percent;
            }
        }
        if (!(bPlaying && bInCombat == force && bIsDay == controller->GetByteAtOffset55() &&
              cCurrentLayer == cLayer) &&
                DAT_011dd37c < time ||
            a)
        {
            MediaSetEntry* pLayer = 0;
            if (controller->GetByteAtOffset55())
            {
                switch (cLayer)
                {
                case 1:
                    pLayer = &pMLOne;
                    break;
                case 2:
                    pLayer = &pMLTwo;
                    break;
                case 3:
                    pLayer = &pMLThree;
                }
            }
            else
            {
                switch (cLayer)
                {
                case 1:
                    pLayer = &pMLFour;
                    break;
                case 2:
                    pLayer = &pMLFive;
                    break;
                case 3:
                    pLayer = &pMLSix;
                }
            }
            if (pLayer)
            {
                DAT_011dd37c = static_cast<unsigned int>(time + fOne * 1000.0);
                unsigned int uiElapsedMilliseconds = 0;
                if (FalloutAudioMedia_MediaGetPosition(MEDIATYPE_MEDIA_LOCATION) && !a)
                    uiElapsedMilliseconds = time - FalloutAudioMedia_MediaGetPosition(MEDIATYPE_MEDIA_LOCATION);
                FalloutAudioMedia::PlayMedia(
                    MEDIATYPE_MEDIA_LOCATION,
                    reinterpret_cast<const char*>(reinterpret_cast<const NiPointer<NiNode>*>(pLayer)->operator->()),
                    static_cast<unsigned int>(fThree * 1000.0), !controller->GetLoop(), true,
                    pLayer->Attenuation, uiElapsedMilliseconds);
                FalloutAudioMedia::EndMediaUpdate();
                uiLoopUpdate = time;
            }
            else
            {
                if (bPlaying)
                    FalloutAudioMedia::FadeOutMedia(static_cast<int>(fThree * 1000.0), MEDIATYPE_MEDIA_LOCATION);
                bReturn = false;
            }
            bInCombat = force;
            bIsDay = controller->GetByteAtOffset55();
            cCurrentLayer = cLayer;
        }
    }
    return bReturn;
}

extern float const runtimeFloatSettingDefault313;

// Clears the name and restores the attenuation and percentage defaults.
// @fnv 0x00597850
// pdb: MediaSet::MediaLayer::Clear | MediaSet.obj | debug, call graph, medium
void MediaSetEntry::Reset()
{
    Name.Set(0, 0);
    Attenuation = runtimeFloatSettingDefault313;
    Percent = 100.0f;
}

// @fnv 0x007d9ec0
// src: GUESS 0x007d9ec0 (confidence: medium)
// pdb: MediaSet::MediaLayer::~MediaLayer | MediaSet.obj | debug, call graph, medium
void __fastcall BSStringT_BSStringT_Thunk(BSStringT<char>* apString)
{
    apString->BSStringT<char>::~BSStringT<char>();
}

extern "C" void* __cdecl _alloca(size_t);



struct MediaSetLoad : public MediaSet
{
    bool MediaSet_Load(TESFile* apFile);
};

// Reads a MediaSet record from the file, handling EDID, OBND, FULL, type, name, attenuation, percent, sound and float chunks, then returns true.
// @fnv 0x00598bc0
// src: XBOX_PDB 0x00598bc0
// pdb: MediaSet::Load | MediaSet.obj | retail, string-set bridge, high
// pdb params: apFile
// pdb locals: itempint, chunkid, pMessage, cSize, tF, stemp
bool MediaSetLoad::MediaSet_Load(TESFile* apFile)
{
    unsigned int itempint = 0;
    int chunkid;

    LoadRecordHeader(apFile);
    SetInitializationState(false);

    while ((chunkid = (int)apFile->GetChunkID()) != 0)
    {
        switch (chunkid)
        {
        case 'DIDE': {
            char* pMessage = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(pMessage, 0x200);
            SetEditorID_AtRuntime(pMessage);
            break;
        }
        case 'DNBO':
            readOBNDSubRecord(apFile);
            break;
        case 'LLUF':
            TESFullName::LoadFullNameChunk(this, apFile);
            break;
        case '1MAN':
            apFile->GetChunkData(&itempint);
            Type = (MEDIASETTYPE)itempint;
            break;
        case '2MAN':
        case '3MAN':
        case '4MAN':
        case '5MAN':
        case '6MAN':
        case '7MAN': {
            char* pMessage = 0;
            float tF = 0.0f;
            unsigned int cSize = apFile->GetChunkSize();
            if (cSize)
            {
                pMessage = (char*)_alloca(apFile->GetChunkSize());
                apFile->GetChunkData(pMessage, 0);
                ::BSStringT<char> stemp("Data\\Music\\");
                stemp += pMessage;
                switch (chunkid)
                {
                case '2MAN':
                    ((::BSStringT<char>*)&pMLOne.Name)->Set(stemp);
                    break;
                case '3MAN':
                    ((::BSStringT<char>*)&pMLTwo.Name)->Set(stemp);
                    break;
                case '4MAN':
                    ((::BSStringT<char>*)&pMLThree.Name)->Set(stemp);
                    break;
                case '5MAN':
                    ((::BSStringT<char>*)&pMLFour.Name)->Set(stemp);
                    break;
                case '6MAN':
                    ((::BSStringT<char>*)&pMLFive.Name)->Set(stemp);
                    break;
                case '7MAN':
                    ((::BSStringT<char>*)&pMLSix.Name)->Set(stemp);
                    break;
                }
            }
            break;
        }
        case '8MAN':
        case '9MAN':
        case '0MAN':
        case 'MANA':
        case 'MANB':
        case 'MANC': {
            float tF = 0.0f;
            apFile->GetChunkData((unsigned int*)&tF);
            switch (chunkid)
            {
            case '8MAN':
                pMLOne.Attenuation = tF;
                break;
            case '9MAN':
                pMLTwo.Attenuation = tF;
                break;
            case '0MAN':
                pMLThree.Attenuation = tF;
                break;
            case 'MANA':
                pMLFour.Attenuation = tF;
                break;
            case 'MANB':
                pMLFive.Attenuation = tF;
                break;
            case 'MANC':
                pMLSix.Attenuation = tF;
                break;
            }
            break;
        }
        case 'MANJ':
        case 'MANK':
        case 'MANL':
        case 'MANM':
        case 'MANN':
        case 'MANO': {
            float tF = 0.0f;
            apFile->GetChunkData((unsigned int*)&tF);
            switch (chunkid)
            {
            case 'MANJ':
                pMLOne.Percent = tF;
                break;
            case 'MANK':
                pMLTwo.Percent = tF;
                break;
            case 'MANL':
                pMLThree.Percent = tF;
                break;
            case 'MANM':
                pMLFour.Percent = tF;
                break;
            case 'MANN':
                pMLFive.Percent = tF;
                break;
            case 'MANO':
                pMLSix.Percent = tF;
                break;
            }
            break;
        }
        case 'MANP':
            ReadTypedData(apFile, &cEnableFlags, 1);
            break;
        case 'MAND':
        case 'MANE':
        case 'MANF':
        case 'MANG': {
            float tF = 0.0f;
            apFile->GetChunkData((unsigned int*)&tF);
            switch (chunkid)
            {
            case 'MAND':
                fOne = tF;
                break;
            case 'MANE':
                fTwo = tF;
                break;
            case 'MANF':
                fThree = tF;
                break;
            case 'MANG':
                fFour = tF;
                break;
            }
            break;
        }
        case 'MANH':
        case 'MANI': {
            unsigned int itempint = 0;
            apFile->GetChunkData(&itempint);
            switch (chunkid)
            {
            case 'MANH':
                pSoundOne = (TESSound*)itempint;
                break;
            case 'MANI':
                pSoundTwo = (TESSound*)itempint;
                break;
            }
            break;
        }
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x00598000
// src: COMMUNITY 0x00598000
int MediaLocationController::GetLoop()
{
    return (iFlags >> 4) & 3;
}

struct MediaSetLifetime
{
    void* Destroy(unsigned int flags);
};
void _Deallocate(void*);
// Destroys the media set, optionally deallocates it, and returns its address.
// @fnv 0x00597880
// pdb: MediaSet::`scalar deleting destructor' | MediaSet.obj | retail, vtable slot, high
void* MediaSetLifetime::Destroy(unsigned int flags)
{
    reinterpret_cast<MediaSet*>(this)->MediaSet::~MediaSet();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

struct UnnamedStr
{
    int _00;
    int _04;
    void __thiscall AssignFromString(const UnnamedStr&);
};
// Operations absent from the shared MediaSetEntry declaration use a stateless
// adapter; all storage and offsets come from MediaSetEntry.
struct MediaSetEntryOperations
{
    MediaSetEntry* MediaSet_MediaLayer_ctor(const MediaSetEntry& other);
};

// @fnv 0x00599780
// pdb: MediaSet::MediaLayer::MediaLayer | MediaSet.obj | retail, object-file run extension, high; the copy constructor (??0MediaLayer@MediaSet@@QAA@ABU01@@Z); 0x005977d0 is the default constructor
MediaSetEntry* MediaSetEntryOperations::MediaSet_MediaLayer_ctor(const MediaSetEntry& other) // src: XBOX_RETAIL_PDB 0x00599780
{
    reinterpret_cast<UnnamedStr*>(this)->AssignFromString(*reinterpret_cast<const UnnamedStr*>(&other.Name));
    reinterpret_cast<MediaSetEntry*>(this)->Attenuation = other.Attenuation;
    reinterpret_cast<MediaSetEntry*>(this)->Percent = other.Percent;
    return reinterpret_cast<MediaSetEntry*>(this);
}

struct S7010 : GO
{
    unsigned int Length();
};

struct MediaFormWriter
{
    void Finish();
};

void __cdecl TESForm_AddChunkArray(unsigned int, void*, unsigned int);
void __cdecl TESForm_AddChunk_3(unsigned int, unsigned char);
void __cdecl _FUN_00485910(unsigned int, unsigned int);
void __cdecl _FUN_00485910(unsigned int, float);

// @fnv 0x005991b0
// pdb: MediaSet::Save | MediaSet.obj | retail, vtable slot, high
void MediaSet::SaveMediaSet()
{
    TESForm::BeginFormRecord();
    TESFullName::Save_00487010();

    _FUN_00485910(0x314d414e, *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(this) + 0x44));

    if (reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x48)->Length() > 0)
    {
        TESForm_AddChunkArray(0x324d414e,
                     reinterpret_cast<void*>(static_cast<unsigned int>(
                         *reinterpret_cast<const GO*>(reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x48)))),
                     reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x48)->Length() + 1);
    }

    if (reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x58)->Length() > 0)
    {
        TESForm_AddChunkArray(0x334d414e,
                     reinterpret_cast<void*>(static_cast<unsigned int>(
                         *reinterpret_cast<const GO*>(reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x58)))),
                     reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x58)->Length() + 1);
    }

    if (reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x68)->Length() > 0)
    {
        TESForm_AddChunkArray(0x344d414e,
                     reinterpret_cast<void*>(static_cast<unsigned int>(
                         *reinterpret_cast<const GO*>(reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x68)))),
                     reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x68)->Length() + 1);
    }

    if (reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x78)->Length() > 0)
    {
        TESForm_AddChunkArray(0x354d414e,
                     reinterpret_cast<void*>(static_cast<unsigned int>(
                         *reinterpret_cast<const GO*>(reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x78)))),
                     reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x78)->Length() + 1);
    }

    if (reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x88)->Length() > 0)
    {
        TESForm_AddChunkArray(0x364d414e,
                     reinterpret_cast<void*>(static_cast<unsigned int>(
                         *reinterpret_cast<const GO*>(reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x88)))),
                     reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x88)->Length() + 1);
    }

    if (reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x98)->Length() > 0)
    {
        TESForm_AddChunkArray(0x374d414e,
                     reinterpret_cast<void*>(static_cast<unsigned int>(
                         *reinterpret_cast<const GO*>(reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x98)))),
                     reinterpret_cast<S7010*>(reinterpret_cast<char*>(this) + 0x98)->Length() + 1);
    }

    _FUN_00485910(0x384d414e, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x50));
    _FUN_00485910(0x394d414e, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x60));
    _FUN_00485910(0x304d414e, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x70));
    _FUN_00485910(0x4d414e41, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x80));
    _FUN_00485910(0x4d414e42, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x90));
    _FUN_00485910(0x4d414e43, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0xa0));
    _FUN_00485910(0x4d414e4a, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x54));
    _FUN_00485910(0x4d414e4b, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x64));
    _FUN_00485910(0x4d414e4c, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x74));
    _FUN_00485910(0x4d414e4d, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x84));
    _FUN_00485910(0x4d414e4e, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x94));
    _FUN_00485910(0x4d414e4f, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0xa4));

    TESForm_AddChunk_3(0x4d414e50, *reinterpret_cast<unsigned char*>(reinterpret_cast<char*>(this) + 0xa8));

    _FUN_00485910(0x4d414e44, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0xac));
    _FUN_00485910(0x4d414e45, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0xb0));
    _FUN_00485910(0x4d414e46, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0xb4));
    _FUN_00485910(0x4d414e47, *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0xb8));

    if (*reinterpret_cast<TESSound**>(reinterpret_cast<char*>(this) + 0xbc))
    {
        _FUN_00485910(0x4d414e48,
                      reinterpret_cast<TESSound*>(*reinterpret_cast<TESSound**>(reinterpret_cast<char*>(this) + 0xbc))
                          ->GetFormID());
    }

    if (*reinterpret_cast<TESSound**>(reinterpret_cast<char*>(this) + 0xc0))
    {
        _FUN_00485910(0x4d414e49,
                      reinterpret_cast<TESSound*>(*reinterpret_cast<TESSound**>(reinterpret_cast<char*>(this) + 0xc0))
                          ->GetFormID());
    }

    reinterpret_cast<MediaFormWriter*>(this)->Finish();
    TESForm::FinishFormRecord();
}

// @fnv 0x005978b0
// pdb: MediaSet::~MediaSet | MediaSet.obj | retail, upstream name already matches, high
MediaSet::~MediaSet()
{}

float __fastcall Sky_GetSunriseBegin(void* apSky);
float __fastcall Sky_GetSunsetEnd(void* apSky);

extern TES* g_TES;

unsigned int GetRandomNumberInRange(int aiMin, int aiMax);

struct MediaSetBattle : public MediaSet
{
    bool UpdateBattleMusic(unsigned int ticks);
};

// @fnv 0x00598890
// pdb: MediaSet::UpdateIncidentals | MediaSet.obj | retail, object-file run extension, high
// pdb params: ticks
// pdb locals: bDay, psky
bool MediaSetBattle::UpdateBattleMusic(unsigned int ticks)
{
    if (ticks <= DAT_011ca9fc)
    {
        return true;
    }
    Sky* pWeatherSky = g_TES->GetSky();
    bool m_bConditionWasTrue = pWeatherSky ? (Sky_GetSunriseBegin(pWeatherSky) <= pWeatherSky->GetFieldEC() &&
                                              pWeatherSky->GetFieldEC() <= Sky_GetSunsetEnd(pWeatherSky))
                                           : true;
    if (m_bConditionWasTrue)
    {
        BSAudio::GetSingleton()->GetSoundHandleByFormID(pSoundOne->GetFormID(), 0x901).Play(false);
        DAT_011ca9fc =
            GetRandomNumberInRange((int)(fOne * 1000.0), (int)(fThree * 1000.0)) + ticks;
    }
    else
    {
        BSAudio::GetSingleton()->GetSoundHandleByFormID(pSoundTwo->GetFormID(), 0x901).Play(false);
        DAT_011ca9fc =
            GetRandomNumberInRange((int)(fTwo * 1000.0), (int)(fFour * 1000.0)) + ticks;
    }
    return true;
}

struct MediaLayerData : MediaSetEntry
{
    MediaLayerData(const MediaLayerData& arOther);
    ~MediaLayerData();
    void SetLayer(MediaLayerData arLayer);
};

struct MediaSetBaseCopy
{
    void CopyBase(TESForm* apForm);
};

// 0x00599550 copies MediaSet fields; its missing shared declaration needs only a storage-free thiscall adapter.
struct MediaFormCopyOperation
{
    void CopyFrom(TESForm* apCopy);
};

// Copies Type, six layer entries and several scalar fields from another MediaSet, then calls CopyBase.
// @fnv 0x00599550
// pdb: MediaSet::Copy | MediaSet.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pcopy
void MediaFormCopyOperation::CopyFrom(TESForm* apCopy)
{
    MediaSet* pOther = dynamic_cast<MediaSet*>(apCopy);
    if (!pOther)
        return;
    reinterpret_cast<MediaSet*>(this)->Type = pOther->Type;
    ((MediaLayerData*)&reinterpret_cast<MediaSet*>(this)->pMLOne)->SetLayer(*(MediaLayerData*)&pOther->pMLOne);
    ((MediaLayerData*)&reinterpret_cast<MediaSet*>(this)->pMLTwo)->SetLayer(*(MediaLayerData*)&pOther->pMLTwo);
    ((MediaLayerData*)&reinterpret_cast<MediaSet*>(this)->pMLThree)->SetLayer(*(MediaLayerData*)&pOther->pMLThree);
    ((MediaLayerData*)&reinterpret_cast<MediaSet*>(this)->pMLFour)->SetLayer(*(MediaLayerData*)&pOther->pMLFour);
    ((MediaLayerData*)&reinterpret_cast<MediaSet*>(this)->pMLFive)->SetLayer(*(MediaLayerData*)&pOther->pMLFive);
    ((MediaLayerData*)&reinterpret_cast<MediaSet*>(this)->pMLSix)->SetLayer(*(MediaLayerData*)&pOther->pMLSix);
    reinterpret_cast<MediaSet*>(this)->cEnableFlags = pOther->cEnableFlags;
    reinterpret_cast<MediaSet*>(this)->fOne = pOther->fOne;
    reinterpret_cast<MediaSet*>(this)->fTwo = pOther->fTwo;
    reinterpret_cast<MediaSet*>(this)->fThree = pOther->fThree;
    reinterpret_cast<MediaSet*>(this)->fFour = pOther->fFour;
    reinterpret_cast<MediaSet*>(this)->pSoundOne = pOther->pSoundOne;
    reinterpret_cast<MediaSet*>(this)->pSoundTwo = pOther->pSoundTwo;
    ((MediaSetBaseCopy*)this)->CopyBase(pOther);
}

// local form of BSStringT<char>: 0x00599700 needs the 0x007d9ec0 destructor wrapper and dummy-argument accessor; shared replacement changes bytes.
struct TextValue
{
    char* text;
    unsigned short length;
    unsigned short capacity;
    TextValue* Modelloader_GetReferencedThis(int flag);
    ~TextValue();
};
struct StringRecord
{
    void FUN_004037f0(TextValue*);
    void SetValues(TextValue value, float firstValue, float secondValue);
};
// @fnv 0x00599700
// pdb: MediaSet::MediaLayer::Copy | MediaSet.obj | debug, call graph, high
// pdb params: l
void StringRecord::SetValues(TextValue aTextValue, float afFirst, float afSecond)
{
    FUN_004037f0(aTextValue.Modelloader_GetReferencedThis(0));
    reinterpret_cast<MediaSetEntry*>(this)->Attenuation = afFirst;
    reinterpret_cast<MediaSetEntry*>(this)->Percent = afSecond;
}

struct LayerInfo : MediaSetEntry
{

    const char* __thiscall Modelloader_GetReferencedThis(unsigned int index);
    int __thiscall FUN_00408a80(const char* text);
    bool __thiscall MediaSet_MediaLayer_Compare(LayerInfo l);
};
// @fnv 0x00599a50
// src: XBOX_RETAIL_PDB 0x00599a50
// pdb: MediaSet::MediaLayer::Compare | MediaSet.obj | retail, call graph, high
// pdb params: l
bool LayerInfo::MediaSet_MediaLayer_Compare(LayerInfo l)
{
    if (Attenuation != l.Attenuation)
        return true;
    if (Percent != l.Percent)
        return true;
    if (FUN_00408a80(l.Modelloader_GetReferencedThis(0)))
        return false;
    return true;
}

struct MediaSetNameHolder : public TESForm, public TESFullName
{
    virtual ~MediaSetNameHolder();
};

// @fnv 0x00599d00
// pdb: TESAmmoEffect::~TESAmmoEffect | TESAmmoEffect.obj | debug, object-file gap fill, medium
MediaSetNameHolder::~MediaSetNameHolder()
{}

bool __fastcall GetControllerCondition(MediaLocationController* controller);
bool __cdecl FalloutAudioMedia_MediaIsPlaying(unsigned long, bool);
unsigned int __cdecl FalloutAudioMedia_MediaGetDuration(unsigned long);
void __cdecl StartFade(int, int);
void __cdecl PlayAudioMedia(int, const char*, unsigned int, bool, bool, float, unsigned int);
void __cdecl FUN_008304c0();
extern unsigned int DAT_011ca9fc;

// @fnv 0x00598060
// pdb: MediaSet::UpdateLocation | MediaSet.obj | retail, call graph, low
// pdb note: debug build links it to MediaSet::UpdateDungeon
// pdb params: ticks, abInCombat, pController
// pdb locals: cLayer, bMusicPlaying, bReturn, fRadiusP, dur, bLoop, fRadius, newLayer, uiSeekTime
bool MediaSet::PointLocation(unsigned int auiTime, bool abEnabled, MediaLocationController* apCtrl)
{
    bool flagDTmp;
    bool retVal;
    unsigned int local7;
    bool b2;
    bool b4;
    MediaSetEntry* ptrLocal;
    int local4;
    b2 = FalloutAudioMedia_MediaIsPlaying(3, true);
    retVal = true;
    b4 = reinterpret_cast<TESObjectREFR*>(apCtrl)->GetScale() < 100.0;
    if (this->bInCombat == 0 && abEnabled)
    {
        DAT_011dd37c = 0;
    }
    local7 = FalloutAudioMedia_MediaGetDuration(2);
    flagDTmp = false;
    if (this->uiLoopUpdate != 0 && local7 != 0)
    {
        if ((double)auiTime > (double)(unsigned int)(this->uiLoopUpdate + local7) - this->fThree * 1000.0)
        {
            StartFade((int)(this->fThree * 1000.0), 2);
            this->uiLoopUpdate = auiTime;
            if (apCtrl->GetLoop() == 0)
            {
                flagDTmp = true;
            }
            else
            {
                return false;
            }
        }
    }
    if (!((b2 && this->bInCombat == abEnabled && this->bInTension == GetControllerCondition(apCtrl) &&
           ((char)this->cCurrentLayer == (char)b4 || (char)this->cCurrentLayer != 0)) ||
          DAT_011dd37c >= auiTime) ||
        flagDTmp)
    {
    body:
        ptrLocal = 0;
        if (abEnabled)
        {
            if (this->pSoundOne != 0 && !this->bStarted)
            {
                BSAudio::BSAudio_QInstance()->GetSoundHandleByFormID_00ad73b0((int)this->pSoundOne->TESForm::GetHavokWorldObjectCountInSceneGraph_0084e3a0(), 0x901).Play(false);
            }
            ptrLocal = &this->pMLOne;
            this->bStarted = 1;
        }
        else if (GetControllerCondition(apCtrl))
        {
            if (this->bStarted && this->pSoundTwo != 0)
            {
                BSAudio::BSAudio_QInstance()->GetSoundHandleByFormID_00ad73b0((int)this->pSoundTwo->TESForm::GetHavokWorldObjectCountInSceneGraph_0084e3a0(), 0x901).Play(false);
            }
            ptrLocal = &this->pMLThree;
            this->bStarted = 0;
        }
        else
        {
            if (this->bStarted && this->pSoundTwo != 0)
            {
                BSAudio::BSAudio_QInstance()->GetSoundHandleByFormID_00ad73b0((int)this->pSoundTwo->TESForm::GetHavokWorldObjectCountInSceneGraph_0084e3a0(), 0x901).Play(false);
            }
            ptrLocal = &this->pMLTwo;
            this->bStarted = 0;
        }
        if (ptrLocal != 0)
        {
            DAT_011dd37c = auiTime;
            if (!abEnabled)
            {
                DAT_011dd37c = (unsigned int)(this->fOne * 1000.0 + (double)DAT_011dd37c);
            }
            local4 = 0;
            if (FalloutAudioMedia_MediaGetPosition((MEDIATYPE)3) != 0 && !flagDTmp)
            {
                local4 = auiTime - FalloutAudioMedia_MediaGetPosition((MEDIATYPE)3);
            }
            if (ptrLocal == &this->pMLOne)
            {
                PlayAudioMedia(3, ptrLocal->Name.String(), 1000, true, true, ptrLocal->Attenuation, (unsigned int)local4);
            }
            else
            {
                PlayAudioMedia(3, ptrLocal->Name.String(), (unsigned int)(this->fThree * 1000.0), true, true,
                               ptrLocal->Attenuation, (unsigned int)local4);
            }
            FUN_008304c0();
        }
        else
        {
            StartFade((int)(this->fThree * 1000.0), 3);
            retVal = false;
        }
        this->bInCombat = abEnabled;
        this->bInTension = GetControllerCondition(apCtrl);
        this->cCurrentLayer = b4;
    }
    else
    {
        if ((char)b4 == 0)
        {
            if (auiTime > DAT_011ca9fc)
            {
                if (this->bStarted == 0)
                {
                    if (b2)
                    {
                        StartFade((int)(this->fThree * 1000.0), 3);
                    }
                    retVal = false;
                }
            }
        }
    }
    return retVal;
}

namespace fnv_00598550 {
struct DungeonController
{
    void FUN_007037c0(unsigned int);
};
extern DungeonController* DAT_011dd374;
bool FalloutAudioMedia_MediaIsPlaying(unsigned long, bool);
unsigned int FalloutAudioMedia_MediaGetDuration(unsigned long);
void FalloutAudioMedia_MediaPause();
void ResetThreadFlags();
void StartFade(int, int);
struct DungeonMediaSet : MediaSet
{
    bool UpdatePlayback(bool bInCombat, unsigned int ticks, int mType);
};
// Starts, refreshes or fades the media set's audio based on active, time and mediaType, updating DAT_011ca9fc and the playing flag.
// @fnv 0x00598550
// pdb: MediaSet::UpdateBattle | MediaSet.obj | retail, call graph, high
// pdb params: bInCombat, ticks, mType
// pdb locals: bIsLoaded, dur, bShouldPlayIntro, bShouldPlayOutro
bool DungeonMediaSet::UpdatePlayback(bool bInCombat, unsigned int ticks, int mType)
{
    bool changed;
    if (bInCombat)
    {
        changed = false;
        bool playing = FalloutAudioMedia_MediaIsPlaying(mType, false);
        if (!playing && !bStarted)
        {
            if (pSoundOne && !bStarted)
                BSAudio::GetSingleton()->GetSoundHandleByFormID(pSoundOne->GetFormID(), 0x901).Play(false);
            DAT_011ca9fc = ticks - 1;
            bStarted = true;
            FalloutAudioMedia::PlayMedia((MEDIATYPE)mType, pMLOne.Name.String(),
                                         (unsigned int)(fTwo * 1000.0), true, true,
                                         pMLOne.Attenuation, 0);
            FalloutAudioMedia::EndMediaUpdate();
            FalloutAudioMedia_MediaPause();
        }
        if (ticks > DAT_011ca9fc + fOne)
        {
            ResetThreadFlags();
            bStarted = true;
        }
        unsigned int length = FalloutAudioMedia_MediaGetDuration(mType);
        if (length && ticks > DAT_011ca9fc + length - 1000)
        {
            FalloutAudioMedia::PlayMedia((MEDIATYPE)mType, pMLOne.Name.String(),
                                         (unsigned int)(fTwo * 1000.0), true, true,
                                         pMLOne.Attenuation, 0);
            FalloutAudioMedia::EndMediaUpdate();
            DAT_011ca9fc = ticks;
        }
    }
    else
    {
        bool fading = false;
        if (FalloutAudioMedia_MediaIsPlaying(mType, false) && bStarted)
        {
            fading = true;
            StartFade((int)(fTwo * 1000.0), mType);
        }
        if (fading)
        {
            if (pSoundTwo)
                BSAudio::GetSingleton()->GetSoundHandleByFormID(pSoundTwo->GetFormID(), 0x901).Play(false);
            DAT_011ca9fc = ticks + fThree * 1000.0;
            if (DAT_011dd374)
                DAT_011dd374->FUN_007037c0(DAT_011ca9fc);
            bStarted = false;
        }
    }
    return bInCombat;
}
}  // namespace fnv_00598550

struct MediaLayerCompare
{
    bool MediaSet_MediaLayer_Compare(MediaLayerData l);
};
struct MediaFormCompare
{
    bool TESForm_CompareAllComponents(TESForm* apCompare);
};
struct MediaSetCompare : MediaSet
{
    bool IsDifferentFrom(TESForm* apCompare);
};

// Compares this MediaSet with form cast to MediaSet: returns true if any layer, scalar field or base comparison differs.
// @fnv 0x005997c0
// pdb: MediaSet::Compare | MediaSet.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pcomp
bool MediaSetCompare::IsDifferentFrom(TESForm* apCompare)
{
    MediaSet* other = dynamic_cast<MediaSet*>(apCompare);
    if (!other)
        return true;
    if (other->Type != Type)
        return true;
    if (reinterpret_cast<MediaLayerCompare*>(&pMLOne)
            ->MediaSet_MediaLayer_Compare(*reinterpret_cast<MediaLayerData*>(&other->pMLOne)))
        return true;
    if (reinterpret_cast<MediaLayerCompare*>(&pMLTwo)
            ->MediaSet_MediaLayer_Compare(*reinterpret_cast<MediaLayerData*>(&other->pMLTwo)))
        return true;
    if (reinterpret_cast<MediaLayerCompare*>(&pMLThree)
            ->MediaSet_MediaLayer_Compare(*reinterpret_cast<MediaLayerData*>(&other->pMLThree)))
        return true;
    if (reinterpret_cast<MediaLayerCompare*>(&pMLFour)
            ->MediaSet_MediaLayer_Compare(*reinterpret_cast<MediaLayerData*>(&other->pMLFour)))
        return true;
    if (reinterpret_cast<MediaLayerCompare*>(&pMLFive)
            ->MediaSet_MediaLayer_Compare(*reinterpret_cast<MediaLayerData*>(&other->pMLFive)))
        return true;
    if (reinterpret_cast<MediaLayerCompare*>(&pMLSix)
            ->MediaSet_MediaLayer_Compare(*reinterpret_cast<MediaLayerData*>(&other->pMLSix)))
        return true;
    if (cEnableFlags != other->cEnableFlags)
        return true;
    if (fOne != other->fOne)
        return true;
    if (fTwo != other->fTwo)
        return true;
    if (fThree != other->fThree)
        return true;
    if (fFour != other->fFour)
        return true;
    if (pSoundOne != other->pSoundOne)
        return true;
    if (pSoundTwo != other->pSoundTwo)
        return true;
    if (reinterpret_cast<MediaFormCompare*>(this)->TESForm_CompareAllComponents(other))
        return true;
    return false;
}
