#pragma once

#include <stddef.h>

enum TESGrass__GRASS_WATER_STATE
{
    TESGrass__GRASS_WATER_STATE_GWS_ABOVE_ONLY_ATLEAST = 0,
    TESGrass__GRASS_WATER_STATE_GWS_ABOVE_ONLY_ATMOST = 1,
    TESGrass__GRASS_WATER_STATE_GWS_BELOW_ONLY_ATLEAST = 2,
    TESGrass__GRASS_WATER_STATE_GWS_BELOW_ONLY_ATMOST = 3,
    TESGrass__GRASS_WATER_STATE_GWS_BOTH_ATLEAST = 4,
    TESGrass__GRASS_WATER_STATE_GWS_BOTH_ATMOST = 5,
    TESGrass__GRASS_WATER_STATE_GWS_BOTH_ATMOST_ABOVE = 6,
    TESGrass__GRASS_WATER_STATE_GWS_BOTH_ATMOST_BELOW = 7,
    TESGrass__GRASS_WATER_STATE_GWS_COUNT = 8
};

class TESGrass__GRASS_DATA
{
public:
    void TESGrass_GRASS_DATA_Endian();

    unsigned char m_ucDensity;
    unsigned char m_ucMinSlope;
    unsigned char m_ucMaxSlope;
    unsigned char layoutPadding3[1];
    unsigned short m_usUnitsFromSurface;
    unsigned char layoutPadding6[2];
    TESGrass__GRASS_WATER_STATE m_eWaterState;
    float m_fPositionRange;
    float m_fHeightRange;
    float m_fWavePeriod;
    float m_fWavePhase;
    char cFlags;
    unsigned char layoutPadding1D[3];
};

typedef char TESGrass__GRASS_DATA_size_check[1];
typedef char TESGrass__GRASS_DATA_density_check[1];
typedef char TESGrass__GRASS_DATA_min_slope_check[1];
typedef char TESGrass__GRASS_DATA_max_slope_check[1];
typedef char TESGrass__GRASS_DATA_padding3_check[1];
typedef char TESGrass__GRASS_DATA_units_check[1];
typedef char TESGrass__GRASS_DATA_padding6_check[1];
typedef char TESGrass__GRASS_DATA_water_state_check[1];
typedef char TESGrass__GRASS_DATA_position_range_check[1];
typedef char TESGrass__GRASS_DATA_height_range_check[1];
typedef char TESGrass__GRASS_DATA_wave_period_check[1];
typedef char TESGrass__GRASS_DATA_wave_phase_check[1];
typedef char TESGrass__GRASS_DATA_flags_check[1];
typedef char TESGrass__GRASS_DATA_padding1D_check[1];
