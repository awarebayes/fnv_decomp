// Private declarations for BGSSaveLoadReferencesMap.
#ifndef FNV_BGSSAVELOADREFERENCESMAP_DECLS_H
#define FNV_BGSSAVELOADREFERENCESMAP_DECLS_H


#include "../../../../include/TESForm.h"
#include "../../../../include/TESObjectCELL.h"
#include "../../../../include/TESWorldSpace.h"

#include "../../../../include/NiTPointerMap.h"
#include "../../../../include/BGSCellNumericIDArrayMap.h"

#include "../../../../include/BSSimpleArray.h"

class BGSReconstructFormsInAllFilesMap;

// pdb: class BGSSaveLoadReferencesMap, size 0x30, 3 members
class BGSSaveLoadReferencesMap
{
  public:
    NiTPointerMap<unsigned int, unsigned int> MovedReferencesMap; // src: XBOX_RETAIL_PDB +0x0 (old: m_kReferenceIDMap)
    BGSCellNumericIDArrayMap InteriorReferencesMap; // src: XBOX_RETAIL_PDB +0x10 (old: m_kCellReferences)
    NiTPointerMap<unsigned int, BGSCellNumericIDArrayMap*> WorldspaceReferencesMap; // src: XBOX_RETAIL_PDB +0x20 (old: m_kWorldspaceCellReferenceMaps)

    void Clear();
    void LoadCellReferences(TESObjectCELL* cell, BGSReconstructFormsInAllFilesMap* forms);
    void ReconstructFormID(unsigned int aiRefID, BGSReconstructFormsInAllFilesMap* apReconstructForms);
};

typedef char bgssaveloadreferencesmap_cell_map_size[1];
typedef char bgssaveloadreferencesmap_references_size[1];

#endif
