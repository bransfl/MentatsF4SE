#pragma once

namespace Internal::Warnings
{
	class LeveledListEntryCountWarning
		: public Warning
	{
	public:
		/**
		 * @brief Installs the warning.
		 */
		static void Install();

		/**
		 * @brief Class name method.
		 */
		static std::string_view Module::GetModuleName()
		{
			return "LeveledListEntryCountWarning"sv;
		}

	private:
		/**
		 * @brief Checks all RE::TESLevItem forms to see if they have >255 entries.
		 */
		static void CheckLeveledLists();
	};
}
