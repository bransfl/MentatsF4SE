#pragma once

namespace Internal
{
	class Logging
	{
	public:
		/**
		 * @brief Logs the user's game version, f4se version, and address library version.
		 */
		static void LogUserVersionInfo();

		/**
		 * @brief Logs a single line of hyphens.
		 */
		static void LogSeparator();
	};
}
