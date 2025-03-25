#pragma once

namespace Internal
{
	/**
	 * @class Shared utility functions used in multiple fixes.
	 */
	class Utility
	{
	public:
		/**
		 * @brief Runs a console command.
		 * @param a_command The command to execute.
		 * @param a_targetRef The target to execute this command on. This can be nullptr.
		 * @param a_silent If the console command should be visible in the console's history.
		 */
		static void ExecuteCommand(std::string_view a_command, RE::TESObjectREFR* a_targetRef, bool a_silent);

		/**
		 * @brief Returns the mod that a form is defined in, or was last modified by.
		 * @param a_form The form to process.
		 * @param a_lastModified If true, returns the name of the mod that last modified this form.
		 * 						 If false, returns the mame of the mod that the form is defined in.
		 */
		static std::string_view GetModName(RE::TESForm* a_form, bool a_lastModified);

		/**
		 * @brief Returns the amount of entries in the given LeveledList.
		 * @details We return an int32_t instead of an int8_t to prevent the value from overflowing if the given LeveledList has more than 255 entries.
		 * @param a_leveledList The LeveledList to process.
		 * @return The amount of entries in this LeveledList.
		 */
		static int32_t GetNumEntries(RE::TESLeveledList* a_leveledList);

		/**
		 * @brief Returns formatted FormID/EditorID information for a_form.
		 * @param a_form The form to process.
		 * @return String representation of a_form.
		 */
		static std::string_view GetFormInfo(RE::TESForm* a_form);
	};
}
