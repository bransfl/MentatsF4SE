#include "Internal/Fixes/PerkEntryPoints/CastSpells.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes::PerkEntryPoints::CastSpells
{
	void Install() noexcept {
		logger::info("Fix installing: CastSpells.");

		if (!Config::bCastSpells.GetValue()) {
			logger::info("Fix aborted: CastSpells. Reason: Fix was disabled in ini file.");
			return;
		}
		
		logger::info("Fix installed: CastSpells.");
	}
}