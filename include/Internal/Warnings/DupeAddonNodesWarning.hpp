#pragma once

namespace Internal::Warnings
{
	class DupeAddonNodesWarning
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
			return "DupeAddonNodesWarning"sv;
		}

	private:
		/**
		 * @brief Checks all RE::BGSAddonNode forms and their member variable index to see if any have the same index.
		 */
		static void CheckDupeAddonNodes() noexcept;

		/**
		 * @brief Clears nodeMap to free up memory after this warning has been processed.
		 */
		static void ClearNodeMap() noexcept;
	};
}
