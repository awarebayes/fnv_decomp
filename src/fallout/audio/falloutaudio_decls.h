#include "../../../include/Interface.h"
// Local audio helpers and globals.
#ifndef FALLOUTAUDIO_DECLS_H
#define FALLOUTAUDIO_DECLS_H

#include <stddef.h>
#include "../../../include/ENUM_FORM_ID.h"

#include "../../../include/SETTING_VALUE.h"
#include "../../../include/SettingT_GameSettingCollection_.h"

struct RegionSound
{
    TESSound* sound;
    unsigned int weatherFlags;
    unsigned int chance;
};

// pdb: class SkySound, size 0x1c (here 0xc), 5 members
class SkySound : public BSSoundHandle
{
  public:
    ~SkySound();
};

class RegionDataList
{
  public:
    TESRegionData* Find(int);
};

struct RegionSoundSource
{
    NiAVObject* region;
    TESRegionData* data;
};
struct RegionSoundOffset
{
    float y;
    float x;
};

// pdb: class TaskQueueInterface, size 0x10 (here 0x1), 4 members
class TaskQueueInterface
{
  public:
    static void CopyStringToBuffer(char*, unsigned int, const char*);
};

// pdb: class FalloutAudio, size 0x1, 0 members
class FalloutAudio // src: XBOX_RETAIL_PDB 0x0082d280 (first retail link of 6 high-tier links to its methods; old: SoundPlaybackManager)
{
  public:
    static void UpdateRegionSounds();
    static bool ResolveSoundName(const char*, char*, unsigned int*, void**);
    static void ClearAcousticSpace(BGSAcousticSpace*);
    static bool SoundDistanceTest(TESSound*, NiPoint3, float);
    static void LoadGame(BGSLoadGameBuffer*);
    static void SetAcousticSpace(BGSAcousticSpace*);
    static void ImpactMixer_InitFootsteps();
    static void InitializeSlots();
};
namespace std {
float abs(float);
int abs(int);
}
unsigned int DialogueRandom();
bool RandomBoolean(float);
extern TESDataHandler* g_dataHandler;
extern PlayerCharacter* g_thePlayer;
extern TES* g_TES;
extern bool DAT_011dcfa6, DAT_011dcfa7, DAT_011dcfb0, DAT_011dcfb1;
extern TESSound* DAT_011dcfa8;
extern unsigned int DAT_011dcfac;
extern BGSAcousticSpace* DAT_011dcfb4;
extern BGSAcousticSpace* DAT_011dcfb8;
extern int DAT_011dcfc0;
extern SettingT_GameSettingCollection_ DAT_011dd044, DAT_011dd094, DAT_011dd050, DAT_011dd06c, DAT_011dcfe4;
extern SettingT_GameSettingCollection_ DAT_011f6dc0, DAT_011f6dcc, DAT_011f6dd8, DAT_011f6de4;
extern BSSoundHandle DAT_011dd0a0, DAT_011dd0d8;
extern NiTPointerMap<unsigned int, BSSoundHandle*> DAT_011dd014;

int Warning(const char*, ...);
extern bool DAT_011ccb74;
#endif
