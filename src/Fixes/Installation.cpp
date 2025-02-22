#include "Internal/Fixes/Installation.hpp"
#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/MagicEffectConditions.hpp"
#include "Internal/Fixes/PerkEntryPoints/ApplySpellsFix.hpp"
#include "Internal/Fixes/TransferManyItems/AddManyItemsFix.hpp"
#include "Internal/Fixes/TransferManyItems/DropManyItemsFix.hpp"
#include "Internal/Fixes/EncounterZoneResetFix.hpp"

namespace Internal::Fixes
{
	void Install() noexcept
	{
		// PerkEntryPoints::ApplySpellsFix::Install(); WIP

		// PerkEntryPoints::BasherFix::Install(); WIP

		// PerkEntryPoints::CastSpellsFix::Install(); WIP

		TransferManyItems::AddManyItemsFix::Install();

		TransferManyItems::DropManyItemsFix::Install();

		// CombatMusicFix is handled in Messaging.cpp

		EncounterZoneResetFix::Install();

		LeveledListCrashFix::Install();

		// MagicEffectConditions::Install(); WIP

		logger::info("Fixes installed."sv);
	}
}
