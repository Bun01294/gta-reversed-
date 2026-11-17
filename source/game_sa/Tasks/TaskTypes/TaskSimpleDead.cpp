#include "StdInc.h"

#include "TaskSimpleDead.h"
#include "Shadows.h"
#include "Stats.h"
#include "PedClothesDesc.h"

void CTaskSimpleDead::InjectHooks() {
    RH_ScopedVirtualClass(CTaskSimpleDead, 0x86DEA4, 9);
    RH_ScopedCategory("Tasks/TaskTypes");
    RH_ScopedInstall(ProcessPed, 0x630600);
}

// NOTSA: *deathTime* originally int32
// 0x630590
CTaskSimpleDead::CTaskSimpleDead(uint32 deathTime, bool hasDrowned) :
    m_nDeathTimeMS{deathTime},
    m_bHasDrowned{hasDrowned}
{
}

// 0x636100
CTaskSimpleDead::CTaskSimpleDead(const CTaskSimpleDead& o) :
    CTaskSimpleDead{o.m_nDeathTimeMS, o.m_bHasDrowned}
{
}

// 0x630600
bool CTaskSimpleDead::ProcessPed(CPed* ped) {
    if (m_bFirstTime) {
        ped->SetPedState(PEDSTATE_DEAD);
        ped->m_nDeathTimeMS = CTimer::GetTimeInMS();
        m_nDeathTimeMS = CTimer::GetTimeInMS();
        m_bFirstTime = false;

        if (ped->bIsOnFire && ped->m_pFire) {
            ped->m_pFire->Extinguish();
            ped->bIsOnFire = false;
        }
    }

    if (!m_bBloodPuddleCreated && CTimer::GetTimeInMS() > m_nDeathTimeMS + 2000) {
        m_bBloodPuddleCreated = true;

        if (!ped->bInVehicle && !m_bHasDrowned && !ped->physicalFlags.bSubmergedInWater && !CStats::BloodyFootprintsEnabled() && !ped->bDoBloodyFootprints) {
            CVector pos = ped->GetPosition();
            if (RwMatrix* boneMat = ped->GetBoneMatrix(BONE_PELVIS)) {
                pos = *reinterpret_cast<CVector*>(RwMatrixGetPos(boneMat));
            }
            pos.z += 0.5f;

            CShadows::AddPermanentShadow(
                SHADOW_DEFAULT,
                gpBloodPoolTex,
                &pos,
                0.75f, 0.0f,
                0.0f, -0.75f,
                255,
                255, 0, 0,
                4.0f,
                3'600'000,
                1.0f
            );
        }
    }

    return false;
}
