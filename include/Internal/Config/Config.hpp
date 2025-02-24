#pragma once

namespace Internal::Config
{
	// false if temporarily disabled / not implemented yet

	// fixes
	inline const auto bApplySpellsFix = REX::INI::Bool{ "Fixes"sv, "bApplySpellsFix"sv, false };
	inline const auto bBasherFix = REX::INI::Bool{ "Fixes"sv, "bBasherFix"sv, false };
	inline const auto bCastSpellsFix = REX::INI::Bool{ "Fixes"sv, "bCastSpellsFix"sv, false };
	//
	inline const auto bAddManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bAddManyItemsFix"sv, true };
	inline const auto bDropManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bDropManyItemsFix"sv, true };
	//
	inline const auto bCombatMusicFix = REX::INI::Bool{ "Fixes"sv, "bCombatMusicFix"sv, true };
	inline const auto bEncounterZoneFix = REX::INI::Bool{ "Fixes"sv, "bEncounterZoneFix"sv, true };
	inline const auto bLeveledListCrashFix = REX::INI::Bool{ "Fixes"sv, "bLeveledListCrashFix"sv, true };
	inline const auto bMagicEffectConditionsFix = REX::INI::Bool{ "Fixes"sv, "bMagicEffectConditionsFix"sv, false };
	inline const auto bWorkbenchSoundFix = REX::INI::Bool{ "Fixes"sv, "bWorkbenchSoundFix"sv, true };

	void Load() noexcept;
}
