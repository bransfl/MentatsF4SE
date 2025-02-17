#pragma once

namespace Internal::Config
{
	// config
	inline const auto bEnableMod = REX::INI::Bool{"Config"sv, "bEnableMod"sv, true};

	// fixes
	inline const auto bApplyCombatHitSpellFix = REX::INI::Bool{ "Fixes"sv, "bApplyCombatHitSpellFix"sv, true};

	// patches
	inline const auto bLoadEditorIds = REX::INI::Bool{ "Patches"sv, "bLoadEditorIds"sv, true };

	void Load() noexcept;
}
