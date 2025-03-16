#pragma once

namespace Internal::Fixes
{
	class LeveledListCrashFix
	{
	public:
		/**
		 * @brief Installs the fix.
		 */
		static void Install() noexcept;

	private:
		/**
		 * @brief Hook for the shared internal function that the game uses for all LL injection, including items/actors/etc.
		 * @details This hook checks the amount of entries in a_this before injection.
		 * 			If the entry count is less than 255, this behaves normally.
		 * 			If the entry count is above 255, this prevents injection and logs a warning for the user.
		 * @param a_this The LeveledList to inject into.
		 * @param a_owner Unsure. This might be the owner of the form that injected it, such as the injection quest?
		 * @param a_level The minimum level for a_form to appear in a_this.
		 * @param a_count The amount of a_form to inject into a_this.
		 * @param a_form The form that will be injected into to a_this.
		 */
		static void Hook_AddScriptAddedLeveledObject(RE::TESLeveledList* a_this, RE::TESForm* a_owner, uint16_t a_level, uint16_t a_count, RE::TESForm* a_form);

		/**
		 * @brief When an injection fails or is blocked, this logs the given list, blocked form, and any invalid forms within the given list.
		 * @param a_list The LeveledList that was attempted to be inserted into.
		 * @param a_form The form that was attempted to be inserted into a_list.
		 */
		static void DebugLeveledList(RE::TESLeveledList* a_list, RE::TESForm* a_form);

		/**
		 * @brief Returns all form entries in the given LeveledList.
		 * @param a_leveledList The LeveledList to process.
		 * @return Vector of all forms contained witin a_leveledList.
		 */
		static std::vector<RE::TESForm*> GetEntries(RE::TESLeveledList* a_leveledList);
	};
}
