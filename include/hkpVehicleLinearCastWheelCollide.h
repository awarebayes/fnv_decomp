#pragma once

#include "hkpVehicleWheelCollide.h"
#include "hkaAnimation.h"
#include "hkArray.h"
#include "hkAabb.h"
#include "hkpRejectChassisListener.h"

class hkpVehicleLinearCastWheelCollideWheelState;
class hkpAabbPhantom;
class hkpCollisionFilter;
class hkpCollidable;
class hkpPairLinearCastCommand;
class hkpPhantom;
class hkpRootCdPoint;
class hkpShape;
class hkpWorld;
struct LCHit;
struct LCWheelCastOut;

class hkpCollidable;

typedef unsigned int hkUint32;

// pdb: class hkpVehicleLinearCastWheelCollide, size 0x34, bases: hkpVehicleWheelCollide, 5 members
class hkpVehicleLinearCastWheelCollide : public hkpVehicleWheelCollide
{
public:
    hkpVehicleLinearCastWheelCollide() throw();
    virtual ~hkpVehicleLinearCastWheelCollide() throw();

    virtual int GetTotalHits() const;
    virtual int getNumHits(unsigned char ucWheel) const;
    virtual int buildLinearCastCommands(const hkpVehicleInstance* vehicle, const hkpCollisionFilter* collisionFilter,
                            hkpCollidable* collidableStorage, hkpPairLinearCastCommand* commandStorage,
                            hkpRootCdPoint* outputStorage) const throw();
    virtual hkContactPoint* determineNearestHit(int numWheel, LCHit* commandStorageOutput);
    virtual char* castQuery(char* found, hkpVehicleInstance* vehicle, unsigned char iWheel,
                            LCWheelCastOut* out);
    virtual void applyHit(hkpVehicleInstance* vehicle, unsigned char iWheel,
                           LCWheelCastOut* out, WheelCollideResult* results);
    virtual void noHit(hkpVehicleInstance* vehicle, unsigned char iWheel, WheelCollideResult* results);

    static void* operator new(size_t uiSize) throw();
    static void operator delete(void* p) throw();

    void InitializeWheelCollisions(const hkpVehicleInstance* vehicle) throw();
    void UpdateAllWheels(int deltaTime, hkpVehicleInstance* vehicle, WheelCollideResult* cdInfoOut);
    void getPhantoms(hkArray<hkpPhantom*>* phantomsOut);
    void UpdateWheelAabb(unsigned char vehicle);
    hkpVehicleLinearCastWheelCollide* CloneWithChassis(const hkpRigidBody& newChassis,
                                                      const hkArray<hkpPhantom*>& newPhantoms) const throw();
    void addToWorld_00e36870(hkpWorld* world);
    void removeFromWorld();
    void SetTargetArg(void* arg);
    void centerWheelContactPoint(int vehicle, unsigned char wheelIndex, hkVector4* cdInfo);
    int getNumHits(int iWheel);
    unsigned int getNumCommands(unsigned char numWheel);
    void CastWheel(int hitOut, hkUint32 unused, unsigned char wheelIndex, int context);
    void setWheelShapes(const hkpVehicleInstance* vehicle,
                          const hkArray<hkReferencedObject*>& shapes);
    hkpShape* hkpVehicleLinearCastWheelCollide_createWheelShape(hkReal width, hkReal radius) throw();
    void hkpVehicleLinearCastWheelCollide_calcAabbOfWheel(const hkpVehicleInstance* vehicle, unsigned char wheelNum,
                      hkAabb& aabbOut) throw();
    void hkpVehicleLinearCastWheelCollide_updateWheelState(const hkpVehicleInstance* vehicle, unsigned char wheelIndex) throw();
    void hkpVehicleLinearCastWheelCollide_calcAabbOfWheel(unsigned char wheelIndex, unsigned char wheelNum, hkAabb* aabbOut);
    void hkpVehicleLinearCastWheelCollide_updateWheelState(unsigned char wheelIndex, unsigned char stateIndex);

    hkUint32 m_wheelCollisionFilterInfo; // src: XBOX_RETAIL_PDB +0xc (old: m_uiWheelCount)
    hkArray<hkpVehicleLinearCastWheelCollideWheelState> m_wheelStates; // src: XBOX_RETAIL_PDB +0x10 (old: m_kWheelStates)
    hkpRejectChassisListener m_rejectChassisListener; // src: XBOX_RETAIL_PDB +0x1c (old: m_kRejectChassisListener)
    hkReal m_maxExtraPenetration; // src: XBOX_RETAIL_PDB +0x2c (old: m_fMaxExtraPenetration)
    hkReal m_startPointTolerance; // src: XBOX_RETAIL_PDB +0x30 (old: m_fMaxExtraPenetrationAgainstFixed)
};

