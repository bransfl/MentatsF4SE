#include "Internal/Fixes/Installation.hpp"
#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/MagicEffectConditions.hpp"
#include "Internal/Fixes/OutfitRedressFix.hpp"
#include "Internal/Fixes/PerkEntryPoints/ApplySpellsFix.hpp"
#include "Internal/Fixes/TransferManyItems/AddManyItemsFix.hpp"

namespace Internal::Fixes
{
	void Install() noexcept
	{
		logger::info("Installing fixes..."sv);

		// PerkEntryPoints::ApplySpellsFix::Install(); WIP

		// PerkEntryPoints::BasherFix::Install(); WIP

		// PerkEntryPoints::CastSpellsFix::Install(); WIP

		TransferManyItems::AddManyItems::Install();

		// TransferManyItems::DropManyItems::Install(); WIP

		// CombatMusicFix is handled in Messaging.cpp

		LeveledListCrashFix::Install();

		MagicEffectConditions::Install();

		OutfitRedressFix::Install();

		logger::info("Fixes installed."sv);
	}
}
