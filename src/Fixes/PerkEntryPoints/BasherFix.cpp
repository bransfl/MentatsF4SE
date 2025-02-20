#include "Internal/Fixes/PerkEntryPoints/BasherFix.hpp"
#include "Internal/Config/Config.hpp"

// fixes entrypoint ModOutgoingLimbBashDamage so it actually works
// note - this will likely require RE, and wont be made until RE is done.
namespace Internal::Fixes::PerkEntryPoints::BasherFix
{
	void Install() noexcept
	{
		logger::info("Fix installing: BasherFix.");

		if (!Config::bBasherFix.GetValue()) {
			logger::info("Fix aborted: BasherFix. Reason: Fix was disabled in ini file.");
			return;
		}

		logger::info("Fix installed: BasherFix.");
	}

	// RE::BGSEntryPoint::ENTRY_POINT::kModBashingDamage = 0x1A,

}
