#pragma once

namespace Internal::Warnings
{
	class LeveledListEntryCountWarning
	{
	public:
		/**
		 * @brief Installs the warning.
		 */
		static void Install();

	private:
		/**
		 * @brief Checks all RE::TESLevItem forms to see if they have >255 entries.
		 */
		static void CheckLeveledLists();
	};
}
