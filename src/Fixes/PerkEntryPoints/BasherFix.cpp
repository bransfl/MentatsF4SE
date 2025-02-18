#include "Internal/Config/Config.hpp"
#include "Internal/Fixes/PerkEntryPoints/BasherFix.hpp"

// fixes entrypoint kModOutgoingLimbBashDamage so it actually works
namespace Internal::Fixes::PerkEntryPoints::BasherFix
{
	void Install() noexcept {
		if (!Config::bBasherFix.GetValue()) {
			logger::info("PerkEntryPoints::BasherFix -> Fix was disabled in the ini file. Fix aborted.");
			return;
		}
	}
}
