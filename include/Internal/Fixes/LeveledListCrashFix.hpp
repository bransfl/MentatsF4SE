#pragma once

namespace Internal::Fixes
{
	class LeveledListCrashFix
	{
	public:
		// installs the fix
		static void Install() noexcept;
		
		// logs any invalid forms within an item leveledlist
		static void DebugLeveledListItem(RE::TESLevItem* a_list, RE::TESForm* a_form);

		// logs any invalid forms within an actor leveledlist
		static void DebugLeveledListActor(RE::TESLevCharacter* a_list, RE::TESForm* a_form);

		// returns a vector of all of the forms in the leveledlist
		static std::vector<RE::TESForm*> GetEntries(RE::TESLeveledList* leveledList);
	};

	// these are outside of the class due to detourxs being the way it is (and skill issue)

	// item hook
	void Hook_LeveledItemAddForm(RE::BSScript::IVirtualMachine* a_vm, std::uint32_t a_stackID, RE::TESLevItem* a_leveledItemList, RE::TESForm* a_formToAdd, uint32_t a_level, uint32_t a_count);

	// actor hook
	void Hook_LeveledActorAddForm(RE::BSScript::IVirtualMachine* a_vm, std::uint32_t a_stackID, RE::TESLevCharacter* a_leveledCharacterList, RE::TESForm* a_formToAdd, uint32_t a_level);
}
