#pragma once

class NiProperty;
class Property;

// pdb: class NiPropertyState, size 0x1c, 1 members
class NiPropertyState
{
public:
    NiProperty* m_aspProps[7];

    NiPropertyState() throw();
    NiPropertyState& operator=(const NiPropertyState& other);
    static void* operator new(size_t amount);
    void SetSlot(Property* property);
    void NiPropertyState_dtor();
};
