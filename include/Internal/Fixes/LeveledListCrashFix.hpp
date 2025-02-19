#pragma once

namespace Internal::Fixes::LeveledListCrashFix
{
	void Install() noexcept;

	void Hook_LeveledItemAddForm(std::uint64_t* a_unk, RE::BSScript::IVirtualMachine* a_vm, __int64 a_unk1, RE::TESForm* a_item, RE::TESForm* a_list, __int16 a_unk3);

	void Hook_LeveledActorAddForm(std::uint64_t* a_unk, RE::BSScript::IVirtualMachine* a_vm, __int64 a_unk1, RE::TESForm* a_actor, RE::TESForm* a_list);

	static inline REL::Relocation<decltype(&Hook_LeveledItemAddForm)> _OC_LeveledItemAddForm;
	static inline REL::Relocation<decltype(&Hook_LeveledActorAddForm)> _OC_LeveledActorAddForm;

	// reports null forms in leveledlists
	void DebugLeveledList(RE::TESLeveledList* a_list);

	// reports invalid leveledlists entries or leveledlists with >255 entries
	void Sanitize();

	// returns the total amount of leveledlist entries
	int8_t GetListEntriesCount(RE::TESLeveledList* leveledList);

	// returns a vector of all of the forms in the leveledlist
	std::vector<RE::TESForm*> GetListEntries(RE::TESLeveledList* leveledList);
}
