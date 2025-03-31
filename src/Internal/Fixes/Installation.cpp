#include "Internal/Fixes/Installation.hpp"
#include "Internal/Fixes/AddManyItemsFix.hpp"
#include "Internal/Fixes/DropManyItemsFix.hpp"
#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/MagicEffectConditionsFix.hpp"

namespace Internal::Fixes
{
	void Installation::Install() noexcept
	{
		logger::info("Fixes installing..."sv);

		// ActorCauseSaveBloatFix is handled in Messaging

		AddManyItemsFix::Install();

		// ApplySpellsFix::Install(); WIP

		// BasherFix::Install(); WIP

		// CombatMusicFix is handled in Messaging

		DropManyItemsFix::Install();

		LeveledListCrashFix::Install();

		MagicEffectConditionsFix::Install();

		// StolenPowerArmorOwnershipFix is handled in Messaging

		// WorkbenchSoundFix is handled in Messaging

		logger::info("Fixes installed."sv);
	}
}
