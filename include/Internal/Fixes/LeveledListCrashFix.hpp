#pragma once

namespace Internal::Fixes::LeveledListCrashFix
{
	// installs the fix
	void Install() noexcept;

	// the hooks
	void Hook_LeveledItemAddForm(RE::BSScript::IVirtualMachine* a_vm, std::uint32_t a_stackID, RE::TESLevItem* a_leveledItemList, RE::TESForm* a_formToAdd, uint32_t a_level, uint32_t a_count);
	void Hook_LeveledActorAddForm(RE::BSScript::IVirtualMachine* a_vm, std::uint32_t a_stackID, RE::TESLevCharacter* a_leveledCharacterList, RE::TESForm* a_formToAdd, uint32_t a_level);

	// reports null forms in leveledlists
	void DebugLeveledList(RE::TESLeveledList* a_list);

	// reports invalid leveledlists entries or leveledlists with >255 entries
	void Sanitize();

	// returns the total amount of leveledlist entries
	int8_t GetNumEntries(RE::TESLeveledList* leveledList);

	// returns a vector of all of the forms in the leveledlist
	std::vector<RE::TESForm*> GetEntries(RE::TESLeveledList* leveledList);
}
