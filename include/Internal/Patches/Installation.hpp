#pragma once

namespace Internal::Patches
{
	class Installation
		: public Installer
	{
	public:
		/**
		 * @brief Installs all patches that do not need to wait for a Messaging callback.
		 */
		static void Install() noexcept;
	};
}
