#pragma once

namespace Internal::Config
{
	// default is false in case a fix is removed from the ini for any reason

	// PerkEntryPoints folder
	inline const auto bApplySpellsFix = REX::INI::Bool{ "Fixes"sv, "bApplySpellsFix"sv, false };

	inline const auto bBasherFix = REX::INI::Bool{ "Fixes"sv, "bBasherFix"sv, false };

	inline const auto bAddManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bAddManyItemsFix"sv, false };

	inline const auto bDropManyItemsFix = REX::INI::Bool{ "Fixes"sv, "bDropManyItemsFix"sv, false };

	inline const auto bCombatMusicFix = REX::INI::Bool{ "Fixes"sv, "bCombatMusicFix"sv, false };

	inline const auto bEncounterZoneFix = REX::INI::Bool{ "Fixes"sv, "bEncounterZoneFix"sv, false };

	inline const auto bLeveledListCrashFix = REX::INI::Bool{ "Fixes"sv, "bLeveledListCrashFix"sv, false };
	inline const auto bLeveledListCrashFix_SanitizeLists = REX::INI::Bool{ "Fixes"sv, "bLeveledListCrashFix_SanitizeLists"sv, false };

	inline const auto bMagicEffectConditionsFix = REX::INI::Bool{ "Fixes"sv, "bMagicEffectConditionsFix"sv, false };

	inline const auto bRaceChangeFix = REX::INI::Bool{ "Fixes"sv, "bRaceChangeFix"sv, false };

	inline const auto bWorkbenchSoundFix = REX::INI::Bool{ "Fixes"sv, "bWorkbenchSoundFix"sv, false };

	void Load() noexcept;
}
