#include "StdInc.h"

#include <extensions/CommandLine.h>

#include "Population.h"
#include "Wanted.h"

void InjectHooksMain() {
    const auto now = std::chrono::high_resolution_clock::now();

    // 1. Population: 1-hour dead bodies (including dead cops) within 800m + 30s slot cooldown
    {
        RH_ScopedClass(CPopulation);
        RH_ScopedCategoryGlobal();
        RH_ScopedInstall(ManagePed, 0x611FC0);
        RH_ScopedInstall(Update, 0x616650);
    }

    // 2. Wanted: Enforce 30s cooldown on dead cop pursuit slots + make Rhino tanks vulnerable to rocket rounds
    {
        RH_ScopedClass(CWanted);
        RH_ScopedCategoryGlobal();
        RH_ScopedInstall(ComputePursuitCopToDisplace, 0x562B00);
        RH_ScopedInstall(Update, 0x562C90);
    }

    NOTSA_LOG_INFO("InjectedHooksMain(): Finished in {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now).count());

    CommandLine::PostHooksInjected();
}
