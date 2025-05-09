#pragma once

namespace Internal::Fixes
{
	class AddManyItemsFix
		: public Fix
	{
	public:
		/**
		 * @brief Installs the fix.
		 */
		static void Install() noexcept;

		/**
		 * @brief Class name method.
		 */
		static inline constexpr std::string_view Module::GetModuleName() noexcept
		{
			return "AddManyItemsFix"sv;
		}

	private:
		/**
		 * @brief Patches relevant code.
		 */
		static void Fix();
	};
}
