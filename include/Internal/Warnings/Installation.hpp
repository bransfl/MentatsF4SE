#pragma once

namespace Internal::Warnings
{
	/**
	 * @brief Installs all warnings that do not need to wait for a Messaging callback.
	 */
	void Install() noexcept;
}
