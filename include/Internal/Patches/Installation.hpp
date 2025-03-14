#pragma once

namespace Internal::Patches
{
	/**
	 * @brief Installs all patches that do not need to wait for a Messaging callback.
	 */
	void Install() noexcept;
}
