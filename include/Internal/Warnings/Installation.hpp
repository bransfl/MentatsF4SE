#pragma once

namespace Internal::Warnings
{
	class Installation
	{
	public:
		/**
		 * @brief Installs all warnings that do not need to wait for a Messaging callback.
		 */
		static void Install() noexcept;
	};
}
