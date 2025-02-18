#pragma once

namespace Internal::Fixes::LeveledListCrashFix
{
	void Install() noexcept;

	RE::LEVELED_OBJECT* Hook_AddLeveledObject(RE::TESLeveledList* a_this, uint16_t a_level, uint16_t a_count, int8_t a_chanceNone, RE::TESForm* a_item, RE::ContainerItemExtra* a_itemExtra);

	// original function for TAddLeveledObject()
	static inline REL::Relocation<decltype(&Hook_AddLeveledObject)> _originalCall;

	// reports null forms in leveledlists
	void DebugLeveledList(RE::TESLeveledList* a_list);

	// reports invalid leveledlists entries or leveledlists with >255 entries
	void Sanitize();

	// returns the total amount of leveledlist entries
	int8_t GetListEntriesCount(RE::TESLeveledList* leveledList);

	// returns a vector of all of the forms in the leveledlist
	std::vector<RE::TESForm*> GetListEntries(RE::TESLeveledList* leveledList);
}
