#include "Internal/Fixes/Installation.hpp"

#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/MagicEffectConditions.hpp"
#include "Internal/Fixes/PerkEntryPoints/ApplySpells.hpp"
#include "Internal/Fixes/TransferManyItems/AddManyItems.hpp"

namespace Internal::Fixes
{
	void Install() noexcept
	{
		logger::info("Installing fixes..."sv);;

		PerkEntryPoints::ApplySpells::Install();
		TransferManyItems::AddManyItems::Install();
		// TransferManyItems::DropManyItems::Install();	still WIP
		// CellAttachDetachEvent::Install();	handled in Messaging.cpp
		LeveledListCrashFix::Install();
		MagicEffectConditions::Install();

		logger::info("Fixes installed."sv);
	}
}
