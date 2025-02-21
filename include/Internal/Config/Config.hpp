#pragma once

namespace Internal::Config
{
	// fixes
	inline const auto bApplySpellsFix = REX::INI::Bool{ "Fixes"sv, "bApplySpellsFix"sv, true };
	inline const auto bBasherFix = REX::INI::Bool{ "Fixes"sv, "bBasherFix"sv, true };
	inline const auto bCastSpellsFix = REX::INI::Bool{ "Fixes"sv, "bCastSpellsFix"sv, true };
	//
	inline const auto bAddManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bAddManyItemsFix"sv, true };
	inline const auto bDropManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bDropManyItemsFix"sv, true };
	//
	inline const auto bCombatMusicFix = REX::INI::Bool{ "Fixes"sv, "bCombatMusicFix"sv, true };
	inline const auto bEncounterZoneFix = REX::INI::Bool{ "Fixes"sv, "bEncounterZoneFix"sv, true };
	inline const auto bLeveledListCrashFix = REX::INI::Bool{ "Fixes"sv, "bLeveledListCrashFix"sv, true };
	inline const auto bMagicEffectConditions = REX::INI::Bool{ "Fixes"sv, "bMagicEffectConditions"sv, true };

	void Load() noexcept;
}
