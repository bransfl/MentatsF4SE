#include "Internal/Fixes/Installation.hpp"
#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/MagicEffectConditions.hpp"
#include "Internal/Fixes/OutfitRedressFix.hpp"
#include "Internal/Fixes/PerkEntryPoints/ApplySpells.hpp"
#include "Internal/Fixes/TransferManyItems/AddManyItems.hpp"

namespace Internal::Fixes
{
	void Install() noexcept
	{
		logger::info("Installing fixes..."sv);

		// PerkEntryPoints::ApplySpells::Install();
		// PerkEntryPoints::BasherFix::Install(); WIP
		// PerkEntryPoints::CastSpells::Install(); WIP
		TransferManyItems::AddManyItems::Install();
		// TransferManyItems::DropManyItems::Install(); WIP
		CombatMusicFix::Install();
		LeveledListCrashFix::Install();
		MagicEffectConditions::Install();
		OutfitRedressFix::Install();

		logger::info("Fixes installed."sv);
	}
}
