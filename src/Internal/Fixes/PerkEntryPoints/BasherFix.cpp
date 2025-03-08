#include "Internal/Fixes/PerkEntryPoints/BasherFix.hpp"
#include "Internal/Config.hpp"

// fixes entrypoint ModOutgoingLimbBashDamage so it actually works
// note - this will likely require RE, and wont be made until RE is done.
namespace Internal::Fixes::PerkEntryPoints
{
	void BasherFix::Install() noexcept
	{
		logger::info("Fix installing: BasherFix."sv);

		if (!Config::bBasherFix.GetValue()) {
			logger::info("Fix aborted: BasherFix. Reason: Fix was disabled in config file."sv);
			return;
		}

		logger::info("Fix installed: BasherFix."sv);
	}

	// RE::BGSEntryPoint::ENTRY_POINT::kModBashingDamage = 0x1A,

}
