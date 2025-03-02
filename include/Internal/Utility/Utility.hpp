#pragma once

// common functions used in multiple fixes
namespace Internal::Utility
{
	// executes a console command
	void ExecuteCommand(std::string_view a_command, RE::TESObjectREFR* a_targetRef, bool a_silent);

	// returns the mod that a form is from
	std::string_view GetModName(RE::TESForm* a_form, bool a_lastModified);

	// returns the amount of entries in the given LeveledList
	int8_t GetNumEntries(RE::TESLeveledList* leveledList);
}
