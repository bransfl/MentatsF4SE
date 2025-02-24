#include "Internal/Fixes/SewingMachineSoundFix.hpp"
#include "Internal/Config/Config.hpp"

// SoundDescriptor:
// EditorID = UIWorkshopSewingMachineRunLPM
// FormID = 0019E999

namespace Internal::Fixes::SewingMachineSoundFix
{
	void Install() noexcept
	{
		logger::info("Fix installing: SewingMachineSoundFix."sv);

		if (!Config::bSewingMachineSoundFix.GetValue()) {
			logger::info("Fix aborted: SewingMachineSoundFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		// register cellchange/load/furniture exit/etc listeners

		logger::info("Fix installed: SewingMachineSoundFix."sv);
	}
}
