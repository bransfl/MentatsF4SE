#pragma once

namespace Internal::Fixes
{
	class AddManyItemsFix
	{
	public:
		/**
		 * @brief Installs the fix.
		 */
		static void Install() noexcept;

	private:
		/**
		 * @brief Patches relevant code.
		 */
		static void Fix();
	};
}
