#pragma once

namespace Internal::Config
{
	// fixes
	inline const auto bApplySpellsFix = REX::INI::Bool{ "Fixes"sv, "bApplySpellsFix"sv, true };
	inline const auto bBasherFix = REX::INI::Bool{ "Fixes"sv, "bBasherFix"sv, true };
	inline const auto bCastSpells = REX::INI::Bool{ "Fixes"sv, "bCastSpells"sv, true };

	inline const auto bAddManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bAddManyItemsFix"sv, true };
	inline const auto bDropManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bDropManyItemsFix"sv, true };

	inline const auto bCombatMusicFix = REX::INI::Bool{ "Fixes"sv, "bCombatMusicFix"sv, true };
	inline const auto bEncounterZoneFix = REX::INI::Bool{ "Fixes"sv, "bEncounterZoneFix"sv, true };
	inline const auto bLeveledListCrashFix = REX::INI::Bool{ "Fixes"sv, "bLeveledListCrashFix"sv, true };
	inline const auto bMagicEffectConditions = REX::INI::Bool{ "Fixes"sv, "bMagicEffectConditions"sv, true };
	inline const auto bOutfitRedressFix = REX::INI::Bool{ "Fixes"sv, "bOutfitRedressFix"sv, true };

	// patches
	inline const auto bLoadEditorIDs = REX::INI::Bool{ "Patches"sv, "bLoadEditorIDs"sv, true };

	// internal
	inline const auto bIsNG = REL::Module::IsNG();

	void Load() noexcept;
}
