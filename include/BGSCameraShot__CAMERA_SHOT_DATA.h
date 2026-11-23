#pragma once

#include <stddef.h>

enum BGSCameraShot__CAM_ACTION
{
    BGSCameraShot__CAM_ACTION_CA_SHOOT = 0,
    BGSCameraShot__CAM_ACTION_CA_FLY = 1,
    BGSCameraShot__CAM_ACTION_CA_HIT = 2,
    BGSCameraShot__CAM_ACTION_CA_ZOOM = 3,
    BGSCameraShot__CAM_ACTION_CA_COUNT = 4
};

enum BGSCameraShot__CAM_OBJECT
{
    BGSCameraShot__CAM_OBJECT_CO_ATTACKER = 0,
    BGSCameraShot__CAM_OBJECT_CO_PROJECTILE = 1,
    BGSCameraShot__CAM_OBJECT_CO_TARGET = 2,
    BGSCameraShot__CAM_OBJECT_CO_COUNT = 3
};

class BGSCameraShot__CAMERA_SHOT_DATA
{
public:
    void BGSCameraShot_CAMERA_SHOT_DATA_Endian();

    BGSCameraShot__CAM_ACTION m_eAction;
    BGSCameraShot__CAM_OBJECT pInputFile;
    BGSCameraShot__CAM_OBJECT m_eAttacker;
    unsigned int iFlags;
    float m_fMinTime;
    float m_fTargetPercent;
    float m_fTimeMultiplier;
    float fMaxTime;
    float m_fCameraValue1;
    float m_fCameraValue2;
};

typedef char BGSCameraShotCameraShotDataSizeCheck[1];
typedef char BGSCameraShotCameraShotDataActionCheck[1];
typedef char BGSCameraShotCameraShotDataInputFileCheck[1];
typedef char BGSCameraShotCameraShotDataAttackerCheck[1];
typedef char BGSCameraShotCameraShotDataFlagsCheck[1];
typedef char BGSCameraShotCameraShotDataMinTimeCheck[1];
typedef char BGSCameraShotCameraShotDataTargetPercentCheck[1];
typedef char BGSCameraShotCameraShotDataTimeMultiplierCheck[1];
typedef char BGSCameraShotCameraShotDataMaxTimeCheck[1];
typedef char BGSCameraShotCameraShotDataCameraValue1Check[1];
typedef char BGSCameraShotCameraShotDataCameraValue2Check[1];
