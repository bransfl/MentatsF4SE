#pragma once

namespace Internal::Fixes
{
	class Installation
	{
	public:
		/**
		 * @brief Installs all fixes that do not need to wait for a Messaging callback.
		 */
		static void Install() noexcept;
	};
}
