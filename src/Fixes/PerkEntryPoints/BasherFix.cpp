#include "Internal/Config/Config.hpp"
#include "Internal/Fixes/PerkEntryPoints/BasherFix.hpp"

// fixes entrypoint kModOutgoingLimbBashDamage so it actually works
namespace Internal::Fixes::PerkEntryPoints::BasherFix
{
	void Install() noexcept {
		logger::info("Fix installing: BasherFix.");

		if (!Config::bBasherFix.GetValue()) {
			logger::info("Fix aborted: BasherFix. Reason: Fix was disabled in ini file.");
			return;
		}
		
		logger::info("Fix installed: BasherFix.");
	}
}
