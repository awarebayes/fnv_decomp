// Unit-local C++03 declarations. Layout evidence is itemized in NOTES.md.
#ifndef FNV_THREAD_SAFE_STRUCTURES_MATCH_H
#define FNV_THREAD_SAFE_STRUCTURES_MATCH_H

#include <stddef.h>
extern "C" void* __cdecl memcpy(void*, const void*, size_t);
extern "C" const char kThreadSafeStructuresSourceFile[];
unsigned int __cdecl _TlsAlloc();
class bhkRigidBody;
class QueuedReference;
class TESObjectREFR;
class AnimIdle;
class QueuedAnimIdle;
class Animation;
class QueuedReplacementKFList;
class QueuedHelmet;
class Model;
class ExteriorCellLoaderTask;
#include "../../include/MEM_CONTEXT.h"
template <class T> class ThreadInterfaceManager
{
  public:
    struct ThreadInterfaceEntry
    {
        ThreadInterfaceEntry();
        unsigned int uiThreadID;
        T* pInterface;
    };
    unsigned int uiMaxThreads;
    unsigned int uiTLSIndex;
    ThreadInterfaceEntry* m_pThreadInterfaceEntries;
    unsigned int uiThreadCount;
    ThreadInterfaceManager(unsigned int maxThreads);
};
#include "../../include/LockFreeQueue.h"


typedef LockFreeQueue<bhkRigidBody*> RigidBodyQueue;
typedef LockFreeMap<unsigned int, NiPointer<ExteriorCellLoaderTask> > TreeMap;
typedef char CheckSpinLockSize[1];
typedef char CheckQueueSize[1];
typedef char CheckQueueLock[1];
typedef char CheckMapSize[1];
typedef char CheckMapLock[1];
typedef char CheckMapInterface[1];
#endif
