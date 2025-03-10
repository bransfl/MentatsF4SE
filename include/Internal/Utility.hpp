#pragma once

// common functions used in multiple fixes
namespace Internal
{
	class Utility
	{
	public:
		// executes a console command
		static void ExecuteCommand(std::string_view a_command, RE::TESObjectREFR* a_targetRef, bool a_silent);

		// returns the mod that a form is from
		static std::string_view GetModName(RE::TESForm* a_form, bool a_lastModified);

		// returns the amount of entries in the given LeveledList
		static int32_t GetNumEntries(RE::TESLeveledList* a_leveledList);
	};
}
