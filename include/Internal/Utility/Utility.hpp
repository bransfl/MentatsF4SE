#pragma once

// common functions used in multiple fixes
namespace Internal::Utility
{
	void ExecuteCommand(std::string_view a_command, RE::TESObjectREFR* a_targetRef, bool a_silent);

	std::string_view GetModName(RE::TESForm* a_form, bool a_lastModified);

	int8_t GetNumEntries(RE::TESLeveledList* leveledList);
}
