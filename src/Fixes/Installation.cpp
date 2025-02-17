#include "Internal/Fixes/Installation.hpp"
#include "Internal/Fixes/MagicEffectConditions.hpp"
#include "Internal/Fixes/PerkEntryPoints/ApplySpells.hpp"

namespace Internal::Fixes
{
	void Install() noexcept
	{
		logger::info("Installing fixes"sv);

		PerkEntryPoints::ApplySpells::Install();
		// CellAttachDetachEvent::Install();		handled in Messaging.cpp
		MagicEffectConditions::Install();
	}
}
