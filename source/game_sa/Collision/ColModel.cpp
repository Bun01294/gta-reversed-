#include "StdInc.h"

#include "ColModel.h"

//#define COL_EXTRA_DEBUG

#ifdef COL_DEBUG
#define DEV_LOG_COL(...) NOTSA_LOG_DEBUG(__VA_ARGS__)
#else
#define DEV_LOG_COL(...)
#endif

void CColModel::InjectHooks() {
    RH_ScopedClass(CColModel);
    RH_ScopedCategory("Collision");

    RH_ScopedInstall(operator new, 0x40FC30, { .Reversed = false });
    RH_ScopedInstall(operator delete, 0x40FC40, { .Reversed = false });
    RH_ScopedInstall(operator=, 0x40F7C0, { .Reversed = false });
    RH_ScopedInstall(MakeMultipleAlloc, 0x40F740, { .Reversed = false });
    RH_ScopedOverloadedInstall(AllocateData, "void", 0x40F810, void(CColModel::*)(), { .Reversed = false });
    RH_ScopedOverloadedInstall(AllocateData, "params", 0x40F870, void(CColModel::*)(int32, int32, int32, int32, int32, bool), { .Reversed = false });
    RH_ScopedInstall(RemoveCollisionVolumes, 0x40F9E0, { .Reversed = false });
    RH_ScopedInstall(CalculateTrianglePlanes, 0x40FA30, { .Reversed = false });
    RH_ScopedInstall(RemoveTrianglePlanes, 0x40FA40, { .Reversed = false });
}

CColModel::CColModel() : m_boundBox() {
    m_nColSlot              = 0;
    m_pColData              = nullptr;
    m_bHasCollisionVolumes  = false;
    m_bIsSingleColDataAlloc = false;
    m_IsActive              = true;
}

CColModel::~CColModel() {
    if (!m_IsActive) {
        return;
    }
    RemoveCollisionVolumes();
}

// 0x40F7C0
CColModel& CColModel::operator=(const CColModel& colModel) {
    return plugin::CallMethodAndReturn<CColModel&, 0x40F7C0, CColModel*, const CColModel&>(this, colModel);
}

// 0x40F740
void CColModel::MakeMultipleAlloc() {
    plugin::CallMethod<0x40F740, CColModel*>(this);
}

// 0x40F810
void CColModel::AllocateData() {
    plugin::CallMethod<0x40F810, CColModel*>(this);
}

// 0x40F870
void CColModel::AllocateData(int32 numSpheres, int32 numBoxes, int32 numLines, int32 numVertices, int32 numTriangles, bool bUsesDisks) {
    plugin::CallMethod<0x40F870, CColModel*, int32, int32, int32, int32, int32, bool>(this, numSpheres, numBoxes, numLines, numVertices, numTriangles, bUsesDisks);
}

// 0x40F9B0
void CColModel::AllocateData(int32 size) {
    plugin::CallMethod<0x40F9B0, CColModel*, int32>(this, size);
}

// 0x40F9E0
void CColModel::RemoveCollisionVolumes() {
    plugin::CallMethod<0x40F9E0, CColModel*>(this);
}

// 0x40FA30
void CColModel::CalculateTrianglePlanes() {
    plugin::CallMethod<0x40FA30, CColModel*>(this);
}

// 0x40FA40
void CColModel::RemoveTrianglePlanes() {
    plugin::CallMethod<0x40FA40, CColModel*>(this);
}

// 0x40FC30
void* CColModel::operator new(unsigned size) {
    return GetColModelPool()->New();
}

// 0x40FC40
void CColModel::operator delete(void* data) {
    GetColModelPool()->Delete(static_cast<CColModel*>(data));
}
