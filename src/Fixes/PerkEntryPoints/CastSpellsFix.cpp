#include "Internal/Fixes/PerkEntryPoints/CastSpellsFix.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes::PerkEntryPoints::CastSpellsFix
{
	void Install() noexcept {
		logger::info("Fix installing: CastSpellsFix."sv);

		if (!Config::bCastSpellsFix.GetValue()) {
			logger::info("Fix aborted: CastSpellsFix. Reason: Fix was disabled in ini file."sv);
			return;
		}
		
		logger::info("Fix installed: CastSpellsFix."sv);
	}
}