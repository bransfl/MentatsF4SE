#pragma once

namespace Internal::Fixes::LeveledListCrashFix
{
	void InstallHooks();
	void Install() noexcept;

	namespace Hooks
	{
		void Hookedmem_LeveledItem_AddForm(std::uint64_t* a_unk, RE::BSScript::IVirtualMachine* a_vm, __int64 a_unk1, RE::TESForm* a_item, RE::TESForm* a_unk2, __int16 a_unk3);
		void Hookedmem_LeveledActor_AddForm(std::uint64_t* a_unk, RE::BSScript::IVirtualMachine* a_vm, __int64 a_unk1, RE::TESForm* a_actor, RE::TESForm* a_unk2);

		void DebugLeveledList(RE::TESLeveledList* a_list);
	}

	namespace Sanitizer
	{
		void Sanitize();
	}

	// returns the total amount of leveledlist entries
	int8_t GetListEntriesCount(RE::TESLeveledList* leveledList);

	// returns a vector of all of the forms in the leveledlist
	std::vector<RE::TESForm*> GetListEntries(RE::TESLeveledList* leveledList);
}
