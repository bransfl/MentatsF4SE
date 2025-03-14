#pragma once

namespace Internal::Fixes
{
	/**
	 * @brief Installs all fixes that do not need to wait for a Messaging callback.
	 */
	void Install() noexcept;
}
