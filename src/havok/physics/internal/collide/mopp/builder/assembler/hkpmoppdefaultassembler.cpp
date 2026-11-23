#include "../../../../../../../../include/hkArray.h"

#include "../../../../../../../../include/hkMemoryRouter.h"
#include "../../../../../../../../include/hkMemoryAllocator.h"
#include "../../../../../../../../include/Pair.h"
#include "../../../../../../../../include/hkMemoryRouter.h"
#include "../../../../../../../../include/hkMemoryRouter.h"
#include "../../../../../../../../include/NiPoint3.h"


inline Pair::Pair() throw()
{}

inline void* Pair::operator new(size_t, void* p) throw()
{
    return p;
}

// src/havok/physics/internal/collide/mopp/builder/assembler/hkpmoppdefaultassembler.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/build/20091222_000100_aianbecldeph/source/physics/internal/collide/mopp/builder/assembler/hkpmoppdefaultassembler.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

extern "C" void __cdecl GrowBuffer();

struct FUN_00e06780_class
{
    void __thiscall AppendChar(char b);
};

// Writes character b at the current write position, advances the offset, and calls GrowBuffer when the offset reaches the capacity.
// @fnv 0x00e06780
void __thiscall FUN_00e06780_class::AppendChar(char acValue)
{
    char* p = *(char**)((char*)this + 0x10) - *(int*)((char*)this + 0xc);
    int base = *(int*)((char*)this + 8);
    *(p + base - 1) = acValue;
    ++*(int*)((char*)this + 0xc);
    if (*(int*)((char*)this + 0xc) >= *(int*)((char*)this + 8))
        GrowBuffer();
}

struct S_00e066f0_Src
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int m_pRayStart;
    int m_kRayEnd;
    int m_uiItemCount;
    int m_offsets[8];
};

struct S_00e066f0
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int m_iElementCount;
    int m_uiItemCount;
    int m_entries[8];

    void CopyOffsets(const S_00e066f0_Src* src);
};

// @fnv 0x00e066f0
void S_00e066f0::CopyOffsets(const S_00e066f0_Src* src)
{
    m_iElementCount = src->m_kRayEnd - src->m_pRayStart;
    m_uiItemCount = src->m_uiItemCount;
    for (int i = 0; i < src->m_uiItemCount; i++)
    {
        m_entries[i] = src->m_offsets[i];
        m_entries[i + 1] = src->m_offsets[i + 1] - src->m_offsets[i];
    }
}

struct AlignRange
{
    volatile int m_iMinControlPoint;
    volatile int m_iMaxControlPoint;
};

struct FUN_00e06730_cls
{
    int unk00;
    int unk04;
    int unk08;
    AlignRange m_akRange[3];
    volatile int m_iShift;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;
    volatile int m_aiAlignedMin[3];
    volatile int m_aiAlignedMax[3];
    void AlignRanges();
};

// @fnv 0x00e06730
void FUN_00e06730_cls::AlignRanges()
{
    int iShift0 = m_iShift;
    int iMax0 = ((m_akRange[0].m_iMaxControlPoint >> iShift0) + 1) << iShift0;
    int iMin0 = (m_akRange[0].m_iMinControlPoint >> iShift0) << iShift0;
    m_aiAlignedMax[0] = iMax0;
    m_aiAlignedMin[0] = iMin0;

    int iShift1 = m_iShift;
    int iMax1 = ((m_akRange[1].m_iMaxControlPoint >> iShift1) + 1) << iShift1;
    int iMin1 = (m_akRange[1].m_iMinControlPoint >> iShift1) << iShift1;
    m_aiAlignedMin[1] = iMin1;
    m_aiAlignedMax[1] = iMax1;

    int iShift2 = m_iShift;
    int iMax2 = ((m_akRange[2].m_iMaxControlPoint >> iShift2) + 1) << iShift2;
    int iMin2 = (m_akRange[2].m_iMinControlPoint >> iShift2) << iShift2;
    m_aiAlignedMin[2] = iMin2;
    m_aiAlignedMax[2] = iMax2;
}

struct ByteSink
{
    int unk00;
    int unk04;
    int m_uiLogicalSize;
    int m_iUsed;
    char* m_pBuffer;

    void GrowBuffer();

    void PutByte(char acValue)
    {
        m_pBuffer[m_uiLogicalSize - m_iUsed - 1] = acValue;
        m_iUsed++;
        if (m_iUsed >= m_uiLogicalSize)
            GrowBuffer();
    }

    void PutBytePair(char code, char param);
};

// Writes acSecond then acFirst to the sink via PutByte, in that order.
// @fnv 0x00e067b0
// pdb: hkpMoppCodeGenerator::addCommand8 | hkpMoppDefaultAssembler.obj | retail, call graph, high
// pdb params: code, param
void ByteSink::PutBytePair(char code, char param)
{
    PutByte(param);
    PutByte(code);
}

typedef void(__thiscall* FlushFunction)(void*);
struct BufferWriter
{
    int unknown00;
    int unknown04;
    int capacity;
    int position;
    unsigned char* data;
    void hkpMoppCodeGenerator_addParam24(int param);
};
// Writes the low three bytes of value into the buffer from the end, advancing position and flushing when it reaches capacity.
// @fnv 0x00e06920
// src: XBOX_RETAIL_PDB 0x00e06920
// pdb: hkpMoppCodeGenerator::addParam24 | hkpMoppDefaultAssembler.obj | retail, call graph, high
// pdb params: param
void BufferWriter::hkpMoppCodeGenerator_addParam24(int param)
{
    data[capacity - position - 1] = (unsigned char)param;
    ++position;
    if (position >= capacity)
        ((FlushFunction)GrowBuffer)(this);
    data[capacity - position - 1] = (unsigned char)(param >> 8);
    ++position;
    if (position >= capacity)
        ((FlushFunction)GrowBuffer)(this);
    data[capacity - position - 1] = (unsigned char)(param >> 16);
    ++position;
    if (position >= capacity)
        ((FlushFunction)GrowBuffer)(this);
}

struct S_00e06aa0_Pool
{
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void Free(void* p);
};
struct S_00e06aa0_Item
{
    int unk00;
    bool m_flag;
    char unk05;
    char unk06;
    char unk07;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    int unk58;
    int unk5C;
    int unk60;
    int unk64;
    int unk68;
    int unk6C;
    void* m_p70;
    void* m_p74;
};
struct S_00e06aa0
{
    int unk00;
    int unk04;
    int unk08;
    S_00e06aa0_Pool* m_kNodePool;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    int unk58;
    int unk5C;
    int unk60;
    int unk64;
    int unk68;
    int unk6C;
    int unk70;
    bool m_enabled;
    void hkpMoppDefaultAssembler_releaseNode(S_00e06aa0_Item* node);
};
// If the item is not flagged and this object is enabled, frees the item's two pointers through the pool and nulls them.
// @fnv 0x00e06aa0
// src: HAVOK_LIB 0x00e06aa0
// pdb: hkpMoppDefaultAssembler::releaseNode | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: node
void S_00e06aa0::hkpMoppDefaultAssembler_releaseNode(S_00e06aa0_Item* node)
{
    if (!node->m_flag && m_enabled)
    {
        m_kNodePool->Free(node->m_p70);
        m_kNodePool->Free(node->m_p74);
        node->m_p70 = 0;
        node->m_p74 = 0;
    }
}

struct Node
{
    Node* next;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    char unk38;
    char unk39;
};
// Walks the linked list from node, marking each node's unk39 flag until one is already set or the list ends.
// @fnv 0x00e06b20
// src: HAVOK_LIB 0x00e06b20
// pdb: hkpMoppTreeNode::toTerminal | hkpMoppDefaultAssembler.obj | debug, call graph, medium
void __stdcall hkpMoppDefaultAssembler_fixScale(Node* node)
{
    while (node)
    {
        if (node->unk39)
            return;
        node->unk39 = 1;
        node = node->next;
    }
}

struct Limits
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0c;
    unsigned int unk10;
    unsigned int unk14;
    unsigned int unk18;
    unsigned int unk1c;
    unsigned int unk20;
    unsigned int unk24;
    unsigned int unk28;
    unsigned int unk2c;
    unsigned int unk30;
    unsigned int unk34;
    unsigned int unk38;
};
// Copies second->unk34 into third->unk34, then compares unk38/unk24 thresholds and may overwrite third->unk34 with first->unk24.
// @fnv 0x00e06b50
void __stdcall AdjustLimits(Limits* first, Limits* second, Limits* third)
{
    third->unk34 = second->unk34;
    unsigned int value = third->unk38 - second->unk34 + first->unk24;
    if (value < 0x20)
        return;
    if (third->unk38 <= 2 && value < 0x100)
        return;
    if (third->unk38 < 0x20 && second->unk38 >= 0x20)
    {
        third->unk34 = first->unk24;
        return;
    }
    if (third->unk38 < 0x100 && second->unk38 >= 0x100)
    {
        third->unk34 = first->unk24;
        return;
    }
    if (third->unk38 >= 0x10000)
        return;
    if (second->unk38 >= 0x10000)
        third->unk34 = first->unk24;
}

struct E06C20Writer
{
    void hkpMoppCodeGenerator_addCommand32(int code, void* param);
};

struct E06C20Object
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    E06C20Writer* m_pStreamWriter;

    int hkpMoppDefaultAssembler_addJumpChunk(void* chunkId);
};

// @fnv 0x00e06c20
// src: HAVOK_LIB 0x00e06c20
// pdb: hkpMoppDefaultAssembler::addJumpChunk | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: chunkId
int E06C20Object::hkpMoppDefaultAssembler_addJumpChunk(void* chunkId)
{
    m_pStreamWriter->hkpMoppCodeGenerator_addCommand32(0x70, chunkId);
    return 5;
}

namespace fnv_00e06db0 {
struct Node
{
    char pad[0x40];
    void* m_40;
    int m_44;
};

struct FUN_00e06db0_class
{
    __declspec(nothrow) void WriteOp(int a, void* p);
    void hkpMoppDefaultAssembler_checkAndAddProperties(int* flag, Node* b, Node* c);
};

// If *flag is set, calls WriteOp(0, c->m_40) when c->m_44 is 0 and b->m_44 is nonzero; otherwise when c->m_44 is 0 and c->m_40 is set.
// @fnv 0x00e06db0
// src: HAVOK_LIB 0x00e06db0
void FUN_00e06db0_class::hkpMoppDefaultAssembler_checkAndAddProperties(int* piFlag, Node* pNodeB, Node* pNodeC)
{
    if (*piFlag)
    {
        if (pNodeC->m_44 == 0 && pNodeB->m_44 != 0)
            WriteOp(0, pNodeC->m_40);
    }
    else
    {
        if (pNodeC->m_44 == 0 && pNodeC->m_40)
            WriteOp(0, pNodeC->m_40);
    }
}
}  // namespace fnv_00e06db0

struct S_00e07040_A
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
};
struct S_00e07040_B
{
    int unk00;
    int unk04;
    int unk08;
};
template <class T> inline T S_00e07040_Max(T a, T b)
{
    return a > b ? a : b;
}
// Returns the smaller of two values.
// @fnv 0x00e07040
// pdb: hkpMoppDefaultAssembler::calcRescaleBits | hkpMoppDefaultAssembler.obj | debug, upstream name already matches, high
// pdb params: parentInfo, currentInfo
template <class T> inline T S_00e07040_Min(T a, T b)
{
    return a < b ? a : b;
}
// src: HAVOK_LIB 0x00e07040
int __stdcall hkpMoppDefaultAssembler_calcRescaleBits(S_00e07040_A* parentInfo, S_00e07040_B* currentInfo)
{
    int n = currentInfo->unk08 - 8;
    if (n <= 0)
        n = 0;
    int r = parentInfo->unk24 - n;
    if (r > parentInfo->unk24)
        r = parentInfo->unk24;
    return r;
}

struct E07510Input
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    unsigned char unk38;
    unsigned char unk39;
    unsigned char m_type;
};

struct E07510Solver
{
    void hkpMoppDefaultAssembler_recalcRescale(void* parent, void* info, void* cmd);
    void AdjustLimits(E07510Input* a, void* b, void* c);
    void hkpMoppDefaultAssembler_initTopDown(E07510Input* currentNode, void* parentInfo, void* currentInfo, void* rescaleCommandInfo);
};

// If the input's m_type is 1, calls FUN_00e07070 with b, c and d, then always calls AdjustLimits with a, b and c.
// @fnv 0x00e07510
// src: HAVOK_LIB 0x00e07510
// pdb: hkpMoppDefaultAssembler::initTopDown | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: currentNode, parentInfo, currentInfo, rescaleCommandInfo
void E07510Solver::hkpMoppDefaultAssembler_initTopDown(E07510Input* currentNode, void* parentInfo, void* currentInfo, void* rescaleCommandInfo)
{
    if (currentNode->m_type == 1)
        hkpMoppDefaultAssembler_recalcRescale(parentInfo, currentInfo, rescaleCommandInfo);
    AdjustLimits(currentNode, parentInfo, currentInfo);
}

struct S_e07c50_Buf
{
    int unk0;
    int unk4;
    int unk8;
    int m_iElementCount;
};
struct S_e07c50_Item;
struct S_e07c50
{
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    S_e07c50_Buf* m_buf;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int m_uiItemCount;
    void hkpMoppDefaultAssembler_fixScale(S_e07c50_Item* item);
    void hkpMoppDefaultAssembler_addTerminals(S_e07c50_Item* thisNode, int parentInfo, int currentInfo);
    void hkpMoppDefaultAssembler_addCut(S_e07c50_Item* currentNode, int currentInfo);
    int hkpMoppDefaultAssembler_assembleCutAndTerminalCommand(S_e07c50_Item* currentNode, int parentInfo, int currentInfo);
};
struct S_e07c50_Item
{
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    char m_flag38;
    char m_flag39;
    char unk3A;
    char unk3B;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    int unk58;
    int m_kRayEnd;
};
// @fnv 0x00e07c50
// src: HAVOK_LIB 0x00e07c50
// pdb: hkpMoppDefaultAssembler::assembleCutAndTerminalCommand | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: currentNode, parentInfo, currentInfo
int S_e07c50::hkpMoppDefaultAssembler_assembleCutAndTerminalCommand(S_e07c50_Item* currentNode, int parentInfo, int currentInfo)
{
    hkpMoppDefaultAssembler_fixScale(currentNode);
    m_uiItemCount++;
    int start = m_buf->m_iElementCount;
    hkpMoppDefaultAssembler_addTerminals(currentNode, parentInfo, currentInfo);
    hkpMoppDefaultAssembler_addCut(currentNode, currentInfo);
    currentNode->m_kRayEnd = m_buf->m_iElementCount;
    int r = m_buf->m_iElementCount - start;
    currentNode->m_flag38 = 1;
    return r;
}

namespace fnv_00e08040 {
struct Node
{
    int unk00;
    unsigned char leaf;
    int unk08, unk0C, unk10, unk14, unk18, unk1C, unk20, unk24, unk28, unk2C, unk30, unk34;
    unsigned char complete;
    int unk3C, unk40, unk44, unk48, unk4C, unk50, unk54, unk58, unk5C, unk60, unk64, unk68, unk6C;
    Node* left;
    Node* right;
};
struct __declspec(align(8)) State
{
    int depth;
    bool retrying;
    int unk08, unk0C, unk10, unk14, unk18, unk1C, unk20, unk24, unk28, unk2C, unk30, unk34, unk38, unk3C, unk40, unk44;
    State()
    {}
    State(const State&, Node*, float*);
    void hkpMoppAssemblerNodeInfo_ctor(const State*, Node*, float*);
};
struct MergeInfo
{
    bool changed;
    int index;
    int unk08, unk0C, unk10;
};
struct CheckResult
{
    bool value;
    CheckResult(bool v) : value(v)
    {}
    operator bool() const
    {
        return value;
    }
};
struct Tracker
{
    int unk00, unk04, unk08, count;
};
void __stdcall hkpMoppDefaultAssembler_fixScale(Node*);
void __stdcall hkpMoppDefaultAssembler_initTopDown(Node*, void*, State*, MergeInfo*);
struct TreeOptimizer
{
    int unk00, unk04, unk08, unk0C;
    Tracker* tracker;
    int unk14, threshold, unk1C, unk20, limit, count, unk2C;
    float config30, config34, config38, config3C, config40, config44, config48, config4C, config50, config54, config58,
        config5C, config60, config64, config68, config6C;
    int period;
    CheckResult hkpMoppDefaultAssembler_shouldAssemble(Node*, State*);
    int hkpMoppDefaultAssembler_assembleCutAndTerminalCommand(Node*, void*, State*);
    void hkpMoppDefaultAssembler_releaseNode(Node*);
    int hkpMoppDefaultAssembler_assembleNonTerminalCommand(Node*, void*, State*, MergeInfo*);
    int OptimizeTree(Node*, void*, State*);
};
// Recursively optimizes a node tree, retrying with a reduced threshold, merging child results and returning a combined score or -1.
// @fnv 0x00e08040
// pdb: hkpMoppDefaultAssembler::assemblesubNode | hkpMoppDefaultAssembler.obj | retail, object-file run extension, low
// pdb params: currentNode, parentInfo, currentInfo
// pdb locals: rescaleCommandInfo, childInfo
int TreeOptimizer::OptimizeTree(Node* node, void* context, State* state)
{
    if (node->complete)
        return 3;
    if (state->depth <= threshold && state->depth % period == 0 && !state->retrying)
    {
        state->retrying = true;
        int result = -1;
        int savedThreshold = threshold;
        while (!node->complete)
        {
            int previousCount = tracker->count;
            State stateCopy = *state;
            result = OptimizeTree(node, context, &stateCopy);
            if (count > limit)
                break;
            if (previousCount == tracker->count)
            {
                if (threshold < 0)
                    break;
                threshold -= period;
            }
        }
        threshold = savedThreshold;
        state->retrying = false;
        return result;
    }
    if (node->leaf)
    {
        hkpMoppDefaultAssembler_fixScale(node);
        if (hkpMoppDefaultAssembler_shouldAssemble(node, state))
            return hkpMoppDefaultAssembler_assembleCutAndTerminalCommand(node, context, state);
    }
    else
    {
        MergeInfo info;
        info.changed = false;
        info.index = -1;
        hkpMoppDefaultAssembler_initTopDown(node, context, state, &info);
        int score = -1;
        {

            Node* rightChild = node->right;
            if (rightChild)
            {
                State stateCopy;
                stateCopy.hkpMoppAssemblerNodeInfo_ctor(state, rightChild, &config30);
                score = OptimizeTree(rightChild, state, &stateCopy);
                if (score >= 0)
                    hkpMoppDefaultAssembler_releaseNode(rightChild);
            }
            Node* leftChild = node->left;
            if (leftChild)
            {
                State stateCopy;
                stateCopy.hkpMoppAssemblerNodeInfo_ctor(state, leftChild, &config30);
                int leftResult = OptimizeTree(leftChild, state, &stateCopy);
                if (leftResult >= 0)
                {
                    hkpMoppDefaultAssembler_releaseNode(leftChild);
                    if (score >= 0 && hkpMoppDefaultAssembler_shouldAssemble(node, state))
                    {
                        ++count;
                        return hkpMoppDefaultAssembler_assembleNonTerminalCommand(node, context, state, &info) + leftResult + score;
                    }
                }
            }
        }
    }
    return -1;
}
}  // namespace fnv_00e08040

struct Allocator
{
    virtual void* unk00(void* p);
    virtual void* unk04(void* p);
    virtual void __thiscall ReleaseBlock(void* p, unsigned short size);
};
extern "C" hkMemoryRouter* __cdecl hkMemoryRouter_getInstance();
struct FUN_00e09640_class
{
    virtual void unknownVirtual();
    unsigned short unk04;
    void __thiscall DestroyAudioBuffer();
    FUN_00e09640_class* __thiscall FUN_00e09640(int flags);
};
// @fnv 0x00e09640
// pdb: hkpMoppDefaultAssembler::`scalar deleting destructor' | hkpMoppDefaultAssembler.obj | retail, vtable slot, high
FUN_00e09640_class* __thiscall FUN_00e09640_class::FUN_00e09640(int flags)
{
    DestroyAudioBuffer();
    if (flags & 1)
    {
        hkMemoryRouter* router = hkMemoryRouter_getInstance();
        ((Allocator*)router->m_heap)->ReleaseBlock(this, unk04);
    }
    return this;
}

struct FUN_00e06800_cls
{
    int unk00;
    int unk04;
    int m_cap;
    int m_uiItemCount;
    char* m_buf;

    void __thiscall Flush();
    void __thiscall hkpMoppCodeGenerator_addCommand16(char code, int param);
};

// @fnv 0x00e06800
// src: HAVOK_LIB 0x00e06800
// pdb: hkpMoppCodeGenerator::addCommand16 | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: code, param
void __thiscall FUN_00e06800_cls::hkpMoppCodeGenerator_addCommand16(char code, int param)
{
    m_buf[m_cap - m_uiItemCount - 1] = (char)param;
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();

    param = param >> 8;
    m_buf[m_cap - m_uiItemCount - 1] = (char)param;
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();

    m_buf[m_cap - m_uiItemCount - 1] = code;
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();
}

struct FUN_00e06880_cls
{
    int unk00;
    int unk04;
    int m_cap;
    int m_uiItemCount;
    char* m_buf;

    void __thiscall Flush();
    void __thiscall hkpMoppCodeGenerator_addCommand24(char code, int param);
};

// Writes the low three bytes of v and then a into the buffer from the end, flushing whenever the buffer fills.
// @fnv 0x00e06880
// src: HAVOK_LIB 0x00e06880
// pdb: hkpMoppCodeGenerator::addCommand24 | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: code, param
void __thiscall FUN_00e06880_cls::hkpMoppCodeGenerator_addCommand24(char code, int param)
{
    m_buf[m_cap - m_uiItemCount - 1] = (char)param;
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();

    m_buf[m_cap - m_uiItemCount - 1] = (char)(param >> 8);
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();

    m_buf[m_cap - m_uiItemCount - 1] = (char)(param >> 16);
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();

    m_buf[m_cap - m_uiItemCount - 1] = code;
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();
}

struct FUN_00e069a0_cls
{
    int unk00;
    int unk04;
    int m_cap;
    int m_uiItemCount;
    char* m_buf;

    void __thiscall Flush();
    void __thiscall hkpMoppCodeGenerator_addCommand32(char code, unsigned int param);
};

// Writes the four bytes of v and then a into m_buf from the end backwards, flushing whenever the buffer fills.
// @fnv 0x00e069a0
// src: HAVOK_LIB 0x00e069a0
// pdb: hkpMoppCodeGenerator::addCommand32 | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: code, param
void __thiscall FUN_00e069a0_cls::hkpMoppCodeGenerator_addCommand32(char code, unsigned int param)
{
    m_buf[m_cap - m_uiItemCount - 1] = (char)param;
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();

    m_buf[m_cap - m_uiItemCount - 1] = (char)(param >> 8);
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();

    m_buf[m_cap - m_uiItemCount - 1] = (char)(param >> 16);
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();

    m_buf[m_cap - m_uiItemCount - 1] = (char)(param >> 24);
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();

    m_buf[m_cap - m_uiItemCount - 1] = code;
    m_uiItemCount++;
    if (m_uiItemCount >= m_cap)
        Flush();
}

struct FUN_00e06490_cls
{
    void __thiscall GrowBuffer();
};

struct FUN_00e06a60_cls
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;

    void __thiscall hkpMoppCodeGenerator_pad(int size);
};

// @fnv 0x00e06a60
// src: XBOX_RETAIL_PDB 0x00e06a60
// pdb: hkpMoppCodeGenerator::pad | hkpMoppDefaultAssembler.obj | retail, object-file gap fill, high
// pdb params: size
void __thiscall FUN_00e06a60_cls::hkpMoppCodeGenerator_pad(int size)
{
    if (size <= 0)
        return;
    int n = size;
    do
    {
        char* base = (char*)unk08;
        base[unk10 - unk0c - 1] = (char)0xcd;
        unk0c++;
        if (unk0c >= unk08)
        {
            ((FUN_00e06490_cls*)this)->GrowBuffer();
        }
        n--;
    } while (n != 0);
}

// FUN_00e06af0 - a small __thiscall member that fills in a result flag.
//
// The original code is exactly:
//     00e06af0  8b 44 24 08        mov eax, dword ptr [esp + 8]
//     00e06af4  83 78 08 16        cmp dword ptr [eax + 8], 16h
//     00e06af8  7d 0a              jge 0x00e06b04
//     00e06afa  8b 44 24 04        mov eax, dword ptr [esp + 4]
//     00e06afe  c6 00 01           mov byte ptr [eax], 1
//     00e06b01  c2 0c 00           ret 0ch
//     00e06b04  8b 54 24 0c        mov edx, dword ptr [esp + 0ch]
//     00e06b08  8b 02              mov eax, dword ptr [edx]
//     00e06b0a  3b 41 18           cmp eax, dword ptr [ecx + 18h]
//     00e06b0d  8b 44 24 04        mov eax, dword ptr [esp + 4]
//     00e06b11  7f eb              jg 0x00e06afe
//     00e06b13  c6 00 00           mov byte ptr [eax], 0
//     00e06b16  c2 0c 00           ret 0ch
//
// so the member is
//     char* FUN_00e06af0(char* out, Actor* actor, int* value)
// and it returns the flag pointer it was given:
//
//   * if actor->unk08 is below 0x16 the flag is set to 1 unconditionally;
//   * otherwise the flag is 1 when *value is greater than the member
//     field at +0x18 and 0 when it is not.
//
// The "return out;" in every branch matters for the code generation: the
// returned pointer is already in EAX, and because the branches are not
// plain assignments MSVC 2008 keeps the two stores as real branches
// instead of folding them into a single conditional store.

// The "actor" the first flag argument points at.
struct FUN_00e06780_actor
{
    int unk00;
    int unk04;
    int unk08;
};

// The class the member belongs to.  The unit's FUN_00e06780_class carries
// no data members, so the flag field lives in the derived stand-in.
struct FUN_00e06780_owner : public FUN_00e06780_class
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;  // +0x18
    char* __thiscall hkpMoppDefaultAssembler_shouldAssemble(char* out, FUN_00e06780_actor* actor, int* value);
};

// Writes 1 to out if actor->unk08 is below 0x16 or *value exceeds this->unk18, otherwise writes 0, and returns out.
// @fnv 0x00e06af0
// src: HAVOK_LIB 0x00e06af0
char* FUN_00e06780_owner::hkpMoppDefaultAssembler_shouldAssemble(char* pResult, FUN_00e06780_actor* actor, int* piValue)
{
    if (actor->unk08 < 0x16)
    {
        *pResult = 1;
        return pResult;
    }
    else
    {
        if (*piValue > this->unk18)
        {
            *pResult = 1;
            return pResult;
        }
        *pResult = 0;
        return pResult;
    }
}

struct Stream
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    char* unk10;

    void GrowBuffer();

    void AppendChar(char b)
    {
        unk10[unk08 - unk0c - 1] = b;
        unk0c++;
        if (unk0c >= unk08)
        {
            GrowBuffer();
        }
    }

    void hkpMoppCodeGenerator_addCommand16(int code, int param);
};

struct FUN_00e06c40_class
{
    char pad[0x10];
    Stream* m_stream;

    int __thiscall hkpMoppDefaultAssembler_addDataOffset(int offset, int numTerminals);
};

// @fnv 0x00e06c40
// src: HAVOK_LIB 0x00e06c40
// pdb: hkpMoppDefaultAssembler::addDataOffset | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: offset, numTerminals
int __thiscall FUN_00e06c40_class::hkpMoppDefaultAssembler_addDataOffset(int offset, int numTerminals)
{
    m_stream->AppendChar((char)numTerminals);
    m_stream->AppendChar((char)(numTerminals >> 8));
    m_stream->hkpMoppCodeGenerator_addCommand16(0xd, offset);
    return 5;
}

extern "C" void __cdecl GrowBuffer();

struct Sink
{
    int unk00;
    int unk04;
    int m_iCap;
    int m_iArrayLength;
    int m_iEnd;
    void __thiscall Flush();
};

struct Pkt
{
    int unk00;
    char m_b04;
    char unk05;
    char unk06;
    char unk07;
    char m_b08;
    char unk09;
    char unk0a;
    char unk0b;
    char m_b0c;
    char unk0d;
    char unk0e;
    char unk0f;
    char m_b10;
};

class FUN_00e06cb0_cls
{
  public:
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    Sink* m_pSink;

    void __thiscall hkpMoppDefaultAssembler_addRescale(const Pkt* cmd);
};

// Writes four bytes of the packet (m_b10, m_b0c, m_b08, m_b04) into the sink buffer, flushing when it fills.
// @fnv 0x00e06cb0
// src: XBOX_RETAIL_PDB 0x00e06cb0
// pdb: hkpMoppDefaultAssembler::addRescale | hkpMoppDefaultAssembler.obj | retail, call graph, high
// pdb params: cmd
void __thiscall FUN_00e06cb0_cls::hkpMoppDefaultAssembler_addRescale(const Pkt* cmd)
{
    {
        Sink* s = m_pSink;
        char v = cmd->m_b10;
        ((char*)s->m_iCap)[s->m_iEnd - s->m_iArrayLength - 1] = v;
        s->m_iArrayLength++;
        if (s->m_iArrayLength >= s->m_iCap)
            s->Flush();
    }
    {
        Sink* s = m_pSink;
        char v = cmd->m_b0c;
        ((char*)s->m_iCap)[s->m_iEnd - s->m_iArrayLength - 1] = v;
        s->m_iArrayLength++;
        if (s->m_iArrayLength >= s->m_iCap)
            s->Flush();
    }
    {
        Sink* s = m_pSink;
        char v = cmd->m_b08;
        ((char*)s->m_iCap)[s->m_iEnd - s->m_iArrayLength - 1] = v;
        s->m_iArrayLength++;
        if (s->m_iArrayLength >= s->m_iCap)
            s->Flush();
    }
    {
        Sink* s = m_pSink;
        char v = cmd->m_b04;
        ((char*)s->m_iCap)[s->m_iEnd - s->m_iArrayLength - 1] = v;
        s->m_iArrayLength++;
        if (s->m_iArrayLength >= s->m_iCap)
            s->Flush();
    }
}

extern char DAT_01000000;

struct S_00e06e00_Item
{
    int unk0;
};

struct S_00e06e00_Buf
{
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    void PutBytePair(int code, unsigned int param);
    void hkpMoppCodeGenerator_addCommand16(int code, unsigned int param);
    void hkpMoppCodeGenerator_addCommand24(int code, unsigned int param);
    void hkpMoppCodeGenerator_addCommand32(int code, unsigned int param);
};

struct S_00e06e00_A0
{
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    S_00e06e00_Item* unk30[12];
    unsigned int* unk60;
};

struct S_00e06e00_A1
{
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44[16];
};

struct S_00e06e00_A2
{
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    unsigned int unk34;
};

struct S_00e06e00_Cls
{
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    S_00e06e00_Buf* unk10;
    int hkpMoppDefaultAssembler_addTerminals(S_00e06e00_A0* thisNode, S_00e06e00_A1* parentInfo, S_00e06e00_A2* currentInfo);
    void WriteOp(int idx, S_00e06e00_Item* item);
};

// @fnv 0x00e06e00
// src: HAVOK_LIB 0x00e06e00
// pdb: hkpMoppDefaultAssembler::addTerminals | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: thisNode, parentInfo, currentInfo
int S_00e06e00_Cls::hkpMoppDefaultAssembler_addTerminals(S_00e06e00_A0* thisNode, S_00e06e00_A1* parentInfo, S_00e06e00_A2* currentInfo)
{
    int start = unk10->unkC;
    unsigned int v = *thisNode->unk60 - currentInfo->unk34;
    S_00e06e00_Buf* buf = unk10;
    if (v < 0x20)
    {
        *(char*)(buf->unk10 - buf->unkC + buf->unk8 - 1) = (char)(v + 0x30);
        buf->unkC++;
        if (buf->unkC >= buf->unk8)
        {
            GrowBuffer();
        }
    }
    else if (v < 0x100)
    {
        buf->PutBytePair(0x50, v);
    }
    else if (v < 0x10000)
    {
        buf->hkpMoppCodeGenerator_addCommand16(0x51, v);
    }
    else if (v < (unsigned int)&DAT_01000000)
    {
        buf->hkpMoppCodeGenerator_addCommand24(0x52, v);
    }
    else
    {
        buf->hkpMoppCodeGenerator_addCommand32(0x53, v);
    }
    for (int i = 0; i < thisNode->unk2C; i++)
    {
        if (thisNode->unk30[i] && parentInfo->unk44[i])
        {
            WriteOp(i, thisNode->unk30[i]);
        }
    }
    return unk10->unkC - start;
}


extern const double g_thresholdA;
extern const double g_thresholdB;

// Compares the x, y and z components of a NiPoint3 against g_thresholdA and g_thresholdB and returns a code from 0 to 12 identifying the region.
// @fnv 0x00e06f20
// pdb: hkpMoppTreeNode::toNode | hkpMoppDefaultAssembler.obj | debug, call graph (shared callers), low
int __stdcall ClassifyVec3Region(const NiPoint3* pVec)
{
    int r;
    if ((double)pVec->x > g_thresholdA)
    {
        r = 0;
        if ((double)pVec->z > g_thresholdA)
        {
            r = 5;
            if ((double)pVec->y > g_thresholdA)
            {
                r = 9;
            }
            else if ((double)pVec->y < g_thresholdB)
            {
                r = 11;
            }
        }
        else if ((double)pVec->z < g_thresholdB)
        {
            r = 6;
            if ((double)pVec->y > g_thresholdA)
            {
                r = 10;
            }
            else if ((double)pVec->y < g_thresholdB)
            {
                r = 12;
            }
        }
        else
        {
            if ((double)pVec->y > g_thresholdA)
            {
                r = 7;
            }
            else if ((double)pVec->y < g_thresholdB)
            {
                r = 8;
            }
        }
    }
    else
    {
        if ((double)pVec->y > g_thresholdA)
        {
            r = 1;
            if ((double)pVec->z > g_thresholdA)
            {
                r = 3;
            }
            else if ((double)pVec->z < g_thresholdB)
            {
                r = 4;
            }
        }
        else
        {
            r = 2;
        }
    }
    return r;
}

struct S_00e07070_Object
{
    int unk00;
    int unk04;
    int unk08;
    Pair r[3];
    int m_24;
    int v[3];
};

struct S_00e07070_Result
{
    char ok;
    int count;
    int q[3];
};

extern "C" int __stdcall hkpMoppDefaultAssembler_calcRescaleBits(S_00e07070_Object* parentInfo, S_00e07070_Object* currentInfo);

// @fnv 0x00e07070
// src: HAVOK_LIB 0x00e07070
// pdb: hkpMoppDefaultAssembler::recalcRescale | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: parent, info, cmd
void __stdcall hkpMoppDefaultAssembler_recalcRescale(S_00e07070_Object* parent, S_00e07070_Object* info, S_00e07070_Result* cmd)
{
    int n = hkpMoppDefaultAssembler_calcRescaleBits(parent, info);
    cmd->ok = 0;
    if (n > 0)
    {
        if (n >= 4)
            n = 4;
        for (;;)
        {
            info->m_24 = parent->m_24 - n;
            int i = 0;
            while (i < 3)
            {
                int s = parent->m_24;
                int q = (info->r[i].a - parent->v[i]) >> s;
                info->v[i] = (q << s) + parent->v[i];
                cmd->q[i] = q;
                if (((info->r[i].b - info->v[i]) >> info->m_24) >= 255)
                    goto retry;
                i++;
            }
            break;
        retry:
            n--;
        }
        cmd->count = n;
        cmd->ok = 1;
    }
    else
    {
        info->m_24 = parent->m_24;
        info->v[0] = parent->v[0];
        info->v[1] = parent->v[1];
        info->v[2] = parent->v[2];
    }
}

struct S_00e07680_Writer
{
    int unk0;
    int unk4;
    int m_cap;
    int m_pos;
    unsigned char* m_buf;
    void AppendChar(int b);
    void GrowBuffer();
    __forceinline void put(unsigned char b)
    {
        m_buf[m_cap - m_pos - 1] = b;
        m_pos++;
        if (m_pos >= m_cap)
            GrowBuffer();
    }
};
struct S_00e07680
{
    int unk0;
    int unk4;
    int unk8;
    int unkc;
    S_00e07680_Writer* w;
    void WriteOffsetDelta(int);
    void hkpMoppDefaultAssembler_addSplit(int pright, int pleft, int directionCode, int lpos, int rpos);
};

// @fnv 0x00e07680
// src: HAVOK_LIB 0x00e07680
// pdb: hkpMoppDefaultAssembler::addSplit | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: pright, pleft, directionCode, lpos, rpos
void S_00e07680::hkpMoppDefaultAssembler_addSplit(int pright, int pleft, int directionCode, int lpos, int rpos)
{
    int e = w->m_pos;
    int d = e - rpos;
    if (directionCode < 3)
    {
        e -= lpos;
        if ((e > 0 || d > 0xf8) && e < 0x10000 && d < 0x10000)
        {
            w->AppendChar(d & 0xff);
            w->AppendChar((d >> 8) & 0xff);
            w->AppendChar(e & 0xff);
            w->AppendChar((e >> 8) & 0xff);
            w->AppendChar(pright);
            w->AppendChar(pleft);
            w->AppendChar(directionCode + 0x23);
            return;
        }
        if (pleft - pright == 1)
        {
            if (d > 0xfb)
            {
                WriteOffsetDelta(rpos);
                rpos = w->m_pos;
            }
            WriteOffsetDelta(lpos);
            w->put(w->m_pos - rpos);
            w->put(pright);
            w->put(directionCode + 0x20);
            return;
        }
    }
    if (d > 0xfb)
    {
        WriteOffsetDelta(rpos);
        rpos = w->m_pos;
    }
    WriteOffsetDelta(lpos);
    w->put(w->m_pos - rpos);
    w->put(pright);
    w->put(pleft);
    w->put(directionCode + 0x10);
}

struct Entry
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    unsigned char unk38;
    int unk3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    int unk58;
    int unk5c;
    int unk60;
    int unk64;
    int unk68;
    int unk6c;
    Entry* unk70;
    Entry* unk74;
};
struct Handler
{
    int unk00, unk04, unk08, unk0c;
    Entry* unk10;
    void __thiscall hkpMoppDefaultAssembler_fixScale(Entry*);
    int __thiscall ClassifyVec3Region(int);
    void __thiscall hkpMoppDefaultAssembler_findPlanes(Entry*, Entry*, Entry**, Entry**);
    void __thiscall hkpMoppDefaultAssembler_addSplit(Entry*, Entry*, int, int, int);
    void __thiscall WriteSize(int);
    void __thiscall hkpMoppDefaultAssembler_checkAndAddProperties(Entry*, Entry*, Entry*);
    void __thiscall hkpMoppDefaultAssembler_addCut(Entry*, Entry*);
    void __thiscall hkpMoppDefaultAssembler_addRescale(char*);
    int __thiscall hkpMoppDefaultAssembler_assembleNonTerminalCommand(Entry*, Entry*, Entry*, char*);
};
// @fnv 0x00e07ca0
// src: HAVOK_LIB 0x00e07ca0
// pdb: hkpMoppDefaultAssembler::assembleNonTerminalCommand | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: currentNode, parentInfo, currentInfo, rescaleCommandInfo
// pdb locals: pright, pleft
int __thiscall Handler::hkpMoppDefaultAssembler_assembleNonTerminalCommand(Entry* currentNode, Entry* parentInfo, Entry* currentInfo, char* rescaleCommandInfo)
{
    Entry* originalFirst = currentNode;
    hkpMoppDefaultAssembler_fixScale(originalFirst);
    int previous = unk10->unk0c;
    int value = ClassifyVec3Region(originalFirst->unk60);
    Entry* originalThird = currentInfo;
    hkpMoppDefaultAssembler_findPlanes(originalFirst, originalThird, &currentNode, &currentInfo);
    hkpMoppDefaultAssembler_addSplit(currentInfo, currentNode, value, originalFirst->unk70->unk5c, originalFirst->unk74->unk5c);
    int difference = originalThird->unk34 - parentInfo->unk34;
    if (difference != 0)
        WriteSize(difference);
    hkpMoppDefaultAssembler_checkAndAddProperties(originalFirst, parentInfo, originalThird);
    hkpMoppDefaultAssembler_addCut(originalFirst, originalThird);
    if (*rescaleCommandInfo)
        hkpMoppDefaultAssembler_addRescale(rescaleCommandInfo);
    originalFirst->unk5c = unk10->unk0c;
    originalFirst->unk38 = 1;
    return unk10->unk0c - previous;
}

extern int __cdecl hkMath_hkFloatToInt(float f);

struct S_00e07da0_Bounds
{
    int unk0;
    char b4;
    int unk8;
    struct Range
    {
        int lo;
        int hi;
    } ranges[3];  // 0x0C
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40[1];
    int unk44[1];

    void AlignRanges();
};

struct S_00e07da0_Region : S_00e07da0_Bounds
{
    int lo48[3];
    int hi54[3];

    void BuildScaledRanges(struct S_00e07da0_Node* node, void* p);
    void hkpMoppAssemblerNodeInfo_updateExtents();
};

struct S_00e07da0_Result
{
    bool valid;
    int value;
    int unk8;
    int unkC;
    int unk10;
};

struct S_00e07da0_Info
{
    char b0;
    char b1;
    char b2;
    char state[3];
    char pad6[2];
    int unk8;
    int mins[3];  // 0x0C
    int maxs[3];  // 0x18
};

struct S_00e07da0_Node
{
    int unk0;
    char b4;
    char pad5[3];
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int count2C;
    int offsets[2];        // 0x30
    S_00e07da0_Info info;  // 0x38
    void* unk5C;
    void* unk60;
    int unk64;
    int unk68;
    int unk6C;
    S_00e07da0_Node* child70;
    S_00e07da0_Node* child74;
};

struct S_00e07da0_Tree
{
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    float unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    float unk54;

    void BuildRegion(S_00e07da0_Node* node, S_00e07da0_Region* parent, S_00e07da0_Region* region);
    void hkpMoppDefaultAssembler_calcTermCut(S_00e07da0_Node* currentNode, S_00e07da0_Region* currentInfo);
    void hkpMoppDefaultAssembler_recalcRescale(S_00e07da0_Region* parent, S_00e07da0_Region* info, S_00e07da0_Result* cmd);
    int ClassifyVec3Region(void* p);
    void hkpMoppDefaultAssembler_findPlanes(S_00e07da0_Node* node, S_00e07da0_Region* info, int* pleft, int* pright);
};

// Recursively builds a region for node from parent and region, merging child bounds and states into node->info.
// @fnv 0x00e07da0
// pdb: hkpMoppDefaultAssembler::preCalcCutPositions | hkpMoppDefaultAssembler.obj | retail, object-file run extension, low
// pdb params: currentNode, parentInfo, currentInfo
// pdb locals: pright, pleft, rescaleCommandInfo, nodes, childInfo
void S_00e07da0_Tree::BuildRegion(S_00e07da0_Node* node, S_00e07da0_Region* parent, S_00e07da0_Region* region)
{
    S_00e07da0_Info* info = &node->info;
    if (info->b0 != 0)
        return;

    if (node->b4 != 0)
    {
        region->AlignRanges();
        hkpMoppDefaultAssembler_calcTermCut(node, region);
        return;
    }

    S_00e07da0_Result out;
    if (info->b2 == 1)
        hkpMoppDefaultAssembler_recalcRescale(parent, region, &out);
    region->AlignRanges();
    int axis = ClassifyVec3Region(node->unk60);
    {
        int lo, hi;
        hkpMoppDefaultAssembler_findPlanes(node, region, &lo, &hi);
    }

    S_00e07da0_Node* first = node->child74;
    S_00e07da0_Node* apChildren[2];
    apChildren[0] = first;
    apChildren[1] = node->child70;

    for (int k = 0; k < 3; k++)
    {
        float sc = unk3C;
        float* ps = &sc;
        hkMath_hkFloatToInt(unk54 * *ps);
        info->mins[k] = 0;
        info->maxs[k] = 0x7fffffff;
    }

    for (int i = 0; i < 2; i++)
    {
        S_00e07da0_Node* child = apChildren[i];
        S_00e07da0_Info* pChildInfo = &child->info;
        if (child == 0)
            continue;

        S_00e07da0_Region childRegion;
        static_cast<S_00e07da0_Bounds&>(childRegion) = *region;
        childRegion.unk38 = child->unk28 - child->unk24;
        childRegion.unk3C = child->count2C;
        for (int iIndex = 0; iIndex < child->count2C; iIndex++)
        {
            childRegion.unk40[iIndex] = child->offsets[iIndex];
            childRegion.unk44[iIndex] = child->offsets[iIndex + 1] - child->offsets[iIndex];
        }
        childRegion.BuildScaledRanges(child, &unk30);
        childRegion.unk0 = region->unk0 + 1;
        childRegion.b4 = 0;
        childRegion.hkpMoppAssemblerNodeInfo_updateExtents();
        BuildRegion(child, region, &childRegion);

        if (child == first)
        {
            if (axis <= 2)
                child->info.mins[axis] = 0;
        }
        else
        {
            if (axis <= 2)
                child->info.maxs[axis] = 0x7fffffff;
        }

        for (int iIndex = 0; iIndex < 3; iIndex++)
        {
            int s = pChildInfo->state[iIndex];
            if (s == 1)
            {
                if (pChildInfo->mins[iIndex] <= region->lo48[iIndex] &&
                    pChildInfo->maxs[iIndex] >= region->hi54[iIndex])
                {
                    int t = info->state[iIndex];
                    info->state[iIndex] = t < 1 ? 1 : t;
                    pChildInfo->state[iIndex] = 0;
                }
            }
            else if (s == 2)
            {
                if (pChildInfo->mins[iIndex] <= region->ranges[iIndex].lo &&
                    pChildInfo->maxs[iIndex] >= region->ranges[iIndex].hi)
                {
                    info->state[iIndex] = 2;
                    pChildInfo->state[iIndex] = 0;
                }
            }
            if (pChildInfo->state[iIndex] == 0)
            {
                {
                    int c = pChildInfo->mins[iIndex];
                    int n = info->mins[iIndex];
                    info->mins[iIndex] = n > c ? n : c;
                }
                {
                    int c = pChildInfo->maxs[iIndex];
                    int n = info->maxs[iIndex];
                    info->maxs[iIndex] = n < c ? n : c;
                }
            }
        }
    }
}



struct BSAudio;

extern int DAT_010ea5dc;
extern int DAT_0102e388;

struct FUN_00e09450_cls
{
    int m_vptr00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    void* m_pData;
    int m_iArrayLength;
    unsigned int m_uCapBits;

    void DestroyAudioBuffer();
};

// @fnv 0x00e09450
void FUN_00e09450_cls::DestroyAudioBuffer()
{
    m_vptr00 = (int)&DAT_010ea5dc;
    hkMemoryRouter* pAudioHolder = (hkMemoryRouter*)hkMemoryRouter_getInstance();
    hkMemoryAllocator* pAudioAllocator = reinterpret_cast<hkMemoryAllocator*>(pAudioHolder->m_heap);
    m_iArrayLength = 0;
    unsigned int uCapacityBits = m_uCapBits;
    if ((uCapacityBits & 0x80000000u) == 0)
    {
        pAudioAllocator->bufFree(m_pData, (uCapacityBits & 0x3fffffffu) * 32);
    }
    m_pData = 0;
    m_uCapBits = 0x80000000u;
    m_vptr00 = (int)&DAT_0102e388;
}

int __cdecl hkMath_hkFloorToInt(float r);

struct FUN_00e094a0_floatpair
{
    float lo;
    float hi;
};


struct FUN_00e094a0_floatset
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    FUN_00e094a0_floatpair pairs[3];
};

struct FUN_00e094a0_cls
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    Pair m_0c[3];

    void BuildScaledRanges(FUN_00e094a0_floatset* a1, float* a2);
};

// @fnv 0x00e094a0
void FUN_00e094a0_cls::BuildScaledRanges(FUN_00e094a0_floatset* pFloatSet, float* pValues)
{
    int best = 0;
    for (int i = 0; i < 3; i++)
    {
        volatile float s = pValues[3];
        float v = (pFloatSet->pairs[i].lo - pValues[i]) * s;
        this->m_0c[i].a = hkMath_hkFloorToInt(v);
        volatile float fScale2 = pValues[3];
        float w = (pFloatSet->pairs[i].hi - pValues[i]) * fScale2;
        this->m_0c[i].b = hkMath_hkFloorToInt(w) + 1;
        int iSpan = this->m_0c[i].b - this->m_0c[i].a;
        if (best <= iSpan)
            best = iSpan;
    }
}

// FUN_00e09540 - compute the level of an object from its bounding box.
//
// this : the object
//   +0x08 : the resulting level (a byte-sized value in the original)
//   +0x0c/+0x10, +0x14/+0x18, +0x1c/+0x20 : the three axis bounds
//
// The largest of the three axis extents is taken, the number of bits it
// needs is counted and the level is derived from that; the result is
// clamped to 0x18.

struct FUN_00e09540_Self
{
    void* unk00;
    int unk04;
    int unk08;  // the level
    int unk0c;  // x min
    int unk10;  // x max
    int unk14;  // y min
    int unk18;  // y max
    int unk1c;  // z min
    int unk20;  // z max
    void hkpMoppAssemblerNodeInfo_updateExtents();
};

// @fnv 0x00e09540
// src: XBOX_DEBUG_PDB 0x00e09540 (was HAVOK_LIB)
// pdb: hkpMoppAssemblerNodeInfo::updateExtents | hkpMoppDefaultAssembler.obj | debug, call graph, high
// pdb params: node, codeInfo
void FUN_00e09540_Self::hkpMoppAssemblerNodeInfo_updateExtents()
{
    int size = this->unk10 - this->unk0c;
    int other = this->unk18 - this->unk14;
    if (size <= other)
        size = other;
    other = this->unk20 - this->unk1c;
    if (size <= other)
        size = other;

    int iBitCount = 0;
    unsigned int shifted = size;
    while (shifted)
    {
        shifted >>= 1;
        iBitCount++;
    }

    int level = -1;
    for (int i = 0; i < 3; i++)
    {
        int scaled = size + (1 << (iBitCount - 4));
        int count = 0;
        unsigned int value = scaled;
        while (value)
        {
            value >>= 1;
            count++;
        }
        if (count > level)
            level = count;
    }

    this->unk08 = (level > 0x18) ? 0x18 : level;
}

class FUN_00e095d0_cls
{
  public:
    unsigned int m_00;
    unsigned char m_04;
    unsigned char unk05;
    unsigned char unk06;
    unsigned char unk07;
    unsigned int unk08;
    unsigned int unk0c;
    unsigned int unk10;
    unsigned int unk14;
    unsigned int unk18;
    unsigned int unk1c;
    unsigned int unk20;
    unsigned int unk24;
    unsigned int unk28;
    unsigned int unk2c;
    unsigned int unk30;
    unsigned int unk34;
    unsigned int unk38;
    unsigned int unk3c;
    unsigned int unk40;
    unsigned int unk44;

    FUN_00e095d0_cls* __thiscall hkpMoppAssemblerNodeInfo_ctor(FUN_00e095d0_cls* parentInfo, unsigned int node, unsigned int codeInfo);
    void __thiscall CopyOffsets(unsigned int a);
    void __thiscall BuildScaledRanges(unsigned int a, unsigned int b);
    void __thiscall hkpMoppAssemblerNodeInfo_updateExtents();
};

// @fnv 0x00e095d0
// src: XBOX_RETAIL_PDB 0x00e095d0
// pdb: hkpMoppAssemblerNodeInfo::hkpMoppAssemblerNodeInfo | hkpMoppDefaultAssembler.obj | retail, call graph, high
// pdb params: parentInfo, node, codeInfo
FUN_00e095d0_cls* FUN_00e095d0_cls::hkpMoppAssemblerNodeInfo_ctor(FUN_00e095d0_cls* parentInfo, unsigned int node, unsigned int codeInfo)
{
    *this = *parentInfo;
    CopyOffsets(node);
    BuildScaledRanges(node, codeInfo);
    m_00 = parentInfo->m_00 + 1;
    m_04 = 0;
    hkpMoppAssemblerNodeInfo_updateExtents();
    return this;
}

// Emit an opcode with an immediate, choosing the byte, short or int form (and its opcode offset) by value range.

class E6BC0Writer
{
  public:
    void WriteByteOp(int code, int param);
    void WriteShortOp(int code, int param);
    void WriteIntOp(int code, int param);
};

class E6BC0Encoder
{
  public:
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    E6BC0Writer* m_pkWriter;

    void WriteOp(int iOpcode, int iValue);
};

// Writes iValue to the writer using a byte, short or int opcode chosen by the value's magnitude, offset from iOpcode.
// @fnv 0x00e06bc0
// pdb: hkpMoppDefaultAssembler::addProperty | hkpMoppDefaultAssembler.obj | debug, object-file run extension, low
// pdb params: property, value
void E6BC0Encoder::WriteOp(int iBaseOpcode, int iValue)
{
    E6BC0Writer* pkWriter = m_pkWriter;
    if (iValue < 0)
        pkWriter->WriteIntOp(iBaseOpcode + 0x68, iValue);
    else if (iValue < 0x100)
        pkWriter->WriteByteOp(iBaseOpcode + 0x60, iValue);
    else if (iValue < 0x10000)
        pkWriter->WriteShortOp(iBaseOpcode + 0x64, iValue);
    else
        pkWriter->WriteIntOp(iBaseOpcode + 0x68, iValue);
}

struct FUN_00e06ed0_Class
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    S_00e06e00_Buf* m_pStreamWriter;
    void WriteOffsetDelta(int iTarget);
};

// Computes the difference between the buffer's field at offset 12 and iTarget, then writes it with a width code (5-8) chosen by its magnitude if positive.
// @fnv 0x00e06ed0
// pdb: hkMemoryAllocator::_bufAlloc<hkpMoppSplittingPlaneDirection> | hkpMoppDefaultAssembler.obj | debug, object-file run extension, low
// pdb params: reqNumInOut
// pdb locals: n
void FUN_00e06ed0_Class::WriteOffsetDelta(int iTargetValue)
{
    int iOffset = ((int*)m_pStreamWriter)[3] - iTargetValue;
    if (iOffset > 0)
    {
        if (iOffset < 0xff)
            m_pStreamWriter->PutBytePair(5, iOffset);
        else if (iOffset < 0xffff)
            m_pStreamWriter->hkpMoppCodeGenerator_addCommand16(6, iOffset);
        else if (iOffset < 0xffffff)
            m_pStreamWriter->hkpMoppCodeGenerator_addCommand24(7, iOffset);
        else
            m_pStreamWriter->hkpMoppCodeGenerator_addCommand32(8, iOffset);
    }
}

extern int __cdecl hkMath_hkFloatToInt(float f);

struct Node07400
{
    int unk00;
    char b04;
    int unk08;
    struct Range
    {
        float fLo;
        float fHi;
    } ranges[3];
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int mins[3];
    int maxs[3];
};

struct Region07400
{
    int unk00;
    char b04;
    int unk08;
    struct Range
    {
        int lo;
        int hi;
    } ranges[3];
    int m_iShift;
};

template <class T> inline T Min07400(T a, T b)
{
    return a < b ? a : b;
}
template <class T> inline T Max07400(T a, T b)
{
    return a > b ? a : b;
}

struct Tree07400
{
    int unk00[15];
    float m_fScale;
    int unk40[6];
    float m_fMinCount;
    int unk5C;
    float m_afMaxCount[3];

    void hkpMoppDefaultAssembler_calcTermCut(Node07400* currentNode, Region07400* currentInfo);
};

// @fnv 0x00e07400
// src: XBOX_RETAIL_PDB 0x00e07400
// pdb: hkpMoppDefaultAssembler::calcTermCut | hkpMoppDefaultAssembler.obj | retail, object-file gap fill, high
// pdb params: currentNode, currentInfo
void Tree07400::hkpMoppDefaultAssembler_calcTermCut(Node07400* currentNode, Region07400* currentInfo)
{
    for (int i = 0; i < 3; i++)
    {
        float fExtent = currentNode->ranges[i].fHi - currentNode->ranges[i].fLo;
        float fCount = (float)(1 << currentInfo->m_iShift) / m_fScale;
        fCount = Max07400(fCount, m_fMinCount);
        float fMaxCount = m_afMaxCount[i];
        if (fExtent < fMaxCount)
            fCount = Min07400(fCount, fMaxCount);
        float fScale = m_fScale;
        int iMargin = hkMath_hkFloatToInt(fScale * fCount);
        int iHi = currentInfo->ranges[i].hi;
        currentNode->mins[i] = currentInfo->ranges[i].lo - iMargin;
        currentNode->maxs[i] = iHi + iMargin + 1;
    }
}

struct Arg1Flags
{
    unsigned char unk00[0x3b];
    unsigned char m_3b[3];
};


struct Arg2Layout
{
    int unk00;
    int unk04;
    int unk08;
    Pair m_0c[3];
    int m_24;
    int m_28[3];
};

struct BufWriter
{
    int unk00;
    int unk04;
    int m_08;
    int m_0c;
    int m_10;
    void __thiscall Flush();
    void __thiscall hkpMoppCodeGenerator_addParam24(int param);
};

inline void EmitByte(BufWriter* w, unsigned char v)
{
    ((char*)(w->m_10 - w->m_0c))[w->m_08 - 1] = v;
    w->m_0c++;
    if (w->m_0c >= w->m_08)
    {
        w->Flush();
    }
}

struct FUN_00e07550_cls
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    BufWriter* m_10;
    void __thiscall hkpMoppDefaultAssembler_addCut(Arg1Flags* currentNode, Arg2Layout* currentInfo);
};

// @fnv 0x00e07550
// src: HAVOK_LIB 0x00e07550
// pdb: hkpMoppDefaultAssembler::addCut | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: currentNode, currentInfo
void FUN_00e07550_cls::hkpMoppDefaultAssembler_addCut(Arg1Flags* currentNode, Arg2Layout* currentInfo)
{
    int i = 0;
    int* pHighValue = &currentInfo->m_0c[0].b;
    int* volatile pBaseValue = currentInfo->m_28;
    for (; i < 3; i++)
    {
        if (currentNode->m_3b[i] == 1)
        {
            int base = *pBaseValue;
            int shift = currentInfo->m_24;
            unsigned char ucHighDelta = (unsigned char)(((pHighValue[0] - base) >> shift) + 1);
            unsigned char ucLowDelta = (unsigned char)((pHighValue[-1] - base) >> shift);
            EmitByte(m_10, ucHighDelta);
            EmitByte(m_10, ucLowDelta);
            EmitByte(m_10, (unsigned char)(i + 0x26));
        }
        if (currentNode->m_3b[i] == 2)
        {
            int hi = pHighValue[0];
            int lo = pHighValue[-1];
            m_10->hkpMoppCodeGenerator_addParam24(hi + 1);
            m_10->hkpMoppCodeGenerator_addParam24(lo);
            EmitByte(m_10, (unsigned char)(i + 0x29));
        }
        pHighValue += 2;
        pBaseValue++;
    }
}

extern "C" unsigned int __cdecl _rotl(unsigned int, int);

struct KdNode;
struct KdCtx;

struct KdCtx
{
    int level;
    char flag;
    char unk05;
    char unk06;
    char unk07;
    int shift;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;

    void __thiscall CopyOffsets(KdNode* k);
    void __thiscall BuildScaledRanges(KdNode* k, float* p);
    void __thiscall hkpMoppAssemblerNodeInfo_updateExtents();
};

struct KdNode
{
    int unk00;
    char leaf;
    char unk05;
    char unk06;
    char unk07;
    int unk08;
    float b[6];
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    char unk38;
    char a;
    char bb;
    char cc[3];
    char unk3e;
    char unk3f;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    int unk58;
    int unk5c;
    int unk60;
    int unk64;
    int unk68;
    int unk6c;
    KdNode* ch[2];
};

struct MyTree
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int lim;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    float f3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    float f58;
    int unk5c;
    float f60[3];

    int __thiscall hkpMoppDefaultAssembler_calcRescaleBits(KdCtx* parentInfo, KdCtx* currentInfo);
    void __thiscall hkpMoppDefaultAssembler_recalcRescale(KdCtx* parent, KdCtx* info, S_00e07070_Result* cmd);
    int __thiscall hkpMoppDefaultAssembler_preCalcScale(KdNode* currentNode, KdCtx* parentInfo, KdCtx* currentInfo);
};

// Recursively returns a shift level for n: a value from f3c, f58 and f60 for a leaf, else the minimum over its children, setting bb on some nodes.
// @fnv 0x00e07160
// src: XBOX_RETAIL_PDB 0x00e07160
// pdb: hkpMoppDefaultAssembler::preCalcScale | hkpMoppDefaultAssembler.obj | retail, object-file gap fill, high
// pdb params: currentNode, parentInfo, currentInfo
// pdb locals: nodes, requiredBitsChildren, rescaleCommandInfo, childInfo
int __thiscall MyTree::hkpMoppDefaultAssembler_preCalcScale(KdNode* currentNode, KdCtx* parentInfo, KdCtx* currentInfo)
{
    int lo = currentInfo->shift;
    if (currentNode->leaf)
    {
        float prev = f58;
        int i = 0;
        int pObj = _rotl(1, lo);
        float* pBounds = &currentNode->b[0];
        float* p = f60;
        float q;
        for (; i < 3; i++, p++, pBounds += 2)
        {
            if (!currentNode->a)
                currentNode->cc[i] = 1;
            q = *p;
            float d = pBounds[1] - pBounds[0];
            if (d < q && q < prev)
            {
                d = f3c;
                int cp;
                cp = hkMath_hkFloatToInt(d * q);
                if ((cp << 7) >= pObj)
                {
                    prev = q;
                }
                else if (!currentNode->a)
                {
                    currentNode->cc[i] = 2;
                }
            }
        }
        unsigned int cp = (unsigned int)hkMath_hkFloatToInt(f3c * double(0.9f) * prev);
        int k = 0;
        while (cp)
        {
            cp >>= 1;
            ++k;
        }
        int v = k + 6;
        return v > lo ? v : lo;
    }
    if (!currentNode->a && currentInfo->level < lim && hkpMoppDefaultAssembler_calcRescaleBits(parentInfo, currentInfo) > 2)
    {
        currentNode->bb = 1;
    }
    S_00e07070_Result buf;
    if (currentNode->bb == 1)
    {
        hkpMoppDefaultAssembler_recalcRescale(parentInfo, currentInfo, &buf);
    }
    long r[2] = {-1, -1};
    long t = 0x7fffffff;
    KdNode* apChildren[2] = {currentNode->ch[0], currentNode->ch[1]};
    for (int i = 1; i >= 0; i--)
    {
        KdNode* k = apChildren[i];
        if (k)
        {
            KdCtx stCtx = *currentInfo;
            stCtx.CopyOffsets(k);
            stCtx.BuildScaledRanges(k, reinterpret_cast<float*>(&unk30));
            stCtx.level = currentInfo->level + 1;
            stCtx.flag = 0;
            stCtx.hkpMoppAssemblerNodeInfo_updateExtents();
            r[i] = hkpMoppDefaultAssembler_preCalcScale(k, currentInfo, &stCtx);
            if (t >= stCtx.shift)
                t = stCtx.shift;
        }
    }
    int iMin = lo + 2 < t + 2 ? lo + 2 : t + 2;
    int v;
    KdNode* best = apChildren[0];
    if (best)
    {
        v = r[0];
        if (currentNode->a)
        {
            if (!best->a && v < iMin)
                best->bb = 1;
        }
        else if (v < lo)
        {
            best->bb = 1;
        }
        if (!best->bb && iMin >= v)
            iMin = v;
    }
    KdNode* pSecond = apChildren[1];
    if (pSecond)
    {
        v = r[1];
        if (currentNode->a)
        {
            if (!pSecond->a && v < iMin)
                pSecond->bb = 1;
        }
        else if (v < lo)
        {
            pSecond->bb = 1;
        }
        if (!pSecond->bb && iMin >= v)
            iMin = v;
    }
    return iMin;
}

#include <xmmintrin.h>
extern double _gModifierZero;
struct FUN_00e07b90_bounds
{
    float unk00, unk04, unk08, minimumX, maximumX, minimumY, maximumY, minimumZ, maximumZ;
};
struct FUN_00e07b90_vector
{
    float x, y, z, w;
};
// Stores the minimum corner of bounds into result and sets result->w to a scale factor derived from the largest axis extent.
// @fnv 0x00e07b90
// src: XBOX_PDB 0x00e07b90
// pdb: hkpMoppDefaultAssembler::addJump | hkpMoppDefaultAssembler.obj | debug, object-file run extension, low
// pdb params: jumpPos
// pdb locals: assertBuf, ostr
void __stdcall hkpMoppDefaultAssembler_getScaleInfo(const FUN_00e07b90_bounds* bounds, FUN_00e07b90_vector* result)
{
    __m128 minCorner = _mm_setr_ps(bounds->minimumX, bounds->minimumY, bounds->minimumZ, 0.0f);
    _mm_store_ps(&result->x, minCorner);
    float fLargestExtent = 0.0f;
    float extent = bounds->maximumX - bounds->minimumX;
    if (!(extent < _gModifierZero))
        fLargestExtent = extent;
    extent = bounds->maximumY - bounds->minimumY;
    if (!(fLargestExtent > extent))
        fLargestExtent = extent;
    extent = bounds->maximumZ - bounds->minimumZ;
    if (!(fLargestExtent > extent))
        fLargestExtent = extent;
    result->w = 65536.0 * (254.0 / fLargestExtent);
}

struct S_00e07d60_Info
{
    float x;
    float y;
    float z;
    __m128 v;
    int i;
    char b;
};

struct S_00e07d60
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    float x;
    float y;
    float z;
    __m128 v;
    int i;
    char b;
    void UpdateFields_00e07d60(const S_00e07d60_Info& info);
};

// @fnv 0x00e07d60
// src: GUESS 0x00e07d60 (confidence: low)
void S_00e07d60::UpdateFields_00e07d60(const S_00e07d60_Info& info)
{
    x = info.x;
    y = info.y;
    z = info.z;
    v = info.v;
    i = info.i;
    b = info.b;
}

namespace e083 {

struct SrcW
{
    hkArrayBase<Pair>* p;
};
}

void __cdecl ResizeAudioStream(hkMemoryAllocator* mem, void* array, int sizeElem);

// Copies all pairs from srcp to dst, adding delta to each pair's a field, growing dst when full.
// @fnv 0x00e08320
// src: HAVOK_LIB 0x00e08320
// pdb: copyJumpCommandInfo | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: newCommands, originalCommands, offset
static __declspec(noinline) void copyJumpCommandInfo(hkArrayBase<Pair>* newCommands, hkArrayBase<Pair>* originalCommands, int offset)
{
    hkArrayBase<Pair>* const* pp = &newCommands;
    hkArrayBase<Pair>* src = *pp;
    for (int i = 0; i < src->m_iElementCount; i++)
    {
        hkMemoryRouter* h = hkMemoryRouter_getInstance();
        if (originalCommands->m_iElementCount == (originalCommands->m_iCapacityState & 0x3fffffff))
            ResizeAudioStream(h->m_heap, originalCommands, 8);
        int n = originalCommands->m_iElementCount;
        Pair* out = originalCommands->m_data + n;
        originalCommands->m_iElementCount = n + 1;
        out->b = src->m_data[i].b;
        out->a = src->m_data[i].a + offset;
    }
}

void e08320_caller(e083::SrcW sw, hkArrayBase<Pair>* dst, int delta)
{
    copyJumpCommandInfo(sw.p, dst, delta);
}

struct BvAllocator
{
    virtual void v0();
    virtual void* AllocateMemoryBlock(unsigned int);
    virtual void v8();
    virtual void vc();
    virtual void ReleaseBlock(void*, int);
};
void __cdecl ResizeAudioStream(void*, void*, int);
inline BvAllocator& GetHeap()
{
    return *(BvAllocator*)hkMemoryRouter_getInstance()->m_heap;
}
inline hkMemoryRouter* GetBvRouter()
{
    return hkMemoryRouter_getInstance();
}
template <class T>
static __forceinline void BvArray_clear(hkArrayBase<T>& self);

template <class T>
static __forceinline void BvArray_ClearAndRelease(hkArrayBase<T>& self, BvAllocator& alloc);

template <class T>
static __forceinline void BvArray_AddBack(hkArrayBase<T>& self, hkMemoryRouter& router, const T& t);

template <class T>
static __forceinline void BvArray_clear(hkArrayBase<T>& self)
{
        for (int i = 0; i < self.m_iElementCount; ++i)
            self.m_data[i].~T();
        self.m_iElementCount = 0;
    }

template <class T>
static __forceinline void BvArray_ClearAndRelease(hkArrayBase<T>& self, BvAllocator& alloc)
{
        BvArray_clear(self);
        if ((self.m_iCapacityState & 0x80000000u) == 0)
            alloc.ReleaseBlock(self.m_data, self.getCapacity() * sizeof(T));
        self.m_data = 0;
        self.m_iCapacityState = 0x80000000;
    }

template <class T>
static __forceinline void BvArray_AddBack(hkArrayBase<T>& self, hkMemoryRouter& router, const T& t)
{
        if (self.m_iElementCount == self.getCapacity())
            ResizeAudioStream(router.m_heap, (&self), sizeof(T));
        T* p = self.m_data + self.m_iElementCount;
        for (int i = 0; i < 1; i++)
            p[i] = t;
        self.m_iElementCount++;
    }

struct BvFixup
{
    int offset;
    int value;
};
struct FUN_00e065f0
{
    void* m_ppVirtualMethodTable;
    unsigned short m_usMemSizeAndFlags;
    short m_sReferenceCount;
    int m_pRayCursor;
    int m_offset;
    unsigned char* m_data;
    hkArrayBase<BvFixup> m_buffer;

    void hkpMoppCodeGenerator_pad(int);
    void nullsub_26();
    FUN_00e065f0(unsigned int size) throw();
    void hkpMoppCodeGenerator_dtor();
    void* operator new(size_t n)
    {
        FUN_00e065f0* p = static_cast<FUN_00e065f0*>(((BvAllocator*)GetBvRouter()->m_heap)->AllocateMemoryBlock(n));
        p->m_usMemSizeAndFlags = (unsigned short)n;
        return p;
    }
};

struct WriterEntry
{
    FUN_00e065f0* m_pStreamWriter;
    unsigned int m_iOwnerObjectOffset;
};

struct BvReferenceConverter
{
    virtual void v0();
    virtual int ConvertReferences(int*, int);
    virtual void writeBuffer(int*, int, unsigned char*);
};
struct BvTree
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    WriterEntry* m_entries;
    int unk14;
    int unk18;
    BvReferenceConverter* m_converter;
};

struct BvNode;

struct BvInfo
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int m_uiEdgeIndex;
};

struct BvNode
{
    int unk00;
    bool m_isLeaf;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int m_depth;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    int unk58;
    int unk5c;
    BvInfo* m_info;
    int unk64;
    int unk68;
    int unk6c;
    BvNode* m_fMinimumU;
    BvNode* m_fMaximumU;
};

struct BvRange
{
    bool m_valid;
    int m_uiEdgeIndex;
    int unk08;
    int unk0c;
    int unk10;
};

void __stdcall hkpMoppDefaultAssembler_assembleCutAndTerminalCommand(struct BvNode* currentNode, FUN_00e095d0_cls* parentInfo, FUN_00e095d0_cls* currentInfo);
void* __cdecl hkString_memCpy(void*, const void*, int);
// src: HAVOK_LIB 0x00e08320
static void copyJumpCommandInfo(hkArrayBase<BvFixup>* newCommands, hkArrayBase<BvFixup>* originalCommands, int offset)
{
    for (int i = 0; i < originalCommands->m_iElementCount; ++i)
    {
        BvAllocator& heap = GetHeap();
        if (newCommands->m_iElementCount == newCommands->getCapacity())
            ResizeAudioStream(&heap, newCommands, sizeof(BvFixup));
        BvFixup* out = newCommands->m_data + newCommands->m_iElementCount;
        ++newCommands->m_iElementCount;
        out->value = originalCommands->m_data[i].value;
        out->offset = originalCommands->m_data[i].offset + offset;
    }
}
// src: HAVOK_LIB 0x00e08380
static int copyCodeAndEmbedTerminals(BvTree* chunkInfo, FUN_00e065f0* codeWithTerminals, hkArrayBase<int>* codeWithoutTerminals, FUN_00e065f0* terminals)
{
    int padding = (-codeWithTerminals->m_offset) & 3;
    if (!chunkInfo->m_converter)
        codeWithoutTerminals->m_iElementCount = 0;
    if (codeWithoutTerminals->m_iElementCount)
    {
        int bytes = chunkInfo->m_converter->ConvertReferences(codeWithoutTerminals->m_data, codeWithoutTerminals->m_iElementCount);
        terminals->hkpMoppCodeGenerator_pad(bytes);
        hkMemoryRouter* router = hkMemoryRouter_getInstance();
        unsigned char* temporary = static_cast<unsigned char*>(router->m_stack.m_next.m_storage);
        router->m_stack.m_next.m_storage = temporary + ((bytes + 127) & ~127);
        chunkInfo->m_converter->writeBuffer(codeWithoutTerminals->m_data, codeWithoutTerminals->m_iElementCount, temporary);
        hkString_memCpy(terminals->m_data + terminals->m_pRayCursor - terminals->m_offset, temporary, bytes);
        hkMemoryRouter_getInstance()->m_stack.m_next.m_storage = temporary;
    }
    int before = terminals->m_offset;
    terminals->hkpMoppCodeGenerator_pad(codeWithTerminals->m_offset + padding);
    hkString_memCpy(terminals->m_data + terminals->m_pRayCursor - terminals->m_offset,
                 codeWithTerminals->m_data + codeWithTerminals->m_pRayCursor - codeWithTerminals->m_offset, codeWithTerminals->m_offset);
    codeWithTerminals->nullsub_26();
    copyJumpCommandInfo(&terminals->m_buffer, &codeWithTerminals->m_buffer, terminals->m_offset - codeWithTerminals->m_offset);
    terminals->nullsub_26();
    return terminals->m_offset - before;
}

struct BvBuilder
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    FUN_00e065f0* m_pStreamWriter;
    BvTree* m_iValueAt40;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int m_info30;

    void hkpMoppDefaultAssembler_addDataOffset(int offset, int numTerminals);
    void hkpMoppDefaultAssembler_assembleJumpChunkCommand(BvNode* currentNode);
    void hkpMoppDefaultAssembler_initTopDown(BvNode* currentNode, FUN_00e095d0_cls* parentInfo, FUN_00e095d0_cls* currentInfo, BvRange* rescaleCommandInfo);
    void hkpMoppDefaultAssembler_assembleNonTerminalCommand(BvNode* currentNode, FUN_00e095d0_cls* parentInfo, FUN_00e095d0_cls* currentInfo, BvRange* rescaleCommandInfo);
    void hkpMoppDefaultAssembler_assembleSubNodeIntoChunks(BvNode* currentNode, FUN_00e095d0_cls* parentInfo, FUN_00e095d0_cls* currentInfo, int chunkId, hkArrayBase<int>* terminalsOut);
};

// Recursively walks a BvNode: deep nodes go to a separate writer, leaves add their edge index to refs, other nodes recurse into both children.
// @fnv 0x00e08a60
// src: HAVOK_LIB 0x00e08a60
// pdb: hkpMoppDefaultAssembler::assembleSubNodeIntoChunks | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: currentNode, parentInfo, currentInfo, chunkId, terminalsOut
// pdb locals: rescaleCommandInfo, chunkCode, chunkTerminals, assertBuf, ostr, childInfo
void BvBuilder::hkpMoppDefaultAssembler_assembleSubNodeIntoChunks(BvNode* currentNode, FUN_00e095d0_cls* parentInfo, FUN_00e095d0_cls* currentInfo, int chunkId,
                             hkArrayBase<int>* terminalsOut)
{
    if (currentNode->m_depth > 0 && !currentInfo->m_04)
    {
        int depthValue = currentNode->m_depth;
        FUN_00e065f0* saved = m_pStreamWriter;
        WriterEntry& entry = m_iValueAt40->m_entries[depthValue];
        {
            FUN_00e065f0 writer(entry.m_iOwnerObjectOffset);
            m_pStreamWriter = &writer;
            currentInfo->m_04 = true;
            hkArrayBase<int> subRefs;
            subRefs.m_data = 0;
            subRefs.m_iElementCount = 0;
            subRefs.m_iCapacityState = 0x80000000;
            hkpMoppDefaultAssembler_assembleSubNodeIntoChunks(currentNode, parentInfo, currentInfo, depthValue, &subRefs);
            if (m_iValueAt40->m_converter != 0)
            {
                int offset = m_pStreamWriter->m_offset + 5;
                hkpMoppDefaultAssembler_addDataOffset(offset + ((-offset) & 3), subRefs.m_iElementCount);
            }
            entry.m_pStreamWriter = new FUN_00e065f0(entry.m_iOwnerObjectOffset);
            copyCodeAndEmbedTerminals(m_iValueAt40, m_pStreamWriter, &subRefs, entry.m_pStreamWriter);
            BvArray_clear(m_pStreamWriter->m_buffer);
            BvArray_ClearAndRelease(subRefs, GetHeap());
            writer.hkpMoppCodeGenerator_dtor();
        }
        m_pStreamWriter = saved;
        hkpMoppDefaultAssembler_assembleJumpChunkCommand(currentNode);
        return;
    }

    if (currentNode->m_isLeaf)
    {
        hkpMoppDefaultAssembler_assembleCutAndTerminalCommand(currentNode, parentInfo, currentInfo);
        const int& index = currentNode->m_info->m_uiEdgeIndex;
        BvArray_AddBack((*(terminalsOut)), *hkMemoryRouter_getInstance(), index);
        return;
    }

    {
        BvRange range;
        FUN_00e095d0_cls child;
        range.m_valid = false;
        range.m_uiEdgeIndex = -1;
        hkpMoppDefaultAssembler_initTopDown(currentNode, parentInfo, currentInfo, &range);
        BvNode* right = currentNode->m_fMaximumU;
        child.hkpMoppAssemblerNodeInfo_ctor(currentInfo, (unsigned int)right, (unsigned int)&m_info30);
        hkpMoppDefaultAssembler_assembleSubNodeIntoChunks(right, currentInfo, &child, chunkId, terminalsOut);
        BvNode* left = currentNode->m_fMinimumU;
        child.hkpMoppAssemblerNodeInfo_ctor(currentInfo, (unsigned int)left, (unsigned int)&m_info30);
        hkpMoppDefaultAssembler_assembleSubNodeIntoChunks(left, currentInfo, &child, chunkId, terminalsOut);
        hkpMoppDefaultAssembler_assembleNonTerminalCommand(currentNode, parentInfo, currentInfo, &range);
    }
}

inline void* operator new(size_t, void* p)
{
    return p;
}
template <class T> __forceinline void constructCopy(T* p, int n, const T& value)
{
    for (int i = 0; i < n; ++i)
        p[i] = value;
}

void __cdecl ResizeAudioStream(hkMemoryAllocator*, void*, int);
struct P154Array
{
    int* data;
    int count;
    unsigned capacity;
    P154Array() : data(0), count(0), capacity(0x80000000)
    {}
    void Search(hkMemoryAllocator*, int, int*, int);
    void append(const int& v)
    {
        hkMemoryRouter* s = hkMemoryRouter_getInstance();
        if (count == int(capacity & 0x3fffffff))
            ResizeAudioStream((hkMemoryAllocator*)s->m_heap, this, 4);
        constructCopy(data + count, 1, v);
        ++count;
    }
    void insert(int* src, int pos, int n)
    {
        hkMemoryRouter* r = hkMemoryRouter_getInstance();
        hkMemoryAllocator* a = (hkMemoryAllocator*)r->m_heap;
        Search(a, pos, src, n);
    }
    void clear()
    {
        hkMemoryRouter* r = hkMemoryRouter_getInstance();
        hkMemoryAllocator* a = (hkMemoryAllocator*)r->m_heap;
        count = 0;
        if (!(capacity & 0x80000000))
            a->bufFree(data, (capacity & 0x3fffffff) * 4);
        data = 0;
        capacity = 0x80000000;
    }
};
struct P154PairArray
{
    Pair* data;
    int count;
    unsigned capacity;
    Pair& grow()
    {
        hkMemoryRouter* s = hkMemoryRouter_getInstance();
        if (count == int(capacity & 0x3fffffff))
            ResizeAudioStream((hkMemoryAllocator*)s->m_heap, this, 8);
        for (int i = 0; i < 1; ++i)
            new (data + count + i) Pair;
        return data[count++];
    }
};
struct P154Cost
{
    virtual void v00();
    virtual int cost(int*, int);
};
struct P154Settings
{
    int limit, unk04, unk08, unk0c;
    P154PairArray pairs;
    P154Cost* metric;
    int base;
};
struct P154Writer
{
    int unk00, unk04, unk08, count;
};
struct P154Region
{
    int depth;
    char leaf;
    int unk08;
    int unk0c, unk10, unk14, unk18, unk1c, unk20;
    int unk24, unk28, unk2c, unk30, context, unk38, unk3c, unk40, unk44;
    void CopyOffsets(struct P154Node*);
    void BuildScaledRanges(struct P154Node*, float*);
    void hkpMoppAssemblerNodeInfo_updateExtents();
};
struct P154Node
{
    int unk00;
    char leaf;
    int unk08, unk0c, unk10, unk14, unk18, unk1c, unk20, unk24, unk28, unk2c, unk30, unk34, unk38, unk3c, index, unk44,
        unk48, unk4c, unk50, unk54, unk58, offset;
    int* values;
    int unk64, unk68, unk6c;
    P154Node* high;
    P154Node* low;
    __forceinline void shift(int amount, bool change)
    {
        if (change)
            offset += amount;
    }
};
__forceinline void adjustOffset(P154Node* node, int amount, bool change)
{
    if (change)
        node->offset += amount;
}
struct P154Result
{
    bool valid;
    int value;
    int unk08, unk0c, unk10;
};
struct P154Tree
{
    int unk00, unk04, unk08, unk0c;
    P154Writer* writer;
    P154Settings* settings;
    int unk18, unk1c, unk20, unk24, unk28, unk2c;
    int scale30, scale34, scale38;
    void hkpMoppDefaultAssembler_assembleCutAndTerminalCommand(P154Node*, P154Region*, P154Region*);
    void hkpMoppDefaultAssembler_initTopDown(P154Node*, P154Region*, P154Region*, P154Result*);
    void hkpMoppDefaultAssembler_assembleNonTerminalCommand(P154Node*, P154Region*, P154Region*, P154Result*);
    __forceinline int getCost(P154Array& a, int size)
    {
        int result = size;
        if (settings->metric && a.count)
            result += settings->metric->cost(a.data, a.count);
        return result;
    }
    int hkpMoppDefaultAssembler_calcChunkPoints(P154Node*, P154Region*, P154Region*, P154Array*);
};
// Recursively sizes node's children via writer->count with optional metric costs, collapses the costlier child while size reaches limit minus base, returns size.
// @fnv 0x00e084f0
// src: HAVOK_LIB 0x00e084f0
// pdb: hkpMoppDefaultAssembler::calcChunkPoints | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: currentNode, parentInfo, currentInfo, terminalsOut
// pdb locals: leftTerminals, allSubTerminals, rightTerminals, rescaleCommandInfo, primId, assertBuf, ostr, childInfo
int P154Tree::hkpMoppDefaultAssembler_calcChunkPoints(P154Node* currentNode, P154Region* parentInfo, P154Region* currentInfo, P154Array* terminalsOut)
{
    int size;
    if (settings->metric)
        currentInfo->context = parentInfo->context;
    if (currentNode->leaf)
    {
        if (settings->metric)
            currentInfo->context = *currentNode->values;
        int start = writer->count;
        hkpMoppDefaultAssembler_assembleCutAndTerminalCommand(currentNode, parentInfo, currentInfo);
        int leafSize = writer->count - start;
        terminalsOut->append(*currentNode->values);
        int value = *currentNode->values;
        int result = leafSize;
        if (settings->metric)
            result += settings->metric->cost(&value, 1);
        return result;
    }
    else
    {
        P154Result info;
        info.valid = false;
        info.value = -1;
        hkpMoppDefaultAssembler_initTopDown(currentNode, parentInfo, currentInfo, &info);
        currentInfo->context = parentInfo->context;
        P154Array lowValues;
        P154Node* low = currentNode->low;
        P154Region child = *currentInfo;
        child.CopyOffsets(low);
        child.BuildScaledRanges(low, reinterpret_cast<float*>(&scale30));
        child.depth = currentInfo->depth + 1;
        child.leaf = false;
        child.hkpMoppAssemblerNodeInfo_updateExtents();
        int lowSize = writer->count;
        hkpMoppDefaultAssembler_calcChunkPoints(low, currentInfo, &child, &lowValues);
        lowSize = writer->count - lowSize;
        int lowCost = getCost(lowValues, lowSize);
        P154Array highValues;
        P154Node* high = currentNode->high;
        child = *currentInfo;
        child.CopyOffsets(high);
        child.BuildScaledRanges(high, reinterpret_cast<float*>(&scale30));
        child.depth = currentInfo->depth + 1;
        child.leaf = false;
        child.hkpMoppAssemblerNodeInfo_updateExtents();
        int highSize = writer->count;
        hkpMoppDefaultAssembler_calcChunkPoints(high, currentInfo, &child, &highValues);
        highSize = writer->count - highSize;
        int highCost = getCost(highValues, highSize);
        int splitSize = writer->count;
        hkpMoppDefaultAssembler_assembleNonTerminalCommand(currentNode, parentInfo, currentInfo, &info);
        splitSize = writer->count - splitSize;
        P154Array merged;
        merged.insert(lowValues.data, 0, lowValues.count);
        merged.insert(highValues.data, merged.count, highValues.count);
        size = highSize + splitSize + lowSize;
        if (settings->metric && merged.count)
            size += settings->metric->cost(merged.data, merged.count);
        bool highRemoved = false, lowRemoved = false;
        while (size >= settings->limit - settings->base)
        {
            int index = settings->pairs.count;
            Pair& pair = settings->pairs.grow();
            P154Node* selected;
            int oldSize, cost;
            if (highCost > lowCost)
            {
                selected = currentNode->high;
                cost = highCost;
                oldSize = highSize;
                highCost = 5;
                highRemoved = true;
                size = lowRemoved ? splitSize + 10 : splitSize + lowCost + 5;
            }
            else
            {
                selected = currentNode->low;
                cost = lowCost;
                oldSize = lowSize;
                lowCost = 5;
                lowRemoved = true;
                size = highRemoved ? splitSize + 10 : splitSize + highCost + 5;
            }
            pair.b = settings->base + cost + 5;
            selected->index = index;
            writer->count += 5 - oldSize;
            P154Node* lowChild = currentNode->low;
            if (lowRemoved)
                lowChild->offset += 5 - oldSize;
            currentNode->high->offset += 5 - oldSize;
            currentNode->offset += 5 - oldSize;
        }
        terminalsOut->count = 0;
        if (!lowRemoved)
            terminalsOut->insert(lowValues.data, 0, lowValues.count);
        if (!highRemoved)
            terminalsOut->insert(highValues.data, terminalsOut->count, highValues.count);
        merged.clear();
        highValues.clear();
        lowValues.clear();
    }
    return size;
}

namespace fnv_00e08240 {
struct PairEntry
{
    void* m_pHeadNode;
    void* m_iSecondInteger;
};

struct PairList
{
    PairEntry* m_data;
    int m_uiItemCount;
    unsigned int m_capBits;
};


hkMemoryRouter* hkMemoryRouter_getInstance();
void ResizeAudioStream(void* mem, PairList* array, unsigned int sizeElem);

struct SlotPair
{
    int m_00;
    int unk04;
    int unk08;
    int unk0C;
    int m_10;
};

struct Ctx
{
    int unk00;
    PairList m_kNode;
    int unk10;
    int unk14;
    int unk18;
    int m_1c;
};

struct Node
{
    int unk00;
    unsigned char m_04;
    unsigned char unk05;
    unsigned short unk06;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int m_24;
    int m_28;
    int unk2C;
    int unk30;
    int unk34;
    unsigned char m_38;
    unsigned char unk39;
    unsigned short unk3A;
    int unk3C;
    int m_40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    int unk58;
    int unk5C;
    SlotPair* m_60;
    int unk64;
    int unk68;
    int unk6C;
    Node* m_70;
    Node* m_74;
};

struct FUN_00e08240_cls
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    Ctx* m_14;
    int hkpMoppDefaultAssembler_calcTerminalIdsForChunks(Node* currentNode, int chunkId, int currentId);
};

// Recursively visits a1's m_74 and m_70 children, setting m_24/m_28 and, when m_04 is set, appending a pair entry to the context list; returns an index.
// @fnv 0x00e08240
// src: HAVOK_LIB 0x00e08240
// pdb: hkpMoppDefaultAssembler::calcTerminalIdsForChunks | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: currentNode, chunkId, currentId
int FUN_00e08240_cls::hkpMoppDefaultAssembler_calcTerminalIdsForChunks(Node* currentNode, int chunkId, int currentId)
{
    int* pIndex = &currentId;
    int idx = currentId;
    volatile int* pv = &currentId;
    *pv = idx;
    int hi = currentNode->m_40;
    currentNode->m_38 = 0;
    if (hi >= 0)
    {
        idx = 0;
    }
    else
    {
        hi = chunkId;
    }
    if (this->m_14->m_1c != 0)
    {
        currentNode->m_24 = idx;
    }
    if (currentNode->m_04 != 0)
    {
        if (this->m_14->m_1c != 0)
        {
            SlotPair* slot = currentNode->m_60;
            slot->m_10 = slot->m_00;
            PairList* list = &this->m_14->m_kNode;
            hkMemoryRouter* holder = (hkMemoryRouter*)hkMemoryRouter_getInstance();
            if (list->m_uiItemCount == (int)(list->m_capBits & 0x3fffffff))
            {
                fnv_00e08240::ResizeAudioStream(holder->m_heap, list, 8);
            }
            PairEntry* e = &list->m_data[list->m_uiItemCount];
            list->m_uiItemCount++;
            hi = (hi * 256) | idx;
            e->m_iSecondInteger = (void*)hi;
            e->m_pHeadNode = (void*)currentNode->m_60->m_00;
            currentNode->m_60->m_00 = idx;
            idx++;
        }
    }
    else
    {
        Node* f = currentNode->m_74;
        int r = this->hkpMoppDefaultAssembler_calcTerminalIdsForChunks(f, hi, idx);
        Node* g = currentNode->m_70;
        idx = this->hkpMoppDefaultAssembler_calcTerminalIdsForChunks(g, hi, r);
    }
    if (this->m_14->m_1c != 0)
    {
        currentNode->m_24 = 0;
        currentNode->m_28 = idx;
    }
    if (currentNode->m_40 < 0)
    {
        return idx;
    }
    return currentId;
}
}  // namespace fnv_00e08240

namespace fnv_00e08470 {
struct S_00e08470_Context
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    char unk38;
    int unk3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    int unk58;
    void* unk5c;
};

struct S_00e08470_Entry
{
    void* unk00;
    int unk04;
};

struct S_00e08470_Vector
{
    S_00e08470_Entry* unk00;
    int unk04;
    int unk08;
};


struct S_00e08470_Data
{
    int unk00;
    int unk04;
    int unk08;
    void* unk0c;
    hkMemoryRouter* unk10;
    S_00e08470_Vector unk14;

    void __thiscall nullsub_26();
};

extern "C" void __cdecl ResizeAudioStream(void*, S_00e08470_Vector*, int);

struct S_00e08470_Class
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    S_00e08470_Data* unk10;

    void __thiscall hkpMoppDefaultAssembler_addJumpChunk(int);
    int __thiscall hkpMoppDefaultAssembler_assembleJumpChunkCommand(S_00e08470_Context*);
};

// Calls FUN_00e06c20 with context->unk40, records the new pointer and unk40 in a vector entry, and returns its difference from the old pointer.
// @fnv 0x00e08470
// src: HAVOK_LIB 0x00e08470
// pdb: hkpMoppDefaultAssembler::assembleJumpChunkCommand | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: currentNode
int __thiscall S_00e08470_Class::hkpMoppDefaultAssembler_assembleJumpChunkCommand(S_00e08470_Context* currentNode)
{
    void* saved = this->unk10->unk0c;

    this->hkpMoppDefaultAssembler_addJumpChunk(currentNode->unk40);

    void* current = this->unk10->unk0c;

    currentNode->unk5c = current;
    currentNode->unk38 = 1;

    S_00e08470_Vector* vector = (S_00e08470_Vector*)((char*)this->unk10 + 0x14);

    hkMemoryRouter* holder = (hkMemoryRouter*)hkMemoryRouter_getInstance();

    if (vector->unk04 == (vector->unk08 & 0x3fffffff))
    {
        ResizeAudioStream((void*)holder->m_heap, vector, 8);
    }

    S_00e08470_Entry* entry = vector->unk00 + vector->unk04;
    ++vector->unk04;

    entry->unk04 = currentNode->unk40;
    entry->unk00 = current;

    this->unk10->nullsub_26();

    return (unsigned int)current - (unsigned int)saved;
}
}  // namespace fnv_00e08470

struct CodeCopySource : FUN_00e065f0
{
};

// Writes converter-produced reference data and then source's data into destination, adjusts fixups, and returns how far destination's offset grew.
// @fnv 0x00e08380
// src: HAVOK_LIB 0x00e08380
// pdb: copyCodeAndEmbedTerminals | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: chunkInfo, codeWithTerminals, codeWithoutTerminals, terminals
// pdb locals: assertBuf, ostr
static __declspec(noinline) int copyCodeAndEmbedTerminals(BvTree* chunkInfo, CodeCopySource* codeWithTerminals, hkArrayBase<int>* codeWithoutTerminals,
                                             FUN_00e065f0* terminals)
{
    int padding = (-codeWithTerminals->m_offset) & 3;
    if (!chunkInfo->m_converter)
        codeWithoutTerminals->m_iElementCount = 0;
    if (codeWithoutTerminals->m_iElementCount)
    {
        int* data = codeWithoutTerminals->m_data;
        int count = codeWithoutTerminals->m_iElementCount;
        int bytes = chunkInfo->m_converter->ConvertReferences(data, count);
        terminals->hkpMoppCodeGenerator_pad(bytes);
        hkMemoryRouter* router = hkMemoryRouter_getInstance();
        unsigned char* temporary = static_cast<unsigned char*>(router->m_stack.m_next.m_storage);
        router->m_stack.m_next.m_storage = temporary + ((bytes + 127) & ~127);
        chunkInfo->m_converter->writeBuffer(codeWithoutTerminals->m_data, codeWithoutTerminals->m_iElementCount, temporary);
        hkString_memCpy(terminals->m_data + terminals->m_pRayCursor - terminals->m_offset, temporary, bytes);
        hkMemoryRouter_getInstance()->m_stack.m_next.m_storage = temporary;
    }
    int before = terminals->m_offset;
    terminals->hkpMoppCodeGenerator_pad(codeWithTerminals->m_offset + padding);
    hkString_memCpy(terminals->m_data + terminals->m_pRayCursor - terminals->m_offset,
                 codeWithTerminals->m_data + codeWithTerminals->m_pRayCursor - codeWithTerminals->m_offset, codeWithTerminals->m_offset);
    codeWithTerminals->nullsub_26();
    copyJumpCommandInfo(&terminals->m_buffer, &codeWithTerminals->m_buffer, terminals->m_offset - codeWithTerminals->m_offset);
    terminals->nullsub_26();
    return terminals->m_offset - before;
}

int copy_code_caller(BvTree* tree, CodeCopySource* source, hkArrayBase<int>* refs, FUN_00e065f0* destination)
{
    return copyCodeAndEmbedTerminals(tree, source, refs, destination);
}

extern "C" void* __cdecl memset(void*, int, size_t);

struct ObjectWriter
{
    int unk00;
    int unk04;
    int m_cap;
    int m_pos;
    char* m_data;
    int unk14;
    int unk18;
    int unk1c;

    void FUN_00e065f0(int size) throw();
    ObjectWriter(int size) throw()
    {
        FUN_00e065f0(size);
    }
    void hkpMoppCodeGenerator_dtor() throw();
    ~ObjectWriter() throw()
    {
        hkpMoppCodeGenerator_dtor();
    }
    void FUN_00c90510() throw();
};

struct HelperEntry
{
    ObjectWriter* ptr;
    int pos;
};

struct EntryArray
{
    HelperEntry* m_data;
    int m_iElementCount;
    int m_iCapacityState;
};

struct AssemblerHelper
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    EntryArray m_entries;
    int unk1C;
};

template <typename T>
struct MyArray
{ // local form of hkArrayBase: 0x00e08cc0 needs its local memset constructor/destructor shape; shared initialization changes extent from 1020 to 1018 bytes.
    T* m_data;
    int m_iElementCount;
    int m_iCapacityState;

    __forceinline MyArray() throw()
    {
        memset(this, 0, 8);
        m_iCapacityState = 0x80000000;
    }
    __forceinline ~MyArray() throw()
    {
        hkMemoryRouter* holder = (hkMemoryRouter*)hkMemoryRouter_getInstance();
        hkMemoryAllocator* alloc = reinterpret_cast<hkMemoryAllocator*>(holder->m_heap);
        m_iElementCount = 0;
        if (!(m_iCapacityState & 0x80000000))
            alloc->bufFree(m_data, (unsigned int)((m_iCapacityState & 0x3fffffff) * 4));
    }
    __forceinline void ClearAndRelease() throw()
    {
        hkMemoryAllocator* alloc = reinterpret_cast<hkMemoryAllocator*>(((hkMemoryRouter*)hkMemoryRouter_getInstance())->m_heap);
        int cap = m_iCapacityState;
        m_iElementCount = 0;
        if (!(cap & 0x80000000))
        {
            int bytes = (cap & 0x3fffffff) * 4;
            alloc->bufFree(m_data, (unsigned int)bytes);
        }
        m_data = 0;
        m_iElementCount = 0;
    }
};

struct __declspec(align(8)) MyBounds
{
    int unk00;
    char unk04;
    char unk05;
    char unk06;
    char unk07;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;

    void BuildScaledRanges(S_00e07da0_Node* node, void* p) throw();
    void CopyOffsets(S_00e07da0_Node* node) throw();
    void hkpMoppAssemblerNodeInfo_updateExtents() throw();
};

struct __declspec(align(8)) MyRegion : public MyBounds
{
    int lo48_0;
    int lo48_1;
    int lo48_2;
    int hi54_0;
    int hi54_1;
    int hi54_2;

    void CopyOffsets(S_00e07da0_Node* node) throw();
    void BuildScaledRanges(S_00e07da0_Node* node, void* p) throw();
    void hkpMoppAssemblerNodeInfo_updateExtents() throw();
    void hkpMoppAssemblerNodeInfo_ctor(MyBounds* parentInfo, S_00e07da0_Node* node, void* codeInfo) throw();
};

extern "C" void __cdecl copyCodeAndEmbedTerminals(AssemblerHelper*, ObjectWriter*, void*) throw();

struct Tree : public FUN_00e09640_class
{
    virtual void vf04() throw();
    virtual void vf08() throw();
    virtual void vf0c() throw();
    virtual void vf10() throw();
    virtual void hkpMoppDefaultAssembler_assemble(S_00e07da0_Node* rootNode, int mgr, int minAssembledElems);
    virtual void vf18(S_00e07da0_Node* node, void* p) throw();

    int unk08;
    int unkC;
    ObjectWriter* m_stream;
    AssemblerHelper* m_pAssemblerHelper;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    int unk58;
    int unk5c;
    int unk60;
    int unk64;
    int unk68;
    int unk6c;
    int unk70;

    void hkpMoppDefaultAssembler_preCalcScale(S_00e07da0_Node* currentNode, MyBounds* parentInfo, MyRegion* currentInfo) throw();
    void BuildRegion(S_00e07da0_Node* node, MyRegion* r1, MyRegion* r2) throw();
    void OptimizeTree(S_00e07da0_Node* node, MyBounds* b1, MyBounds* b2) throw();
    void hkpMoppDefaultAssembler_calcChunkPoints(S_00e07da0_Node* currentNode, MyBounds* parentInfo, MyRegion* currentInfo, void* terminalsOut) throw();
    void hkpMoppDefaultAssembler_calcTerminalIdsForChunks(S_00e07da0_Node* currentNode, int chunkId, int currentId) throw();
    void hkpMoppDefaultAssembler_assembleSubNodeIntoChunks(S_00e07da0_Node* currentNode, MyBounds* parentInfo, MyBounds* currentInfo, int chunkId, void* terminalsOut) throw();
    void hkpMoppDefaultAssembler_addDataOffset(int offset, int numTerminals) throw();
};

typedef void(__thiscall* Vf18Fn)(Tree*, S_00e07da0_Node*, void*) throw();

// Sets up MyBounds and MyRegion state for node, then writes node data into streams via m_pAssemblerHelper if present, else calls FUN_00e08040.
// @fnv 0x00e08cc0
// src: HAVOK_LIB 0x00e08cc0
// pdb: hkpMoppDefaultAssembler::assemble | hkpMoppDefaultAssembler.obj | retail, upstream name already matches, high
// pdb params: rootNode, mgr, minAssembledElems
// pdb locals: parentInfo, currentInfo, parentCutInfo, dummyPrims, dummyInfo, dummyCode, rootTerminals, rootChunkCode
void Tree::hkpMoppDefaultAssembler_assemble(S_00e07da0_Node* rootNode, int mgr, int minAssembledElems)
{
    MyRegion p;
    MyBounds s;
    MyBounds x;
    MyRegion n;

    this->unkC = mgr;
    this->unk24 = minAssembledElems;
    this->unk28 = 0;
    this->vf18(rootNode, &this->unk30);
    if (!rootNode)
        return;

    this->unk18 = 0;
    unsigned int bits = rootNode->unk28;
    while (bits != 0)
    {
        bits >>= 1;
        ++this->unk18;
    }

    this->unk1C = this->unk18 >> 1;
    this->unk18 = ((this->unk18 / this->unk70) + 2) * this->unk70 - 1;

    s.BuildScaledRanges(rootNode, &this->unk30);
    s.unk24 = 0x10;
    s.unk30 = 0;
    s.unk2c = 0;
    s.unk28 = 0;
    s.unk00 = -1;
    s.unk34 = 0;
    s.unk04 = 0;
    s.CopyOffsets(rootNode);
    s.hkpMoppAssemblerNodeInfo_updateExtents();

    (MyBounds&)p = s;
    p.CopyOffsets(rootNode);
    p.BuildScaledRanges(rootNode, &this->unk30);
    p.unk00 = s.unk00 + 1;
    p.unk04 = 0;
    p.hkpMoppAssemblerNodeInfo_updateExtents();

    this->hkpMoppDefaultAssembler_preCalcScale(rootNode, &s, &p);

    p.BuildScaledRanges(rootNode, &this->unk30);
    p.unk24 = 0x10;
    p.unk30 = 0;
    p.unk2c = 0;
    p.unk28 = 0;
    p.unk00 = -1;
    p.unk34 = 0;
    p.unk04 = 0;
    p.CopyOffsets(rootNode);
    p.hkpMoppAssemblerNodeInfo_updateExtents();

    (MyBounds&)n = (MyBounds&)p;
    n.CopyOffsets(rootNode);
    n.BuildScaledRanges(rootNode, &this->unk30);
    n.unk00 = p.unk00 + 1;
    n.unk04 = 0;
    n.hkpMoppAssemblerNodeInfo_updateExtents();
    this->BuildRegion(rootNode, &p, &n);

    x = s;
    x.CopyOffsets(rootNode);
    x.BuildScaledRanges(rootNode, &this->unk30);
    x.unk00 = s.unk00 + 1;
    x.unk04 = 0;
    x.hkpMoppAssemblerNodeInfo_updateExtents();

    if (!this->m_pAssemblerHelper)
    {
        this->OptimizeTree(rootNode, &s, &x);
        return;
    }

    rootNode->info.unk8 = 0;
    {
        EntryArray& entries = this->m_pAssemblerHelper->m_entries;
        hkMemoryRouter* router = hkMemoryRouter_getInstance();
        if (entries.m_iElementCount == (entries.m_iCapacityState & 0x3fffffff))
            ResizeAudioStream(router->m_heap, &entries, 8);
        entries.m_iElementCount++;
    }

    ObjectWriter* saved = this->m_stream;
    p.hkpMoppAssemblerNodeInfo_ctor(&s, rootNode, &this->unk30);

    MyArray<unsigned int> arr1;
    {
        ObjectWriter stream1(0x100000);
        this->m_stream = &stream1;
        this->hkpMoppDefaultAssembler_calcChunkPoints(rootNode, &s, &p, &arr1);
        this->hkpMoppDefaultAssembler_calcTerminalIdsForChunks(rootNode, rootNode->info.unk8, 0);
    }

    arr1.ClearAndRelease();
    arr1.m_iCapacityState = 0x80000000;
    {
        ObjectWriter stream2(0x100000);
        this->m_stream = &stream2;
        this->hkpMoppDefaultAssembler_assembleSubNodeIntoChunks(rootNode, &s, &x, rootNode->info.unk8, &arr1);
        if (this->m_pAssemblerHelper->unk1C)
        {
            int pos = this->m_stream->m_pos + 5;
            this->hkpMoppDefaultAssembler_addDataOffset(pos + (-pos & 3), arr1.m_iElementCount);
        }
        this->m_stream = saved;
        copyCodeAndEmbedTerminals(this->m_pAssemblerHelper, &stream2, &arr1);
        HelperEntry* entry = &this->m_pAssemblerHelper->m_entries.m_data[rootNode->info.unk8];
        ObjectWriter* w = this->m_stream;
        entry->ptr = w;
        w->FUN_00c90510();
        entry->pos = this->m_stream->m_pos;
    }
}
