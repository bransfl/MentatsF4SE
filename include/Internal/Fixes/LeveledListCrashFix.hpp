#pragma once

namespace Internal::Fixes
{
	class LeveledListCrashFix
	{
	public:
		// installs the fix
		static void Install() noexcept;

		// the hook
		static void Hook_AddScriptAddedLeveledObject(RE::TESLeveledList* a_this, RE::TESForm* a_owner, uint16_t a_level, uint16_t a_count, RE::TESForm* a_form);

		// logs any invalid forms within an item leveledlist
		static void DebugLeveledList(RE::TESLeveledList* a_list, RE::TESForm* a_form);
		
		// returns a vector of all of the forms in the leveledlist
		static std::vector<RE::TESForm*> GetEntries(RE::TESLeveledList* leveledList);
	};
}
