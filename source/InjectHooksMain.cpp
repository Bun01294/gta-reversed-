#include "StdInc.h"

#include <extensions/CommandLine.h>

#include "Pools/Pools.h"
#include "Population.h"
#include "CarCtrl.h"
#include "Wanted.h"
#include "Shadows.h"
#include "Tasks/TaskTypes/TaskSimpleDead.h"
#include "Explosion.h"
#include "Entity/Vehicle/Vehicle.h"

void InjectHooksMain() {
    const auto now = std::chrono::high_resolution_clock::now();

    // Hook ONLY the persistence & gameplay classes we modified,
    // avoiding all conflicts with CheatMenuSA, Limit Adjusters, SilentPatch, and other ASI mods:
    CPools::InjectHooks();
    CPopulation::InjectHooks();
    CCarCtrl::InjectHooks();
    CWanted::InjectHooks();
    CStaticShadow::InjectHooks();
    CShadows::InjectHooks();
    CTaskSimpleDead::InjectHooks();
    CExplosion::InjectHooks();
    CVehicle::InjectHooks();

    NOTSA_LOG_INFO("InjectedHooksMain(): Finished in {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now).count());

    CommandLine::PostHooksInjected();
}
