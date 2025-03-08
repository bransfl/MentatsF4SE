#pragma once

namespace Internal::Config
{
	void Load() noexcept;

	// default is false in case a fix is removed from the ini for any reason

	// FIXES
	inline const auto bApplySpellsFix = REX::TOML::Bool{ "Fixes"sv, "bApplySpellsFix"sv, false };

	inline const auto bBasherFix = REX::TOML::Bool{ "Fixes"sv, "bBasherFix"sv, false };

	inline const auto bAddManyItemsFix = REX::TOML::Bool{ "Fixes"sv, "bAddManyItemsFix"sv, false };

	inline const auto bDropManyItemsFix = REX::TOML::Bool{ "Fixes"sv, "bDropManyItemsFix"sv, false };

	inline const auto bActorCauseSaveBloatFix = REX::TOML::Bool{ "Fixes"sv, "bActorCauseSaveBloatFix"sv, false };

	inline const auto bCombatMusicFix = REX::TOML::Bool{ "Fixes"sv, "bCombatMusicFix"sv, false };

	inline const auto bEncounterZoneResetFix = REX::TOML::Bool{ "Fixes"sv, "bEncounterZoneResetFix"sv, false };

	inline const auto bLeveledListCrashFix = REX::TOML::Bool{ "Fixes"sv, "bLeveledListCrashFix"sv, false };

	inline const auto bMagicEffectConditionsFix = REX::TOML::Bool{ "Fixes"sv, "bMagicEffectConditionsFix"sv, false };

	inline const auto bStolenPowerArmorOwnershipFix = REX::TOML::Bool{ "Fixes"sv, "bStolenPowerArmorOwnershipFix"sv, false };

	inline const auto bWorkbenchSoundFix = REX::TOML::Bool{ "Fixes"sv, "bWorkbenchSoundFix"sv, false };

	// PATCHES
	inline const auto bPuddleCubemapsPatch = REX::TOML::Bool{ "Patches"sv, "bPuddleCubemapsPatch"sv, false };

	// WARNINGS
	inline const auto bDupeAddonNodesWarning = REX::TOML::Bool{ "Warnings"sv, "bDupeAddonNodesWarning"sv, false };

	inline const auto bLeveledListEntryCountWarning = REX::TOML::Bool{ "Warnings"sv, "bLeveledListEntryCountWarning"sv, false };
}
