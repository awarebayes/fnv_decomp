#pragma once

#include <stddef.h>

struct _AddonNodeFlags
{
    enum Flags
    {
        IS_MASTER_PARTICLE_SYSTEM = 1u << 0
    };

    bool bIsMasterParticleSystem : 1;
};

namespace _BitfieldInternal
{
    template <class TYPE, class STRUCT>
    class BitfieldBase : public STRUCT
    {
    };
}

class AddonData
{
public:
    unsigned short m_usAddonNodeIndex;
    _BitfieldInternal::BitfieldBase<unsigned char, _AddonNodeFlags> m_ucEntryType;
};

typedef char AddonData_size_check[1];
typedef char AddonData_index_offset_check[1];
typedef char AddonData_entry_type_offset_check[1];
