#include "StdInc.h"

#include <extensions/CommandLine.h>

#include "Population.h"
#include "CarCtrl.h"
#include "Wanted.h"
#include "Shadows.h"
#include "Entity/Vehicle/Vehicle.h"

void InjectHooksMain() {
    const auto now = std::chrono::high_resolution_clock::now();

    // 1. Population: 1-hour corpses within 800m + dead peds don't block new spawns
    {
        RH_ScopedClass(CPopulation);
        RH_ScopedCategoryGlobal();
        RH_ScopedInstall(ManagePed, 0x611FC0);
        RH_ScopedInstall(Update, 0x616650);
    }

    // 2. CarCtrl: 1-hour wrecked/abandoned vehicles within 800m + free traffic/cop counters
    {
        RH_ScopedClass(CCarCtrl);
        RH_ScopedCategoryGlobal();
        RH_ScopedInstall(PossiblyRemoveVehicle, 0x424F80);
    }

    // 3. Wanted: Immediately remove dead cops from m_CopsInPursuit
    {
        RH_ScopedClass(CWanted);
        RH_ScopedCategoryGlobal();
        RH_ScopedInstall(Update, 0x562C90);
    }

    // 4. Shadows: 512 permanent shadows + 1-hour blood pools & explosion scorch marks
    {
        RH_ScopedClass(CShadows);
        RH_ScopedCategory("Shadows");
        RH_ScopedInstall(Init, 0x706CD0);
        RH_ScopedInstall(TidyUpShadows, 0x707770);
        RH_ScopedInstall(AddPermanentShadow, 0x706F60);
        RH_ScopedInstall(UpdatePermanentShadows, 0x70C950);
    }

    // 5. Vehicle: Rhino tank no longer explosion-proof
    {
        RH_ScopedVirtualClass(CVehicle, 0x871e80, 66);
        RH_ScopedCategory("Vehicle");
        RH_ScopedInstall(CanVehicleBeDamaged, 0x6D1280);
    }

    NOTSA_LOG_INFO("InjectedHooksMain(): Finished in {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now).count());

    CommandLine::PostHooksInjected();
}
