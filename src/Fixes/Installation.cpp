#include "Internal/Fixes/Installation.hpp"

#include "Internal/Fixes/EncounterZoneResetFix.hpp"
#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/TransferManyItems/AddManyItemsFix.hpp"
#include "Internal/Fixes/TransferManyItems/DropManyItemsFix.hpp"

namespace Internal::Fixes
{
	void Install() noexcept
	{
		// PerkEntryPoints::ApplySpellsFix::Install(); WIP

		// PerkEntryPoints::BasherFix::Install(); WIP

		TransferManyItems::AddManyItemsFix::Install();

		TransferManyItems::DropManyItemsFix::Install();

		// CombatMusicFix is handled in Messaging.cpp

		EncounterZoneResetFix::Install();

		LeveledListCrashFix::Install();

		// MagicEffectConditionsFix::Install(); WIP

		// WorkbenchSoundFix is handled in Messaging.cpp
	}
}
