#pragma once

namespace Internal::Config
{
	// config
	// inline const auto bEnableMod = REX::INI::Bool{"Config"sv, "bEnableMod"sv, true};

	// fixes
	inline const auto bApplyCombatHitSpellFix = REX::INI::Bool{ "Fixes"sv, "bApplyCombatHitSpellFix"sv, true};
	inline const auto bBasherFix = REX::INI::Bool{ "Fixes"sv, "bBasherFix"sv, true};
	inline const auto bAddManyItems = REX::INI::Bool{ "Fixes"sv, "bAddManyItems"sv, true};
	inline const auto bDropManyItems = REX::INI::Bool{ "Fixes"sv, "bDropManyItems"sv, true};
	inline const auto bMagicEffectConditions = REX::INI::Bool{ "Fixes"sv, "bMagicEffectConditions"sv, true};
	inline const auto bCellAttachDetachEvent = REX::INI::Bool{ "Fixes"sv, "bCellAttachDetachEvent"sv, true};
	inline const auto bLeveledListCrashFix = REX::INI::Bool{ "Fixes"sv, "bLeveledListCrashFix"sv, true};

	// patches
	inline const auto bLoadEditorIds = REX::INI::Bool{ "Patches"sv, "bLoadEditorIds"sv, true };
	inline const auto bConsoleHelpPrintEditorID = REX::INI::Bool{ "Patches"sv, "bConsoleHelpPrintEditorID"sv, true };

	void Load() noexcept;
}
