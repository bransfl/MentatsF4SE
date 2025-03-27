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
		static void Install() noexcept;

		/**
		 * @brief Class name method.
		 */
		static inline constexpr std::string_view Module::GetModuleName() noexcept
		{
			return "LeveledListEntryCountWarning"sv;
		}

	private:
		/**
		 * @brief Checks all RE::TESLevItem forms to see if they have >255 entries.
		 */
		static void CheckLeveledLists() noexcept;
	};
}
