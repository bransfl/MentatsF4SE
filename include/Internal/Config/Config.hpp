#pragma once

namespace Internal::Config
{
	// config
	// inline const auto bEnableMod = REX::INI::Bool{"Config"sv, "bEnableMod"sv, true};

	// fixes
	inline const auto bApplySpellsFix = REX::INI::Bool{ "Fixes"sv, "bApplySpellsFix"sv, true};
	inline const auto bBasherFix = REX::INI::Bool{ "Fixes"sv, "bBasherFix"sv, true};
	inline const auto bCastSpells = REX::INI::Bool{ "Fixes"sv, "bCastSpells"sv, true};
	inline const auto bAddManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bAddManyItemsFix"sv, true};
	inline const auto bDropManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bDropManyItemsFix"sv, true};
	inline const auto bMagicEffectConditions = REX::INI::Bool{ "Fixes"sv, "bMagicEffectConditions"sv, true};
	inline const auto bLeveledListCrashFix = REX::INI::Bool{ "Fixes"sv, "bLeveledListCrashFix"sv, true};
	inline const auto bOutfitRedressFix = REX::INI::Bool { "Fixes"sv, "bOutfitRedressFix"sv, true};
	inline const auto bCombatMusicFix = REX::INI::Bool { "Fixes"sv, "bCombatMusicFix"sv, true};

	// patches
	inline const auto bLoadEditorIds = REX::INI::Bool{ "Patches"sv, "bLoadEditorIds"sv, true };

	void Load() noexcept;
}
